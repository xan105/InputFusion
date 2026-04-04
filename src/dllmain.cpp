/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dllmain.h"
#include "detour.h"
#include "util.h"
#include "flags.h"
#include "version.h"

std::atomic<bool> SDL_Event_Loop(true);
HANDLE SDL_Init_Wait = nullptr;
HANDLE SDL_Quit_Wait = nullptr;

void setDefaultGamepadAPIs() {
    SDL_SetHint(SDL_HINT_JOYSTICK_DIRECTINPUT, "0");    // Dinput8
    SDL_SetHint(SDL_HINT_XINPUT_ENABLED, "0");          // Xinput      
    SDL_SetHint(SDL_HINT_JOYSTICK_WGI, "1");            // Windows.Gaming.Input (WinRT)
    SDL_SetHint(SDL_HINT_JOYSTICK_GAMEINPUT, "1");      // GameInput
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "1");         // HID
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS3, "1");                 // Enable PS3 via its driver
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_ENHANCED_REPORTS, "1");           //"1" => Breaks DInput for others app until controller reboot
    SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "0");       // RAW (Xbox controllers)
                                                        // 0 => Disabled for now cf: https://github.com/libsdl-org/SDL/issues/13047#issuecomment-2913284199
    SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "0");  // Raw input pulls data from WGI/XInput providing better support for Xbox controllers
                                                                    // 0 => Disable for now cf: https://github.com/xan105/InputFusion/issues/12
    SDL_SetHint(SDL_HINT_JOYSTICK_THREAD, "1");                     // Xbox controllers raw input (SDL is not running in the main thread)

    // https://github.com/libsdl-org/SDL/pull/14353
    // Should be merged in SDL3 3.6
    // SDL_SetHint(SDL_HINT_JOYSTICK_DSU, "1");         // DualShock UDP (DS4Windows, BetterJoy, etc.)
}

bool init() {
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

    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        SDL_LogError(SDL_LOG_CATEGORY_INPUT, "SDL_INIT_GAMEPAD > ERROR: %s", SDL_GetError());
        if (SDL_Init_Wait) SetEvent(SDL_Init_Wait);
        return false;
    }
    SDL_Log("SDL_Init");

    const int version = SDL_GetVersion();
    SDL_Log("SDL version %d.%d.%d", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
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

    if (SDL_Init_Wait) SetEvent(SDL_Init_Wait);
    return true;
}

void quit() {
    SDL_Log("SDL_Quit");
    SDL_Quit();
    if (SDL_Quit_Wait) SetEvent(SDL_Quit_Wait);
    if (SDL_Init_Wait) {
        CloseHandle(SDL_Init_Wait);
        SDL_Init_Wait = nullptr;
    }
}

void closeGamepads() {
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

void eventLoop() {
    SDL_Event event;
    while (SDL_Event_Loop) {
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
                    SDL_Event_Loop = false;
                    break;
                }
            }
        }
        SDL_Delay(10); // Prevent this loop from consuming too much CPU
    }
}

void Logger(void *userdata, int category, SDL_LogPriority priority, const char *message) {
    constexpr const char* fmt = "%s\n";
    SDL_IOStream *io = (SDL_IOStream *)userdata;
    if (io) {
      SDL_IOprintf(io, fmt, message);
      SDL_FlushIO(io);
    }
    printf(fmt, message);
}

DWORD WINAPI Main(LPVOID lpReserved) {
    #ifdef _DEBUG
    enableConsole();
    SDL_IOStream *logFile = SDL_IOFromFile("InputFusion.log", "w");
    SDL_SetLogOutputFunction(Logger, logFile);
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
    #else
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_ERROR);
    #endif

    setDefaultGamepadAPIs();
    setDetours();
    if (init()) {
        eventLoop();
        closeGamepads();
    }
    quit();
    
    #ifdef _DEBUG
    if (logFile) SDL_CloseIO(logFile);
    #endif
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            SDL_Init_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            SDL_Quit_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            HANDLE hThread = CreateThread(nullptr, 0, Main, hModule, 0, nullptr);
            if (hThread) {
                CloseHandle(hThread);
            }
            break;
        }
    }
    return TRUE;
}