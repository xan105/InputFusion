/*
Copyright (c) Anthony Beaumont
This source code is licensed under the MIT License
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "detour.h"
#include "xinput.h"
#include "util.h"

extern std::atomic<bool> running;

SetWindowLong_t pSetWindowLong = nullptr;
WNDPROC oldWndProc = nullptr;

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_CLOSE || msg == WM_DESTROY) {
        running = false; //Exit SDL_eventLoop()
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

void setDetoursForWndProcEvent() {
    HMODULE hMod = LoadLibraryA("User32.dll");
    if (hMod == nullptr) return;

    std::cout << "LoadLibraryA: User32.dll" << std::endl;

    #if defined(_X86_)
    pSetWindowLong = (SetWindowLong_t)GetProcAddress(hMod, "SetWindowLongW");
    #elif defined(_AMD64_)
    pSetWindowLong = (SetWindowLong_t)GetProcAddress(hMod, "SetWindowLongPtrW");
    #endif

    if (pSetWindowLong != nullptr) DetourAttach(&(PVOID&)pSetWindowLong, Detour_SetWindowLong);
}

void setDetoursForXInput() {
    std::vector<std::string> versions = {
        "XInput1_4.dll",
        "xinput1_3.dll",
        "XInput9_1_0.dll"
    };

    for (const std::string& version : versions) {
        HMODULE hMod = LoadLibraryA(version.c_str());
        if (hMod == nullptr) continue;
        std::cout << "LoadLibraryA: " << version << std::endl;

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
}

bool setDetours() {

    bool GAMEPAD_SDL_EXIT = Getenv(L"GAMEPAD_SDL_EXIT") == L"HOOK";
    bool GAMEPAD_API_XINPUT = Getenv(L"GAMEPAD_API_XINPUT") == L"HOOK";

    if (!GAMEPAD_SDL_EXIT && !GAMEPAD_API_XINPUT) return false;

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if (GAMEPAD_SDL_EXIT) setDetoursForWndProcEvent();
    if (GAMEPAD_API_XINPUT) setDetoursForXInput();

    return DetourTransactionCommit() == NO_ERROR;
}