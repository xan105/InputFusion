/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

//cf: https://learn.microsoft.com/en-us/gaming/gdk/docs/reference/input/gameinput-v1/gameinput_members-v1

#include "GameInput.h"

#ifdef GAMEINPUT_EXPORTS
extern "C" {
#endif

HRESULT WINAPI GameInputCreate(IGameInput** gameInput) {
    SDL_Log("GameInputCreate()");
    static IGameInput singleton{};
    *gameInput = &singleton;
    return S_OK;
}

void WINAPI GameInputDeviceCallback(GameInputCallbackToken callbackToken, void* context, IGameInputDevice* device, uint64_t timestamp, GameInputDeviceStatus currentStatus, GameInputDeviceStatus previousStatus) {
    SDL_Log("GameInputDeviceCallback()");
    return;
}

void WINAPI GameInputKeyboardLayoutCallback(GameInputCallbackToken callbackToken, void * context, IGameInputDevice * device, uint64_t timestamp, uint32_t currentLayout, uint32_t previousLayout) {
    SDL_Log("GameInputKeyboardLayoutCallback()");
    return;
}

void WINAPI GameInputReadingCallback(GameInputCallbackToken callbackToken, void* context, IGameInputReading* reading) {
    SDL_Log("GameInputReadingCallback()");
    return;
}

void WINAPI GameInputSystemButtonCallback(GameInputCallbackToken callbackToken, void* context, IGameInputDevice* device, uint64_t timestamp, GameInputSystemButtons currentButtons, GameInputSystemButtons previousButtons) {
    SDL_Log("GameInputSystemButtonCallback()");
    return;
}

HRESULT WINAPI DllCanUnloadNow() {
    return S_OK;
}

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    return CLASS_E_CLASSNOTAVAILABLE;
}

#ifdef GAMEINPUT_EXPORTS
}
#endif