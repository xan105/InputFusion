/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "delegate.h"
#include "flags.h"

#if defined(INPUTFUSION_EXPORTS)
#include "GameInput/gameinput.h"
#include "XInput/xinput.h"
#include "DInput/dinput.h"
#include "WinMM/winmm.h"
#elif defined(GAMEINPUT_EXPORTS)
#include "GameInput/gameinput.h"
#elif defined(XINPUT_EXPORTS)
#include "XInput/xinput.h"
#elif defined(DINPUT8_EXPORTS)
#include "DInput8/dinput8.h"
#elif defined(DINPUT_EXPORTS)
#include "DInput/dinput.h"
#elif defined(WINMM_EXPORTS)
#include "WinMM/winmm.h"
#endif

HMODULE LoadSystemLibraryW(const wchar_t* moduleName)
{
    if (!moduleName || !*moduleName) {
      return nullptr;
    }

    wchar_t systemPath[MAX_PATH] = {};
    UINT length = GetSystemDirectoryW(systemPath, MAX_PATH);

    if (length == 0 || length >= MAX_PATH) {
      return nullptr;
    }

    std::wstring modulePath(systemPath, length);
    modulePath += L'\\';
    modulePath += moduleName;

    HMODULE hMod = LoadLibraryExW(
        modulePath.c_str(),          // Absolute path to load the DLL under its fully qualified module name,
                                     // rather than loading it by basename alone, this should result in a different
                                     // module identity/name representation in the process module list.
                                     // NB: Loading by basename alone would be sufficient in most circumstances, 
                                     // but certain edge cases across all supported configurations require this trick. 
                                     // This provides a more consistent behavior across all supported configurations.
        nullptr,
        LOAD_LIBRARY_SEARCH_SYSTEM32 // Restrict DLL dependency searches to the System32 directory
    );

    return hMod;
}

std::string GetModuleFileNameUTF8(HMODULE hMod) {
    WCHAR szPath[MAX_PATH];
    DWORD len = GetModuleFileNameW(hMod, szPath, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) { 
        return {};
    }

    int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, szPath, static_cast<int>(len), nullptr, 0, nullptr, nullptr);
    if (size == 0){ 
        return {};
    }

    std::string path(size, '\0');
    if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, szPath, static_cast<int>(len), path.data(), size, nullptr, nullptr) == 0) {
        return {};
    }
    return path;
}

/*  
    The following is an heuristic approach to try to determine if SDL3 is the caller.
    The idea is to determine whether a return address belongs to this module (DLL).
    This includes SDL3 which is statically linked into it.
*/

// Returns the HMODULE of the module containing this function.
static HMODULE GetSelfModule() {
    static HMODULE self = [] {
        HMODULE module = nullptr;
        GetModuleHandleExW(
            // Find the module containing the given address, without incrementing its reference count
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&GetSelfModule),
            &module);
        return module;
        }();
    return self;
}

// Returns true if the address belongs to this module OR if Windows cannot
// identify the module containing the address (default to fail-open).
// Since SDL3 is statically linked, we can make the assumption that the caller is SDL3.
bool IsSDL3(void* returnAddress) {
    HMODULE module = nullptr;
    if (!GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(returnAddress),
        &module) || !module) {
        return true;
    }
    return module == GetSelfModule();
}

#if defined(INPUTFUSION_EXPORTS) || defined(XINPUT_EXPORTS)

XInputEnable_t                      Win32_XInputEnable                      = nullptr;
XInputGetState_t                    Win32_XInputGetState                    = nullptr;
XInputSetState_t                    Win32_XInputSetState                    = nullptr;
XInputGetCapabilities_t             Win32_XInputGetCapabilities             = nullptr;
XInputGetBatteryInformation_t       Win32_XInputGetBatteryInformation       = nullptr;
XInputGetKeystroke_t                Win32_XInputGetKeystroke                = nullptr;
XInputGetAudioDeviceIds_t           Win32_XInputGetAudioDeviceIds           = nullptr;
XInputGetDSoundAudioDeviceGuids_t   Win32_XInputGetDSoundAudioDeviceGuids   = nullptr;
XInputGetStateEx_t                  Win32_XInputGetStateEx                  = nullptr;
XInputWaitForGuideButton_t          Win32_XInputWaitForGuideButton          = nullptr;
XInputCancelGuideButtonWait_t       Win32_XInputCancelGuideButtonWait       = nullptr;
XInputPowerOffController_t          Win32_XInputPowerOffController          = nullptr;
XInputGetBaseBusInformation_t       Win32_XInputGetBaseBusInformation       = nullptr;
XInputGetCapabilitiesEx_t           Win32_XInputGetCapabilitiesEx           = nullptr;

void delegateXInput()
{
    HMODULE hMod = LoadSystemLibraryW(L"XInput1_4.dll");
    if (hMod == nullptr) return;

    Win32_XInputEnable                      = (XInputEnable_t)GetProcAddress(hMod, "XInputEnable");
    Win32_XInputGetState                    = (XInputGetState_t)GetProcAddress(hMod, "XInputGetState");
    Win32_XInputSetState                    = (XInputSetState_t)GetProcAddress(hMod, "XInputSetState");
    Win32_XInputGetCapabilities             = (XInputGetCapabilities_t)GetProcAddress(hMod, "XInputGetCapabilities");
    Win32_XInputGetBatteryInformation       = (XInputGetBatteryInformation_t)GetProcAddress(hMod, "XInputGetBatteryInformation");
    Win32_XInputGetKeystroke                = (XInputGetKeystroke_t)GetProcAddress(hMod, "XInputGetKeystroke");
    Win32_XInputGetAudioDeviceIds           = (XInputGetAudioDeviceIds_t)GetProcAddress(hMod, "XInputGetAudioDeviceIds");
    Win32_XInputGetDSoundAudioDeviceGuids   = (XInputGetDSoundAudioDeviceGuids_t)GetProcAddress(hMod, "XInputGetDSoundAudioDeviceGuids");
    Win32_XInputGetStateEx                  = (XInputGetStateEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(100));
    Win32_XInputWaitForGuideButton          = (XInputWaitForGuideButton_t)GetProcAddress(hMod, MAKEINTRESOURCEA(101));
    Win32_XInputCancelGuideButtonWait       = (XInputCancelGuideButtonWait_t)GetProcAddress(hMod, MAKEINTRESOURCEA(102));
    Win32_XInputPowerOffController          = (XInputPowerOffController_t)GetProcAddress(hMod, MAKEINTRESOURCEA(103));
    Win32_XInputGetBaseBusInformation       = (XInputGetBaseBusInformation_t)GetProcAddress(hMod, MAKEINTRESOURCEA(104));
    Win32_XInputGetCapabilitiesEx           = (XInputGetCapabilitiesEx_t)GetProcAddress(hMod, MAKEINTRESOURCEA(108));
        
    std::string szPathUTF8 = GetModuleFileNameUTF8(hMod);
    SDL_Log("Delegating SDL3 XInput API calls to [\"%s\"]", szPathUTF8.c_str());
}
#endif

void delegateSDLInputCalls() {
    if (!Flags().delegate) {
      return;
    }
    SDL_Log("InputFusion is running in delegation mode!");
    
    #if defined(XINPUT_EXPORTS)
    delegateXInput();
    #endif 

    #if defined(INPUTFUSION_EXPORTS)
    if (Flags().xinput_detour) {
      delegateXInput();
    }
    #endif 
}