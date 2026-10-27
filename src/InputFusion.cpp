/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "InputFusion.h"
#include "flags.h"
#include "util.h"
#include "version.h"
#include "../vendor/SimpleINI/SimpleIni.h"

HANDLE InputFusion_SDL_Init_Wait = nullptr;
HANDLE InputFusion_SDL_Quit_Wait = nullptr;
std::atomic<bool> InputFusion_SDL_Event_Loop(true);

constexpr auto INPUTFUSION_SDL_EVENT_LOOP_DELAY = 10;

void InputFusion_SDL_SetHints() {

    CSimpleIniW ini;
    ini.SetUnicode();
    ini.SetQuotes(true);

    std::wstring file = util::GetModuleDirectory() + L"InputFusion.ini";

    struct {
        bool HIDAPI = true;         // HID
        bool GAMEINPUT = true;      // GameInput
        bool WGI = true;            // Windows.Gaming.Input (WinRT)
        bool XINPUT = true;         // Xinput 
        bool DINPUT8 = true;        // Dinput8
        bool RAWINPUT = false;      // RAW (Xbox controllers) | Disabled for now cf: https://github.com/libsdl-org/SDL/issues/13047#issuecomment-2913284199
        std::wstring CONTROLLER_MAPPING = L"";
        std::wstring CONTROLLER_MAPPING_FILE = L"";
    } SDL_Hints;

    if (ini.LoadFile(file.c_str()) == SI_OK) {
        SDL_Hints.HIDAPI = ini.GetBoolValue(L"SDL", L"HIDAPI", true);
        SDL_Hints.GAMEINPUT = ini.GetBoolValue(L"SDL", L"GAMEINPUT", true);
        SDL_Hints.WGI = ini.GetBoolValue(L"SDL", L"WGI", true);
        SDL_Hints.XINPUT = ini.GetBoolValue(L"SDL", L"XINPUT", true);
        SDL_Hints.DINPUT8 = ini.GetBoolValue(L"SDL", L"DINPUT8", true);
        SDL_Hints.RAWINPUT = ini.GetBoolValue(L"SDL", L"RAWINPUT", false);
        SDL_Hints.CONTROLLER_MAPPING = ini.GetValue(L"SDL", L"CONTROLLER_MAPPING", L"");
        SDL_Hints.CONTROLLER_MAPPING_FILE = ini.GetValue(L"SDL", L"CONTROLLER_MAPPING_FILE", L"");

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Parsed SDL Hints from [\"%s\"]", util::WStringToUtf8(file).c_str());
    } else {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to read SDL Hints from [\"%s\"]", util::WStringToUtf8(file).c_str());
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
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "InputFusion version %s", VER_FILEVERSION_STR);

    #if defined(GAMEINPUT_EXPORTS)
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "InputFusion is running as GameInput dll");
        if (SDL_GetHintBoolean(SDL_HINT_JOYSTICK_GAMEINPUT, false) && SDL_SetHintWithPriority(SDL_HINT_JOYSTICK_GAMEINPUT, "0", SDL_HINT_OVERRIDE)) {
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "-> Disabled GameInput within SDL to prevent conflict!");
        }
    #elif defined(XINPUT_EXPORTS)
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "InputFusion is running as XInput dll");
        if (SDL_GetHintBoolean(SDL_HINT_XINPUT_ENABLED, false) && SDL_SetHintWithPriority(SDL_HINT_XINPUT_ENABLED, "0", SDL_HINT_OVERRIDE)) {
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "-> Disabled XInput within SDL to prevent conflict!");
        }
    #elif defined(DINPUT8_EXPORTS)
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "InputFusion is running as DInput8 dll");
        if (SDL_GetHintBoolean(SDL_HINT_JOYSTICK_DIRECTINPUT, false) && SDL_SetHintWithPriority(SDL_HINT_JOYSTICK_DIRECTINPUT, "0", SDL_HINT_OVERRIDE)) {
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "-> Disabled DirectInput within SDL to prevent conflict!");
        }
    #elif defined(DINPUT_EXPORTS)
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "InputFusion is running as DInput dll");
    #elif defined(WINMM_EXPORTS)
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "InputFusion is running as WinMM dll");
    #endif

    if (SDL_GetHint(SDL_HINT_GAMECONTROLLERCONFIG) != nullptr) {
        SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Gamepad mapping override hint!");
    }
    if (SDL_GetHint(SDL_HINT_GAMECONTROLLERCONFIG_FILE) != nullptr) {
        SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Extra gamecontroller db entries hint!");
    }
        
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
    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "SDL_INIT_GAMEPAD");

    const int version = SDL_GetVersion();
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, 
        "SDL version %d.%d.%d",
        SDL_VERSIONNUM_MAJOR(version),
        SDL_VERSIONNUM_MINOR(version),
        SDL_VERSIONNUM_MICRO(version));

    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT,
        "SDL controller handling:\n"
        "- DirectInput: \t%s\n"
        "- XInput: \t%s\n"
        "- WGI: \t\t%s\n"
        "- GameInput: \t%s\n"
        "- HID: \t\t%s\n"
        "- RAWINPUT: \t%s",
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

void InputFusion_SDL_Quit(SDL_IOStream* logFile) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "SDL_Quit");
    if (logFile) {
        SDL_CloseIO(logFile);
    }
    SDL_Quit();
    if (InputFusion_SDL_Init_Wait) {
        CloseHandle(InputFusion_SDL_Init_Wait);
        InputFusion_SDL_Init_Wait = nullptr;
    }
    if (InputFusion_SDL_Quit_Wait) {
        SetEvent(InputFusion_SDL_Quit_Wait);
    }
}

void InputFusion_SDL_CloseGamepads() {
    int count = 0;
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
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Closing gamepad: %s", SDL_GetGamepadName(gamepad));
        SDL_CloseGamepad(gamepad);
    }
    SDL_free(gamepads);
    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Closed gamepads");
}

void InputFusion_SDL_EventLoop() {
    SDL_Event event;
    while (InputFusion_SDL_Event_Loop) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_GAMEPAD_ADDED: {
                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "SDL_EVENT_GAMEPAD_ADDED");
                SDL_JoystickID id = event.gdevice.which;
                SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
                if (gamepad != nullptr) {
                    std::string name = SDL_GetGamepadName(gamepad);
                    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Open gamepad: %s", name.c_str());

                    if (name.find("Xbox") != std::string::npos) { //https://github.com/libsdl-org/SDL/issues/12071#issuecomment-2682639565
                        SDL_GUID guid = SDL_GetGamepadGUIDForID(id);
                        char buffer[64];
                        SDL_GUIDToString(guid, buffer, sizeof(buffer));
                        std::string guidStr(buffer);
                        if (guidStr.size() >= 4) {
                            std::string suffix = guidStr.substr(guidStr.size() - 4);
                            if (suffix == "7200") {
                                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "%s is using RAW Input", name.c_str());
                            }
                            else if (suffix == "7701") {
                                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "%s is using WGI", name.c_str());
                            }
                            else if (suffix == "6700") {
                                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "%s is using GameInput", name.c_str());
                            }
                            else if (guidStr.find("xinput") != std::string::npos) {
                                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "%s is using XInput", name.c_str());
                            }
                        }

                    }

                    char* mapping = SDL_GetGamepadMapping(gamepad);
                    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Gamepad Mapping: \"%s\"", mapping);
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
                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "SDL_EVENT_GAMEPAD_REMOVED");
                SDL_JoystickID id = event.gdevice.which;
                SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
                if (gamepad != nullptr) {
                    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Closing gamepad: %s", SDL_GetGamepadName(gamepad));
                    SDL_CloseGamepad(gamepad);
                }
                break;
            }
            case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: {
                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "SDL_EVENT_JOYSTICK_BATTERY_UPDATED");
                if (!Flags().ledbatterylvl) break;

                SDL_GamepadType type = SDL_GetGamepadTypeForID(event.jbattery.which);
                if (type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5) {

                    if (event.jbattery.state == SDL_POWERSTATE_ON_BATTERY) {

                        Uint8 red = 0, green = 0, blue = 0;

                        if (event.jbattery.percent <= 25) {
                            red = 255; green = 0; blue = 0;
                            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LED to red");
                        }
                        else if (event.jbattery.percent <= 50) {
                            red = 255; green = 165; blue = 0;
                            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LED to orange");
                        }
                        else if (event.jbattery.percent <= 75) {
                            red = 255; green = 255; blue = 0;
                            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LED to yellow");
                        }
                        else if (event.jbattery.percent <= 100) {
                            red = 0; green = 255; blue = 0;
                            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LED to green");
                        }

                        SDL_Gamepad* gamepad = SDL_GetGamepadFromID(event.jbattery.which);
                        if (gamepad != nullptr) {
                            SDL_SetGamepadLED(gamepad, red, green, blue);
                            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "SDL_SetGamepadLED");
                        }

                    }
                }
                break;
            }
            case SDL_EVENT_QUIT: {
                SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "SDL_EVENT_QUIT");
                InputFusion_SDL_Event_Loop = false;
                break;
            }
            }
        }
        SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY); // Prevent this loop from consuming too much CPU
    }
}

void InputFusion_SDL_Wait_ready(DWORD timeout) {
    if (!(SDL_WasInit(SDL_INIT_GAMEPAD) & SDL_INIT_GAMEPAD)) {
        if (InputFusion_SDL_Init_Wait) {
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "SDL is not ready! WAITING...");
            WaitForSingleObject(InputFusion_SDL_Init_Wait, timeout);
            SDL_Delay(INPUTFUSION_SDL_EVENT_LOOP_DELAY);
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "...RESUMING!");
        }
    }
}