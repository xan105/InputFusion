/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "dllmain.h"
#include "detour.h"
#include "util.h"

std::atomic<bool> running(true);
HANDLE hSDL_Quit = nullptr;

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
        std::cout << "Closing gamepad: " << SDL_GetGamepadName(gamepad) << std::endl;
        SDL_CloseGamepad(gamepad);   
    }
    SDL_free(gamepads);
    std::cout << "Closed gamepads"<< std::endl;
}

void SDL_eventLoop() {

    hSDL_Quit = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    
    //SDL Gamepad APIs
    SDL_SetHint(SDL_HINT_JOYSTICK_DIRECTINPUT, "0"); //Prevent SDL using our implementation of DInput (when hooking)
    SDL_SetHint(SDL_HINT_XINPUT_ENABLED, "0"); //Prevent SDL using our implementation of XInput (when hooking)
    SDL_SetHint(SDL_HINT_JOYSTICK_WGI, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "1");
    //SDL Gamepad APIs Tweaks
    SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "1"); //raw input pull data from WGI providing better support for Xbox controllers.
    SDL_SetHint(SDL_HINT_JOYSTICK_THREAD, "1"); //Xbox controllers raw input (SDL is not running in the main thread)
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS3, "1"); //Enable PS3 via its driver
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5, "1");
    //SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1"); //Breaks DInput for others app until controller reboot | enable later
    //SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1"); //Breaks DInput for others app until controller reboot | enable later
    //SDL_HINT_JOYSTICK_ENHANCED_REPORTS => will replace SDL_HINT_JOYSTICK_HIDAPI_PS*_RUMBLE (SDL3 next version)


    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        std::cout << "SDL_INIT_GAMEPAD > ERROR: " << SDL_GetError() << std::endl;
        SetEvent(hSDL_Quit);
        return;
    }
    std::cout << "SDL_INIT" << std::endl;

    bool LEDAsBatteryLvl = Getenv(L"GAMEPAD_LED") == L"BATTERYLVL";
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_GAMEPAD_ADDED: {
                    std::cout << "SDL_EVENT_GAMEPAD_ADDED" << std::endl;
                    SDL_JoystickID id = event.gdevice.which;
                    SDL_Gamepad* gamepad = SDL_OpenGamepad(id);
                    if (gamepad != nullptr) {
                        std::cout << "Open gamepad: " << SDL_GetGamepadName(gamepad) << std::endl;
                        char* mapping = SDL_GetGamepadMapping(gamepad);
                        std::cout << "Gamepad Mapping: \"" << mapping << "\"" << std::endl;
                        SDL_free(mapping);
                    }
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED: {
                    std::cout << "SDL_EVENT_GAMEPAD_REMOVED" << std::endl;
                    SDL_JoystickID id = event.gdevice.which;
                    SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
                    if (gamepad != nullptr) {
                        std::cout << "Closing gamepad: " << SDL_GetGamepadName(gamepad) << std::endl;
                        SDL_CloseGamepad(gamepad);
                    }
                    break;
                }
                case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: {

                    if (!LEDAsBatteryLvl) break;

                    std::cout << "SDL_EVENT_JOYSTICK_BATTERY_UPDATED" << std::endl;
                    
                    SDL_GamepadType type = SDL_GetGamepadTypeForID(event.jbattery.which);

                    if (type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5) {

                        if (event.jbattery.state == SDL_POWERSTATE_ON_BATTERY) {

                            Uint8 red = 0, green = 0, blue = 0;

                            if (event.jbattery.percent <= 25) {
                                red = 255; green = 0; blue = 0;
                                std::cout << "LED to red" << std::endl;
                            }
                            else if (event.jbattery.percent <= 50) {
                                red = 255; green = 165; blue = 0;
                                std::cout << "LED to orange" << std::endl;
                            }
                            else if (event.jbattery.percent <= 75) {
                                red = 255; green = 255; blue = 0;
                                std::cout << "LED to yellow" << std::endl;
                            }
                            else if (event.jbattery.percent <= 100) {
                                red = 0; green = 255; blue = 0;
                                std::cout << "LED to green" << std::endl;
                            }

                            SDL_Gamepad* gamepad = SDL_GetGamepadFromID(event.jbattery.which);
                            if (gamepad != nullptr) {
                                SDL_SetGamepadLED(gamepad, red, green, blue);
                                std::cout << "SDL_SetGamepadLED" << std::endl;
                            }

                        }
                    }
                    break;
                }
                case SDL_EVENT_QUIT: {
                    std::cout << "SDL_EVENT_QUIT" << std::endl;
                    running = false;
                    break;
                }
            }
        }
        SDL_Delay(10); // Prevent this loop from consuming too much CPU
    }

    closeGamepads();
    std::cout << "BYE BYE" << std::endl;
    SDL_Quit();
    SetEvent(hSDL_Quit);
}

DWORD WINAPI Main(LPVOID lpReserved) {
    #ifdef _DEBUG
        enableConsole();
    #endif

    setDetours();
    SDL_eventLoop();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            HANDLE hThread = CreateThread(nullptr, 0, Main, hModule, 0, nullptr);
            if (hThread) {
                CloseHandle(hThread);
            }
            break;
        }
    }
    return TRUE;
}