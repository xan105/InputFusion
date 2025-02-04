/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "detour.h"

extern std::atomic<bool> running;
extern HANDLE hSDL_Quit;
ExitProcess_t pExitProcess = nullptr;

void WINAPI Detour_ExitProcess(UINT uExitCode) {
    SDL_Log("ExitProcess(%u)", uExitCode);

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
    SDL_Log("LoadLibraryA: KERNEL32.dll");

    pExitProcess = (ExitProcess_t)GetProcAddress(hMod, "ExitProcess");
    if (pExitProcess == nullptr) return false;
    return takeDetour(&(PVOID&)pExitProcess, Detour_ExitProcess);
}

#if defined(INPUTFUSION_EXPORTS) || defined(XINPUT_EXPORTS)
bool setDetoursForXInput() {
    std::vector<std::string> versions = {
        "XInput1_4.dll",
        "xinput1_3.dll",
        "XInput9_1_0.dll"
    };

    for (const std::string& version : versions) {
        HMODULE hMod = LoadLibraryA(version.c_str());
        if (hMod == nullptr) return false;
        SDL_Log("LoadLibraryA: %s", version.c_str());

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
#endif

#if defined(INPUTFUSION_EXPORTS) || defined(DINPUT8_EXPORTS) || defined(XINPUT_EXPORTS)
bool setDetoursForDInput8() {
    HMODULE hMod = LoadLibraryA("dinput8.dll");
    if (hMod == nullptr) return false;
    SDL_Log("LoadLibraryA: dinput8.dll");

    DirectInput8Create_t pDirectInput8Create = (DirectInput8Create_t)GetProcAddress(hMod, "DirectInput8Create");
    if (pDirectInput8Create == nullptr) return false;
    return takeDetour(&(PVOID&)pDirectInput8Create, DirectInput8Create);
}
#endif

#if defined(INPUTFUSION_EXPORTS) || defined(WINMM_EXPORTS)
bool setDetoursForWinmm() {
    HMODULE hMod = LoadLibraryA("winmm.dll");
    if (hMod == nullptr) return false;
    SDL_Log("LoadLibraryA: winmm.dll");

    joyConfigChanged_t pjoyConfigChanged = (joyConfigChanged_t)GetProcAddress(hMod, "joyConfigChanged");
    if (pjoyConfigChanged == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyConfigChanged, joyConfigChanged)) return false;
    
    joyGetDevCapsA_t pjoyGetDevCapsA = (joyGetDevCapsA_t)GetProcAddress(hMod, "joyGetDevCapsA");
    if (pjoyGetDevCapsA == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyGetDevCapsA, joyGetDevCapsA)) return false;
    
    joyGetDevCapsW_t pjoyGetDevCapsW = (joyGetDevCapsW_t)GetProcAddress(hMod, "joyGetDevCapsW");
    if (pjoyGetDevCapsW == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyGetDevCapsW, joyGetDevCapsW)) return false;
    
    joyGetNumDevs_t pjoyGetNumDevs = (joyGetNumDevs_t)GetProcAddress(hMod, "joyGetNumDevs");
    if (pjoyGetNumDevs == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyGetNumDevs, joyGetNumDevs)) return false;
    
    joyGetPos_t pjoyGetPos = (joyGetPos_t)GetProcAddress(hMod, "joyGetPos");
    if (pjoyGetPos == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyGetPos, joyGetPos)) return false;
    
    joyGetPosEx_t pjoyGetPosEx = (joyGetPosEx_t)GetProcAddress(hMod, "joyGetPosEx");
    if (pjoyGetPosEx == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyGetPosEx, joyGetPosEx)) return false;
    
    joyGetThreshold_t pjoyGetThreshold = (joyGetThreshold_t)GetProcAddress(hMod, "joyGetThreshold");
    if (pjoyGetThreshold == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyGetThreshold, joyGetThreshold)) return false;
    
    joyReleaseCapture_t pjoyReleaseCapture = (joyReleaseCapture_t)GetProcAddress(hMod, "joyReleaseCapture");
    if (pjoyReleaseCapture == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoyReleaseCapture, joyReleaseCapture)) return false;
    
    joySetCapture_t pjoySetCapture = (joySetCapture_t)GetProcAddress(hMod, "joySetCapture");
    if (pjoySetCapture == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoySetCapture, joySetCapture)) return false;
    
    joySetThreshold_t pjoySetThreshold = (joySetThreshold_t)GetProcAddress(hMod, "joySetThreshold");
    if (pjoySetThreshold == nullptr) return false;
    if (!takeDetour(&(PVOID&)pjoySetThreshold, joySetThreshold)) return false;
    
    return true;
}
#endif

void setDetours() {
    
    if (setDetoursExitProcess()) SDL_Log("Detour set for exit handler");
    
    #if defined(INPUTFUSION_EXPORTS) || defined(XINPUT_EXPORTS)
    if (Getenv(L"GAMEPAD_API_XINPUT") == L"HOOK") {
        if (setDetoursForXInput()) SDL_Log("Detour set for XInput");
    }
    #endif

    #if defined(INPUTFUSION_EXPORTS) || defined(DINPUT8_EXPORTS) || defined(XINPUT_EXPORTS)
    if (Getenv(L"GAMEPAD_API_DINPUT8") == L"HOOK") {
        if (setDetoursForDInput8()) SDL_Log("Detour set for DInput8");
    }
    #endif
    
    #if defined(INPUTFUSION_EXPORTS) || defined(WINMM_EXPORTS)
    if (Getenv(L"GAMEPAD_API_WINMM") == L"HOOK") {
        if (setDetoursForWinmm()) SDL_Log("Detour set for WinMM");
    }
    #endif
}