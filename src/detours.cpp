/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "detours.h"
#include <vector>
#include "InputFusion.h"
#include "util.h"
#include "flags.h"
#include "GameInput/gameinput.h"
#include "XInput/xinput.h"
#include "DInput8/dinput8.h"
#include "DInput/dinput.h"
#include "WinMM/winmm.h"

#include "../vendor/Detour/detours.h"
#if defined(_X86_)
    #pragma comment(lib, "../vendor/Detour/detours.x86.lib")
#elif defined(_AMD64_)
    #pragma comment(lib, "../vendor/Detour/detours.x64.lib")
#endif

namespace {

    ExitProcess_t Win32_ExitProcess = nullptr;

    void WINAPI InputFusion_ExitProcess(UINT uExitCode) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "ExitProcess(%u)", uExitCode);

        InputFusion_SDL_Event_Loop = false;
        if (InputFusion_SDL_Quit_Wait) {
            WaitForSingleObject(InputFusion_SDL_Quit_Wait, 1000);
            CloseHandle(InputFusion_SDL_Quit_Wait);
            InputFusion_SDL_Quit_Wait = nullptr;
        }
        return Win32_ExitProcess(uExitCode);
    }
    
    bool InputFusion_Detours_Attach(PVOID* ppPointer, PVOID pDetour) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(ppPointer, pDetour);
        return DetourTransactionCommit() == NO_ERROR;
    }

    bool InputFusion_Detours_Hook_ExitProcess() {
        HMODULE hMod = LoadLibraryW(L"KERNEL32.dll");
        if (hMod == nullptr) {
            return false;
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "LoadLibraryW: KERNEL32.dll");

        Win32_ExitProcess = (ExitProcess_t)GetProcAddress(hMod, "ExitProcess");
        if (Win32_ExitProcess == nullptr) {
            return false;
        }

        return InputFusion_Detours_Attach(&(PVOID&)Win32_ExitProcess, InputFusion_ExitProcess);
    }

    bool InputFusion_Detours_Hook_GameInput() {
        HMODULE hMod = LoadLibraryW(L"gameinput.dll"); // GameInputRedist.dll (?)
        if (hMod == nullptr) {
            return false;
        }
        std::wstring szPath = util::GetModuleFileNameWString(hMod);
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LoadLibraryW: gameinput.dll [\"%s\"]", util::WStringToUtf8(szPath).c_str());

        GameInputCreate_t Win32_GameInputCreate = (GameInputCreate_t)GetProcAddress(hMod, "GameInputCreate");
        if (Win32_GameInputCreate == nullptr) {
            return false;
        }

        return InputFusion_Detours_Attach(&(PVOID&)Win32_GameInputCreate, GameInputCreate);
    }

    bool InputFusion_Detours_Hook_XInput() {
        std::vector<std::wstring> versions = {
            L"XInput1_4.dll",
            L"xinput1_3.dll",
            L"XInput9_1_0.dll"
        };

        for (const std::wstring& version : versions) {
            HMODULE hMod = LoadLibraryW(version.c_str());
            if (hMod == nullptr) {
                return false;
            }
            std::wstring szPath = util::GetModuleFileNameWString(hMod);
            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LoadLibraryW: %s [\"%s\"]", util::WStringToUtf8(version).c_str(), util::WStringToUtf8(szPath).c_str());

            XInputEnable_t Win32_XInputEnable = (XInputEnable_t)GetProcAddress(hMod, "XInputEnable");
            if (Win32_XInputEnable != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputEnable, XInputEnable))
            {
                return false;
            }

            XInputGetState_t Win32_XInputGetState = (XInputGetState_t)GetProcAddress(hMod, "XInputGetState");
            if (Win32_XInputGetState != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetState, XInputGetState))
            {
                return false;
            }

            XInputSetState_t Win32_XInputSetState = (XInputSetState_t)GetProcAddress(hMod, "XInputSetState");
            if (Win32_XInputSetState != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputSetState, XInputSetState))
            {
                return false;
            }

            XInputGetCapabilities_t Win32_XInputGetCapabilities = (XInputGetCapabilities_t)GetProcAddress(hMod, "XInputGetCapabilities");
            if (Win32_XInputGetCapabilities != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetCapabilities, XInputGetCapabilities))
            {
                return false;
            }

            XInputGetBatteryInformation_t Win32_XInputGetBatteryInformation = (XInputGetBatteryInformation_t)GetProcAddress(hMod, "XInputGetBatteryInformation");
            if (Win32_XInputGetBatteryInformation != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetBatteryInformation, XInputGetBatteryInformation))
            {
                return false;
            }

            XInputGetKeystroke_t Win32_XInputGetKeystroke = (XInputGetKeystroke_t)GetProcAddress(hMod, "XInputGetKeystroke");
            if (Win32_XInputGetKeystroke != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetKeystroke, XInputGetKeystroke))
            {
                return false;
            }

            XInputGetAudioDeviceIds_t Win32_XInputGetAudioDeviceIds = (XInputGetAudioDeviceIds_t)GetProcAddress(hMod, "XInputGetAudioDeviceIds");
            if (Win32_XInputGetAudioDeviceIds != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetAudioDeviceIds, XInputGetAudioDeviceIds))
            {
                return false;
            }

            XInputGetDSoundAudioDeviceGuids_t Win32_XInputGetDSoundAudioDeviceGuids = (XInputGetDSoundAudioDeviceGuids_t)GetProcAddress(hMod, "XInputGetDSoundAudioDeviceGuids");
            if (Win32_XInputGetDSoundAudioDeviceGuids != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetDSoundAudioDeviceGuids, XInputGetDSoundAudioDeviceGuids))
            {
                return false;
            }

            XInputGetStateEx_t Win32_XInputGetStateEx = (XInputGetStateEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(100));
            if (Win32_XInputGetStateEx != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetStateEx, XInputGetStateEx))
            {
                return false;
            }

            XInputWaitForGuideButton_t Win32_XInputWaitForGuideButton = (XInputWaitForGuideButton_t)GetProcAddress(hMod, MAKEINTRESOURCEA(101));
            if (Win32_XInputWaitForGuideButton != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputWaitForGuideButton, XInputWaitForGuideButton))
            {
                return false;
            }

            XInputCancelGuideButtonWait_t Win32_XInputCancelGuideButtonWait = (XInputCancelGuideButtonWait_t)GetProcAddress(hMod, MAKEINTRESOURCEA(102));
            if (Win32_XInputCancelGuideButtonWait != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputCancelGuideButtonWait, XInputCancelGuideButtonWait))
            {
                return false;
            }

            XInputPowerOffController_t Win32_XInputPowerOffController = (XInputPowerOffController_t)GetProcAddress(hMod, MAKEINTRESOURCEA(103));
            if (Win32_XInputPowerOffController != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputPowerOffController, XInputPowerOffController))
            {
                return false;
            }

            XInputGetBaseBusInformation_t Win32_XInputGetBaseBusInformation = (XInputGetBaseBusInformation_t)GetProcAddress(hMod, MAKEINTRESOURCEA(104));
            if (Win32_XInputGetBaseBusInformation != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetBaseBusInformation, XInputGetBaseBusInformation))
            {
                return false;
            }

            XInputGetCapabilitiesEx_t Win32_XInputGetCapabilitiesEx = (XInputGetCapabilitiesEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(108));
            if (Win32_XInputGetCapabilitiesEx != nullptr &&
                !InputFusion_Detours_Attach(&(PVOID&)Win32_XInputGetCapabilitiesEx, XInputGetCapabilitiesEx))
            {
                return false;
            }
        }

        return true;
    }

    bool InputFusion_Detours_Hook_DInput8() {
        HMODULE hMod = LoadLibraryW(L"dinput8.dll");
        if (hMod == nullptr) {
            return false;
        }
        std::wstring szPath = util::GetModuleFileNameWString(hMod);
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LoadLibraryW: dinput8.dll [\"%s\"]", util::WStringToUtf8(szPath).c_str());

        DirectInput8Create_t Win32_DirectInput8Create = (DirectInput8Create_t)GetProcAddress(hMod, "DirectInput8Create");
        if (Win32_DirectInput8Create == nullptr) {
            return false;
        }

        return InputFusion_Detours_Attach(&(PVOID&)Win32_DirectInput8Create, DirectInput8Create);
    }

    bool InputFusion_Detours_Hook_DInput() {
        HMODULE hMod = LoadLibraryW(L"dinput.dll");
        if (hMod == nullptr) {
            return false;
        }
        std::wstring szPath = util::GetModuleFileNameWString(hMod);
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LoadLibraryW: dinput.dll [\"%s\"]", util::WStringToUtf8(szPath).c_str());

        DirectInputCreateA_t Win32_DirectInputCreateA = (DirectInputCreateA_t)GetProcAddress(hMod, "DirectInputCreateA");
        if (Win32_DirectInputCreateA != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_DirectInputCreateA, DirectInputCreateA))
        {
            return false;
        }

        DirectInputCreateW_t Win32_DirectInputCreateW = (DirectInputCreateW_t)GetProcAddress(hMod, "DirectInputCreateW");
        if (Win32_DirectInputCreateW != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_DirectInputCreateW, DirectInputCreateW))
        {
            return false;
        }

        DirectInputCreateEx_t Win32_DirectInputCreateEx = (DirectInputCreateEx_t)GetProcAddress(hMod, "DirectInputCreateEx");
        if (Win32_DirectInputCreateEx != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_DirectInputCreateEx, DirectInputCreateEx))
        {
            return false;
        }

        return true;
    }

    bool InputFusion_Detours_Hook_Winmm() {
        HMODULE hMod = LoadLibraryW(L"winmm.dll");
        if (hMod == nullptr) {
            return false;
        }
        std::wstring szPath = util::GetModuleFileNameWString(hMod);
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "LoadLibraryW: winmm.dll [\"%s\"]", util::WStringToUtf8(szPath).c_str());

        joyConfigChanged_t Win32_joyConfigChanged = (joyConfigChanged_t)GetProcAddress(hMod, "joyConfigChanged");
        if (Win32_joyConfigChanged != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyConfigChanged, joyConfigChanged))
        {
            return false;
        }

        joyGetDevCapsA_t Win32_joyGetDevCapsA = (joyGetDevCapsA_t)GetProcAddress(hMod, "joyGetDevCapsA");
        if (Win32_joyGetDevCapsA != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyGetDevCapsA, joyGetDevCapsA))
        {
            return false;
        }

        joyGetDevCapsW_t Win32_joyGetDevCapsW = (joyGetDevCapsW_t)GetProcAddress(hMod, "joyGetDevCapsW");
        if (Win32_joyGetDevCapsW != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyGetDevCapsW, joyGetDevCapsW))
        {
            return false;
        }

        joyGetNumDevs_t Win32_joyGetNumDevs = (joyGetNumDevs_t)GetProcAddress(hMod, "joyGetNumDevs");
        if (Win32_joyGetNumDevs != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyGetNumDevs, joyGetNumDevs))
        {
            return false;
        }

        joyGetPos_t Win32_joyGetPos = (joyGetPos_t)GetProcAddress(hMod, "joyGetPos");
        if (Win32_joyGetPos != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyGetPos, joyGetPos))
        {
            return false;
        }

        joyGetPosEx_t Win32_joyGetPosEx = (joyGetPosEx_t)GetProcAddress(hMod, "joyGetPosEx");
        if (Win32_joyGetPosEx != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyGetPosEx, joyGetPosEx))
        {
            return false;
        }

        joyGetThreshold_t Win32_joyGetThreshold = (joyGetThreshold_t)GetProcAddress(hMod, "joyGetThreshold");
        if (Win32_joyGetThreshold != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyGetThreshold, joyGetThreshold))
        {
            return false;
        }

        joyReleaseCapture_t Win32_joyReleaseCapture = (joyReleaseCapture_t)GetProcAddress(hMod, "joyReleaseCapture");
        if (Win32_joyReleaseCapture != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joyReleaseCapture, joyReleaseCapture))
        {
            return false;
        }

        joySetCapture_t Win32_joySetCapture = (joySetCapture_t)GetProcAddress(hMod, "joySetCapture");
        if (Win32_joySetCapture != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joySetCapture, joySetCapture))
        {
            return false;
        }

        joySetThreshold_t Win32_joySetThreshold = (joySetThreshold_t)GetProcAddress(hMod, "joySetThreshold");
        if (Win32_joySetThreshold != nullptr &&
            !InputFusion_Detours_Attach(&(PVOID&)Win32_joySetThreshold, joySetThreshold))
        {
            return false;
        }

        return true;
    }
}

void InputFusion_Detours_SetHooks() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Process is \"%s\"", util::WStringToUtf8(util::GetCurrentProcessName()).c_str());

    if (InputFusion_Detours_Hook_ExitProcess()) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Set Detour for Exit handler");
    }
    
    if (Flags().gameinput_detour && InputFusion_Detours_Hook_GameInput()) {
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Set Detour for GameInput");
        if (SDL_GetHintBoolean(SDL_HINT_JOYSTICK_GAMEINPUT, false) && SDL_SetHintWithPriority(SDL_HINT_JOYSTICK_GAMEINPUT, "0", SDL_HINT_OVERRIDE)) {
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "-> Disabled GameInput within SDL to prevent conflict!");
        }
    }
    
    if (Flags().xinput_detour && InputFusion_Detours_Hook_XInput()) {
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Set Detour for XInput");
        if (SDL_GetHintBoolean(SDL_HINT_XINPUT_ENABLED, false) && SDL_SetHintWithPriority(SDL_HINT_XINPUT_ENABLED, "0", SDL_HINT_OVERRIDE)) {
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "-> Disabled XInput within SDL to prevent conflict!");
        }
    }

    if (Flags().dinput8_detour && InputFusion_Detours_Hook_DInput8()) {
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Set Detour for DInput8");
        if (SDL_GetHintBoolean(SDL_HINT_JOYSTICK_DIRECTINPUT, false) && SDL_SetHintWithPriority(SDL_HINT_JOYSTICK_DIRECTINPUT, "0", SDL_HINT_OVERRIDE)) {
            SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "-> Disabled DirectInput within SDL to prevent conflict!");
        }
    }

    if (Flags().dinput_detour && InputFusion_Detours_Hook_DInput()) {
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Set Detour for DInput 1-7");
    }
    
    if (Flags().winmm_detour && InputFusion_Detours_Hook_Winmm()) {
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Set Detour for WinMM");
    }
}