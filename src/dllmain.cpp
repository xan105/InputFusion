/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dllmain.h"
#include "flags.h"
#include "detours.h"
#include "util.h"
#include "version.h"
#include "../vendor/SimpleINI/SimpleIni.h"

HANDLE InputFusion_SDL_Init_Wait = nullptr;
HANDLE InputFusion_SDL_Quit_Wait = nullptr;
std::atomic<bool> InputFusion_SDL_Event_Loop(true);

namespace {
    void InputFusion_SDL_SetHints() {

        CSimpleIniW ini;
        ini.SetUnicode();

        std::wstring file = util::GetModuleDirectory() + L"InputFusion.ini";

        struct {
            bool HIDAPI = true;   // HID
            bool GAMEINPUT = true;   // GameInput
            bool WGI = true;   // Windows.Gaming.Input (WinRT)
            bool XINPUT = false;  // Xinput 
            bool DINPUT8 = false;  // Dinput8
            bool RAWINPUT = false;  // RAW (Xbox controllers) | Disabled for now cf: https://github.com/libsdl-org/SDL/issues/13047#issuecomment-2913284199
            std::wstring CONTROLLER_MAPPING = L"";
            std::wstring CONTROLLER_MAPPING_FILE = L"";
        } SDL_Hints;

        if (ini.LoadFile(file.c_str()) == SI_OK) {
            SDL_Hints.HIDAPI = ini.GetBoolValue(L"SDL", L"HIDAPI", true);
            SDL_Hints.GAMEINPUT = ini.GetBoolValue(L"SDL", L"GAMEINPUT", true);
            SDL_Hints.WGI = ini.GetBoolValue(L"SDL", L"WGI", true);
            SDL_Hints.XINPUT = ini.GetBoolValue(L"SDL", L"XINPUT", false);
            SDL_Hints.DINPUT8 = ini.GetBoolValue(L"SDL", L"DINPUT8", false);
            SDL_Hints.RAWINPUT = ini.GetBoolValue(L"SDL", L"RAWINPUT", false);
            SDL_Hints.CONTROLLER_MAPPING = ini.GetValue(L"SDL", L"CONTROLLER_MAPPING", L"");
            SDL_Hints.CONTROLLER_MAPPING_FILE = ini.GetValue(L"SDL", L"CONTROLLER_MAPPING_FILE", L"");

            SDL_Log("Parsed SDL Hints from [\"%s\"]", util::WStringToUtf8(file).c_str());
        }

        SDL_SetHint(SDL_HINT_JOYSTICK_DIRECTINPUT, SDL_Hints.DINPUT8 ? "1" : "0");
        SDL_SetHint(SDL_HINT_XINPUT_ENABLED, SDL_Hints.XINPUT ? "1" : "0");
        SDL_SetHint(SDL_HINT_JOYSTICK_WGI, SDL_Hints.WGI ? "1" : "0");
        SDL_SetHint(SDL_HINT_JOYSTICK_GAMEINPUT, SDL_Hints.GAMEINPUT ? "1" : "0");
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, SDL_Hints.HIDAPI ? "1" : "0");
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS3, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "1");
        SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, SDL_Hints.RAWINPUT ? "1" : "0");
        SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "1");  // Raw input pulls data from WGI/XInput providing better support for Xbox controllers                                                               
        SDL_SetHint(SDL_HINT_JOYSTICK_THREAD, "1");                     // Xbox controllers raw input (SDL is not running in the main thread)

        // https://github.com/libsdl-org/SDL/pull/14353
        // Should be merged in SDL3 3.6
        // SDL_SetHint(SDL_HINT_JOYSTICK_DSU, "1");                         // DualShock UDP (DS4Windows, BetterJoy, etc.)

        if (!SDL_Hints.CONTROLLER_MAPPING.empty()) {
            SDL_SetHint(SDL_HINT_GAMECONTROLLERCONFIG, util::WStringToUtf8(SDL_Hints.CONTROLLER_MAPPING).c_str());
        }
        if (!SDL_Hints.CONTROLLER_MAPPING_FILE.empty()) {
            SDL_SetHint(SDL_HINT_GAMECONTROLLERCONFIG_FILE, util::WStringToUtf8(SDL_Hints.CONTROLLER_MAPPING_FILE).c_str());
        }
    }

    bool InputFusion_SDL_Init() {
        SDL_Log("InputFusion version %s", VER_FILEVERSION_STR);

        #if defined(GAMEINPUT_EXPORTS)
            SDL_Log("InputFusion is running as GameInput dll");
            if (SDL_GetHintBoolean(SDL_HINT_JOYSTICK_GAMEINPUT, false) && SDL_SetHintWithPriority(SDL_HINT_JOYSTICK_GAMEINPUT, "0", SDL_HINT_OVERRIDE)) {
                SDL_Log("-> Disabled GameInput within SDL to prevent conflict!");
            }
        #elif defined(XINPUT_EXPORTS)
            SDL_Log("InputFusion is running as XInput dll");
            if (SDL_GetHintBoolean(SDL_HINT_XINPUT_ENABLED, false) && SDL_SetHintWithPriority(SDL_HINT_XINPUT_ENABLED, "0", SDL_HINT_OVERRIDE)) {
                SDL_Log("-> Disabled XInput within SDL to prevent conflict!");
            }
        #elif defined(DINPUT8_EXPORTS)
            SDL_Log("InputFusion is running as DInput8 dll");
            if (SDL_GetHintBoolean(SDL_HINT_JOYSTICK_DIRECTINPUT, false) && SDL_SetHintWithPriority(SDL_HINT_JOYSTICK_DIRECTINPUT, "0", SDL_HINT_OVERRIDE)) {
                SDL_Log("-> Disabled DirectInput within SDL to prevent conflict!");
            }
        #elif defined(DINPUT_EXPORTS)
            SDL_Log("InputFusion is running as DInput dll");
        #elif defined(WINMM_EXPORTS)
            SDL_Log("InputFusion is running as WinMM dll");
        #endif

        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, VER_PRODUCTNAME_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, VER_FILEVERSION_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, VER_COMPANYNAME_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, VER_LEGALCOPYRIGHT_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, VER_COMPANYDOMAIN_STR);
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, "com.xan105.inputfusion");

        if (!SDL_Init(SDL_INIT_GAMEPAD)) {
            SDL_LogError(SDL_LOG_CATEGORY_INPUT, "SDL_INIT_GAMEPAD > ERROR: %s", SDL_GetError());
            if (InputFusion_SDL_Init_Wait) {
                SetEvent(InputFusion_SDL_Init_Wait);
            }
            return false;
        }
        SDL_Log("SDL_INIT_GAMEPAD");

        const int version = SDL_GetVersion();
        SDL_Log("SDL version %d.%d.%d",
            SDL_VERSIONNUM_MAJOR(version),
            SDL_VERSIONNUM_MINOR(version),
            SDL_VERSIONNUM_MICRO(version));

        SDL_Log("SDL controller handling:\n"
            "- DirectInput: %s\n"
            "- XInput: %s\n"
            "- WGI: %s\n"
            "- GameInput: %s\n"
            "- HID: %s\n"
            "- RAWINPUT: %s",
            //"- DSU: %s"
            SDL_GetHint(SDL_HINT_JOYSTICK_DIRECTINPUT),
            SDL_GetHint(SDL_HINT_XINPUT_ENABLED),
            SDL_GetHint(SDL_HINT_JOYSTICK_WGI),
            SDL_GetHint(SDL_HINT_JOYSTICK_GAMEINPUT),
            SDL_GetHint(SDL_HINT_JOYSTICK_HIDAPI),
            SDL_GetHint(SDL_HINT_JOYSTICK_RAWINPUT));
        //SDL_GetHint(SDL_HINT_JOYSTICK_DSU)

        if (InputFusion_SDL_Init_Wait) {
            SetEvent(InputFusion_SDL_Init_Wait);
        }
        return true;
    }

    void InputFusion_SDL_Quit() {
        SDL_Log("SDL_Quit");
        SDL_Quit();
        if (InputFusion_SDL_Quit_Wait) {
            SetEvent(InputFusion_SDL_Quit_Wait);
        }
        if (InputFusion_SDL_Init_Wait) {
            CloseHandle(InputFusion_SDL_Init_Wait);
            InputFusion_SDL_Init_Wait = nullptr;
        }
    }

    void InputFusion_SDL_CloseGamepads() {
        int count;

        SDL_JoystickID* gamepads = SDL_GetGamepads(&count);
        if (gamepads == nullptr) return;

        for (int i = 0; i < count; i++) {
            SDL_JoystickID id = gamepads[i];
            SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
            if (gamepad == nullptr) continue;
            SDL_SetGamepadPlayerIndex(gamepad, -1); //turn off player light
            if (SDL_GetGamepadType(gamepad) == SDL_GAMEPAD_TYPE_PS4) {
                SDL_SetGamepadLED(gamepad, 10, 20, 20); //Not the exact default color (no player) but close enough
            }
            SDL_Log("Closing gamepad: %s", SDL_GetGamepadName(gamepad));
            SDL_CloseGamepad(gamepad);
        }
        SDL_free(gamepads);
        SDL_Log("Closed gamepads");
    }

    void InputFusion_SDL_EventLoop() {
        SDL_Event event;
        while (InputFusion_SDL_Event_Loop) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_EVENT_GAMEPAD_ADDED: {
                    SDL_Log("SDL_EVENT_GAMEPAD_ADDED");
                    SDL_JoystickID id = event.gdevice.which;
                    SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
                    if (gamepad != nullptr) {
                        std::string name = SDL_GetGamepadName(gamepad);
                        SDL_Log("Open gamepad: %s", name.c_str());

                        if (name.find("Xbox") != std::string::npos) { //https://github.com/libsdl-org/SDL/issues/12071#issuecomment-2682639565
                            SDL_GUID guid = SDL_GetGamepadGUIDForID(id);
                            char buffer[64];
                            SDL_GUIDToString(guid, buffer, sizeof(buffer));
                            std::string guidStr(buffer);
                            if (guidStr.size() >= 4) {
                                std::string suffix = guidStr.substr(guidStr.size() - 4);
                                if (suffix == "7200") {
                                    SDL_Log("%s is using RAW Input", name.c_str());
                                }
                                else if (suffix == "7701") {
                                    SDL_Log("%s is using WGI", name.c_str());
                                }
                                else if (suffix == "6700") {
                                    SDL_Log("%s is using GameInput", name.c_str());
                                }
                                else if (guidStr.find("xinput") != std::string::npos) {
                                    SDL_Log("%s is using XInput", name.c_str());
                                }
                            }

                        }

                        char* mapping = SDL_GetGamepadMapping(gamepad);
                        SDL_Log("Gamepad Mapping: \"%s\"", mapping);
                        SDL_free(mapping);

                        // Workaround for https://github.com/libsdl-org/SDL/issues/16261
                        SDL_GamepadType type = SDL_GetGamepadType(gamepad);
                        if (type == SDL_GAMEPAD_TYPE_PS4) {
                            int index = SDL_GetGamepadPlayerIndex(gamepad);
                            if (index == 0) {
                                SDL_SetGamepadLED(gamepad, 0, 0, 64); // Blue (First Player)
                            }
                        }
                    }
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    SDL_Log("SDL_EVENT_GAMEPAD_REMOVED");
                    SDL_JoystickID id = event.gdevice.which;
                    SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
                    if (gamepad != nullptr) {
                        SDL_Log("Closing gamepad: %s", SDL_GetGamepadName(gamepad));
                        SDL_CloseGamepad(gamepad);
                    }
                    break;
                }
                case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: {
                    SDL_Log("SDL_EVENT_JOYSTICK_BATTERY_UPDATED");
                    if (!Flags().ledbatterylvl) break;

                    SDL_GamepadType type = SDL_GetGamepadTypeForID(event.jbattery.which);
                    if (type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5) {

                        if (event.jbattery.state == SDL_POWERSTATE_ON_BATTERY) {

                            Uint8 red = 0, green = 0, blue = 0;

                            if (event.jbattery.percent <= 25) {
                                red = 255; green = 0; blue = 0;
                                SDL_Log("LED to red");
                            }
                            else if (event.jbattery.percent <= 50) {
                                red = 255; green = 165; blue = 0;
                                SDL_Log("LED to orange");
                            }
                            else if (event.jbattery.percent <= 75) {
                                red = 255; green = 255; blue = 0;
                                SDL_Log("LED to yellow");
                            }
                            else if (event.jbattery.percent <= 100) {
                                red = 0; green = 255; blue = 0;
                                SDL_Log("LED to green");
                            }

                            SDL_Gamepad* gamepad = SDL_GetGamepadFromID(event.jbattery.which);
                            if (gamepad != nullptr) {
                                SDL_SetGamepadLED(gamepad, red, green, blue);
                                SDL_Log("SDL_SetGamepadLED");
                            }

                        }
                    }
                    break;
                }
                case SDL_EVENT_QUIT: {
                    SDL_Log("SDL_EVENT_QUIT");
                    InputFusion_SDL_Event_Loop = false;
                    break;
                }
                }
            }
            SDL_Delay(10); // Prevent this loop from consuming too much CPU
        }
    }

    void InputFusion_SDL_Logger(void* userdata, int category, SDL_LogPriority priority, const char* message) {
        constexpr const char* fmt = "%s\n";
        SDL_IOStream* io = (SDL_IOStream*)userdata;
        if (io) {
            SDL_IOprintf(io, fmt, message);
            SDL_FlushIO(io);
        }
        printf(fmt, message);
    }
}

DWORD WINAPI InputFusion_Main(LPVOID lpReserved) {
    #ifdef _DEBUG
        util::EnableConsole();
        std::wstring file = util::GetModuleDirectory() + L"InputFusion.log";
        SDL_IOStream *logFile = SDL_IOFromFile(util::WStringToUtf8(file).c_str(), "w");
        SDL_SetLogOutputFunction(InputFusion_SDL_Logger, logFile);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
    #else
        SDL_SetLogPriorities(SDL_LOG_PRIORITY_ERROR);
    #endif

    InputFusion_SDL_SetHints();
    InputFusion_Detours_SetHooks();

    if (InputFusion_SDL_Init()) {
        InputFusion_SDL_EventLoop();
        InputFusion_SDL_CloseGamepads();
    }
    InputFusion_SDL_Quit();
    
    #ifdef _DEBUG
    if (logFile) {
        SDL_CloseIO(logFile);
    }
    #endif
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            InputFusion_SDL_Init_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            InputFusion_SDL_Quit_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            HANDLE hThread = CreateThread(nullptr, 0, InputFusion_Main, hModule, 0, nullptr);
            if (hThread) {
                CloseHandle(hThread);
            }
            break;
        }
    }
    return TRUE;
}