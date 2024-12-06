/*
Copyright (c) Anthony Beaumont
This source code is licensed under the MIT License
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "dllmain.h"
#include "xinput.h"
#include "util.h"

SetWindowLong_t pSetWindowLong = nullptr;
WNDPROC oldWndProc = nullptr;
std::atomic<bool> running(true);

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

    SDL_SetHint(SDL_HINT_JOYSTICK_THREAD, "1"); //Xbox controllers input (SDL is not running in the main thread)

    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        std::cout << "SDL_INIT_GAMEPAD > ERROR: " << SDL_GetError() << std::endl;
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
}

DWORD WINAPI Main(LPVOID lpReserved) {
    #ifdef _DEBUG
        enableConsole();
    #endif

     if (Getenv(L"GAMEPAD_SDL_EXIT") == L"HOOK") {
        if (setDetoursForWndProcEvent()) {
            std::cout << "Detour WndProcEvent" << std::endl;
        }
        else {
            std::cerr << "Detour WndProcEvent FAILED" << std::endl;
        }
     }
        
    if (Getenv(L"GAMEPAD_API_XINPUT") == L"HOOK") {
        if (setDetoursForXInput()) {
            std::cout << "Detour XInput" << std::endl;
        }
        else {
            std::cerr << "Detour XInput FAILED" << std::endl;
        }
    }

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

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (msg == WM_CLOSE || msg == WM_DESTROY) {
        running = false;
    }

    return CallWindowProcW(oldWndProc, hwnd, msg, wParam, lParam);
}

#if defined(_X86_)
    LONG WINAPI Detour_SetWindowLong(HWND hwnd, int nIndex, LONG dwNewLong) {
        if (nIndex == GWLP_WNDPROC) {
            oldWndProc = (WNDPROC)GetWindowLongW(hwnd, nIndex);
            return pSetWindowLong(hwnd, nIndex, (LONG)NewWndProc);
        }

        return pSetWindowLong(hwnd, nIndex, dwNewLong);
    }
#elif defined(_AMD64_)
    LONG_PTR WINAPI Detour_SetWindowLong(HWND hwnd, int nIndex, LONG_PTR dwNewLong) {
        if (nIndex == GWLP_WNDPROC) {
            oldWndProc = (WNDPROC)GetWindowLongPtrW(hwnd, nIndex);
            return pSetWindowLong(hwnd, nIndex, (LONG_PTR)NewWndProc);
        }

        return pSetWindowLong(hwnd, nIndex, dwNewLong);
    }
#endif


bool setDetoursForWndProcEvent() {
    HMODULE hMod = LoadLibraryA("User32.dll");
    if (hMod == nullptr) return false;

    std::cout << "LoadLibraryA: User32.dll" << std::endl;

    #if defined(_X86_)
        pSetWindowLong = (SetWindowLong_t)GetProcAddress(hMod, "SetWindowLongW");
    #elif defined(_AMD64_)
        pSetWindowLong = (SetWindowLong_t)GetProcAddress(hMod, "SetWindowLongPtrW");
    #endif

    if (pSetWindowLong == nullptr) return false;
        
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pSetWindowLong, Detour_SetWindowLong);
    if (DetourTransactionCommit() != NO_ERROR) return false;

    return true;
}

bool setDetoursForXInput() {

    std::vector<std::string> versions = {
        "XInput1_4.dll",
        "xinput1_3.dll",
        "xinput1_2.dll",
        "xinput1_1.dll",
        "XInput9_1_0.dll"
    };

    std::vector<HMODULE> xinput;

    for (const std::string& version : versions) {
        HMODULE hMod = LoadLibraryA(version.c_str());
        if (hMod != nullptr) {
            std::cout << "LoadLibraryA: " << version << std::endl;
            xinput.push_back(hMod);
        }
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    for (HMODULE hMod : xinput) {

        XInputEnable_t pXInputEnable = (XInputEnable_t)GetProcAddress(hMod, "XInputEnable");
        if (pXInputEnable != nullptr) DetourAttach(&(PVOID&)pXInputEnable, XInputEnable);

        XInputGetState_t pXInputGetState = (XInputGetState_t)GetProcAddress(hMod, "XInputGetState");
        if (pXInputGetState != nullptr) DetourAttach(&(PVOID&)pXInputGetState, XInputGetState);

        XInputSetState_t pXInputSetState = (XInputSetState_t)GetProcAddress(hMod, "XInputSetState");
        if (pXInputSetState != nullptr) DetourAttach(&(PVOID&)pXInputSetState, XInputSetState);

        XInputGetCapabilities_t pXInputGetCapabilities = (XInputGetCapabilities_t)GetProcAddress(hMod, "XInputGetCapabilities");
        if (pXInputGetCapabilities != nullptr) DetourAttach(&(PVOID&)pXInputGetCapabilities, XInputGetCapabilities);

        XInputGetBatteryInformation_t pXInputGetBatteryInformation = (XInputGetBatteryInformation_t)GetProcAddress(hMod, "XInputGetBatteryInformation");
        if (pXInputGetBatteryInformation != nullptr) DetourAttach(&(PVOID&)pXInputGetBatteryInformation, XInputGetBatteryInformation);

        XInputGetKeystroke_t pXInputGetKeystroke = (XInputGetKeystroke_t)GetProcAddress(hMod, "XInputGetKeystroke");
        if (pXInputGetKeystroke != nullptr) DetourAttach(&(PVOID&)pXInputGetKeystroke, XInputGetKeystroke);

        XInputGetAudioDeviceIds_t pXInputGetAudioDeviceIds = (XInputGetAudioDeviceIds_t)GetProcAddress(hMod, "XInputGetAudioDeviceIds");
        if (pXInputGetAudioDeviceIds != nullptr) DetourAttach(&(PVOID&)pXInputGetAudioDeviceIds, XInputGetAudioDeviceIds);

        XInputGetDSoundAudioDeviceGuids_t pXInputGetDSoundAudioDeviceGuids = (XInputGetDSoundAudioDeviceGuids_t)GetProcAddress(hMod, "XInputGetDSoundAudioDeviceGuids");
        if (pXInputGetDSoundAudioDeviceGuids != nullptr) DetourAttach(&(PVOID&)pXInputGetDSoundAudioDeviceGuids, XInputGetDSoundAudioDeviceGuids);

        XInputGetStateEx_t pXInputGetStateEx = (XInputGetStateEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(100));
        if (pXInputGetStateEx != nullptr) DetourAttach(&(PVOID&)pXInputGetStateEx, XInputGetStateEx);

        XInputWaitForGuideButton_t pXInputWaitForGuideButton = (XInputWaitForGuideButton_t)GetProcAddress(hMod, MAKEINTRESOURCEA(101));
        if (pXInputWaitForGuideButton != nullptr) DetourAttach(&(PVOID&)pXInputWaitForGuideButton, XInputWaitForGuideButton);

        XInputCancelGuideButtonWait_t pXInputCancelGuideButtonWait = (XInputCancelGuideButtonWait_t)GetProcAddress(hMod, MAKEINTRESOURCEA(102));
        if (pXInputCancelGuideButtonWait != nullptr) DetourAttach(&(PVOID&)pXInputCancelGuideButtonWait, XInputCancelGuideButtonWait);

        XInputPowerOffController_t pXInputPowerOffController = (XInputPowerOffController_t)GetProcAddress(hMod, MAKEINTRESOURCEA(103));
        if (pXInputPowerOffController != nullptr) DetourAttach(&(PVOID&)pXInputPowerOffController, XInputPowerOffController);

        XInputGetBaseBusInformation_t pXInputGetBaseBusInformation = (XInputGetBaseBusInformation_t)GetProcAddress(hMod, MAKEINTRESOURCEA(104));
        if (pXInputGetBaseBusInformation != nullptr) DetourAttach(&(PVOID&)pXInputGetBaseBusInformation, XInputGetBaseBusInformation);

        XInputGetCapabilitiesEx_t pXInputGetCapabilitiesEx = (XInputGetCapabilitiesEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(108));
        if (pXInputGetCapabilitiesEx != nullptr) DetourAttach(&(PVOID&)pXInputGetCapabilitiesEx, XInputGetCapabilitiesEx);
    }

    if (DetourTransactionCommit() != NO_ERROR) {
        return false;
    }

    return true;
}