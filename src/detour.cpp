/*
Copyright (c) Anthony Beaumont
This source code is licensed under the MIT License
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "detour.h"
#include "xinput.h"
#include "dinput8.h"
#include "util.h"

extern std::atomic<bool> running;
extern HANDLE hSDL_Quit;
ExitProcess_t pExitProcess = nullptr;

void WINAPI Detour_ExitProcess(UINT uExitCode) {
    std::cout << "ExitProcess()" << std::endl;
    running = false; //Exit SDL_eventLoop()
    WaitForSingleObject(hSDL_Quit, 1000);
    CloseHandle(hSDL_Quit);
    return pExitProcess(uExitCode);
}

bool takeDetour(PVOID* ppPointer, PVOID pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(ppPointer, pDetour);
    return DetourTransactionCommit() == NO_ERROR;
}

bool setDetoursExitProcess() {
    HMODULE hMod = LoadLibraryA("KERNEL32.dll");
    if (hMod == nullptr) return false;

    std::cout << "LoadLibraryA: KERNEL32.dll" << std::endl;

    pExitProcess = (ExitProcess_t)GetProcAddress(hMod, "ExitProcess");
    if (pExitProcess == nullptr) return false;
    return takeDetour(&(PVOID&)pExitProcess, Detour_ExitProcess);
}

bool setDetoursForXInput() {
    std::vector<std::string> versions = {
        "XInput1_4.dll",
        "xinput1_3.dll",
        "XInput9_1_0.dll"
    };

    for (const std::string& version : versions) {
        HMODULE hMod = LoadLibraryA(version.c_str());
        if (hMod == nullptr) return false;
        std::cout << "LoadLibraryA: " << version << std::endl;

        XInputEnable_t pXInputEnable = (XInputEnable_t)GetProcAddress(hMod, "XInputEnable");
        if (pXInputEnable != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputEnable, XInputEnable)) return false;
        }

        XInputGetState_t pXInputGetState = (XInputGetState_t)GetProcAddress(hMod, "XInputGetState");
        if (pXInputGetState != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetState, XInputGetState)) return false;
        }

        XInputSetState_t pXInputSetState = (XInputSetState_t)GetProcAddress(hMod, "XInputSetState");
        if (pXInputSetState != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputSetState, XInputSetState)) return false;
        }

        XInputGetCapabilities_t pXInputGetCapabilities = (XInputGetCapabilities_t)GetProcAddress(hMod, "XInputGetCapabilities");
        if (pXInputGetCapabilities != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetCapabilities, XInputGetCapabilities)) return false;
        }

        XInputGetBatteryInformation_t pXInputGetBatteryInformation = (XInputGetBatteryInformation_t)GetProcAddress(hMod, "XInputGetBatteryInformation");
        if (pXInputGetBatteryInformation != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetBatteryInformation, XInputGetBatteryInformation)) return false;
        }

        XInputGetKeystroke_t pXInputGetKeystroke = (XInputGetKeystroke_t)GetProcAddress(hMod, "XInputGetKeystroke");
        if (pXInputGetKeystroke != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetKeystroke, XInputGetKeystroke)) return false;
        }

        XInputGetAudioDeviceIds_t pXInputGetAudioDeviceIds = (XInputGetAudioDeviceIds_t)GetProcAddress(hMod, "XInputGetAudioDeviceIds");
        if (pXInputGetAudioDeviceIds != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetAudioDeviceIds, XInputGetAudioDeviceIds)) return false;
        }

        XInputGetDSoundAudioDeviceGuids_t pXInputGetDSoundAudioDeviceGuids = (XInputGetDSoundAudioDeviceGuids_t)GetProcAddress(hMod, "XInputGetDSoundAudioDeviceGuids");
        if (pXInputGetDSoundAudioDeviceGuids != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetDSoundAudioDeviceGuids, XInputGetDSoundAudioDeviceGuids)) return false;
        }

        XInputGetStateEx_t pXInputGetStateEx = (XInputGetStateEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(100));
        if (pXInputGetStateEx != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetStateEx, XInputGetStateEx)) return false;
        }

        XInputWaitForGuideButton_t pXInputWaitForGuideButton = (XInputWaitForGuideButton_t)GetProcAddress(hMod, MAKEINTRESOURCEA(101));
        if (pXInputWaitForGuideButton != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputWaitForGuideButton, XInputWaitForGuideButton)) return false;
        }

        XInputCancelGuideButtonWait_t pXInputCancelGuideButtonWait = (XInputCancelGuideButtonWait_t)GetProcAddress(hMod, MAKEINTRESOURCEA(102));
        if (pXInputCancelGuideButtonWait != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputCancelGuideButtonWait, XInputCancelGuideButtonWait)) return false;
        }

        XInputPowerOffController_t pXInputPowerOffController = (XInputPowerOffController_t)GetProcAddress(hMod, MAKEINTRESOURCEA(103));
        if (pXInputPowerOffController != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputPowerOffController, XInputPowerOffController)) return false;
        }

        XInputGetBaseBusInformation_t pXInputGetBaseBusInformation = (XInputGetBaseBusInformation_t)GetProcAddress(hMod, MAKEINTRESOURCEA(104));
        if (pXInputGetBaseBusInformation != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetBaseBusInformation, XInputGetBaseBusInformation)) return false;
        }

        XInputGetCapabilitiesEx_t pXInputGetCapabilitiesEx = (XInputGetCapabilitiesEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(108));
        if (pXInputGetCapabilitiesEx != nullptr) {
            if (!takeDetour(&(PVOID&)pXInputGetCapabilitiesEx, XInputGetCapabilitiesEx)) return false;
        }
    }

    return true;
}

bool setDetoursForDInput8() {
    HMODULE hMod = LoadLibraryA("dinput8.dll");
    if (hMod == nullptr) return false;

    std::cout << "LoadLibraryA: dinput8.dll" << std::endl;

    DirectInput8Create_t pDirectInput8Create = (DirectInput8Create_t)GetProcAddress(hMod, "DirectInput8Create");
    if (pDirectInput8Create == nullptr) return false;
    return takeDetour(&(PVOID&)pDirectInput8Create, DirectInput8Create);
}

void setDetours() {
    if (Getenv(L"GAMEPAD_SDL_EXIT") == L"HOOK") {
        if (setDetoursExitProcess()) std::cout << "Detour set for exit handler" << std::endl;
    }

    if (Getenv(L"GAMEPAD_API_XINPUT") == L"HOOK") {
        if (setDetoursForXInput()) std::cout << "Detour set for XInput" << std::endl;
    }

    if (Getenv(L"GAMEPAD_API_DINPUT8") == L"HOOK") {
        if (setDetoursForDInput8()) std::cout << "Detour set for DInput8" << std::endl;
    }
}