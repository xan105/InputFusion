/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "xinput.h"

const std::unordered_map<SDL_GamepadButton, DWORD> BUTTONS = {
    {SDL_GAMEPAD_BUTTON_SOUTH, XINPUT_GAMEPAD_A},
    {SDL_GAMEPAD_BUTTON_EAST, XINPUT_GAMEPAD_B},
    {SDL_GAMEPAD_BUTTON_WEST, XINPUT_GAMEPAD_X},
    {SDL_GAMEPAD_BUTTON_NORTH, XINPUT_GAMEPAD_Y},
    {SDL_GAMEPAD_BUTTON_BACK, XINPUT_GAMEPAD_BACK},
    {SDL_GAMEPAD_BUTTON_START, XINPUT_GAMEPAD_START},
    {SDL_GAMEPAD_BUTTON_LEFT_STICK, XINPUT_GAMEPAD_LEFT_THUMB},
    {SDL_GAMEPAD_BUTTON_RIGHT_STICK, XINPUT_GAMEPAD_RIGHT_THUMB},
    {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, XINPUT_GAMEPAD_LEFT_SHOULDER},
    {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER},
    {SDL_GAMEPAD_BUTTON_DPAD_UP, XINPUT_GAMEPAD_DPAD_UP},
    {SDL_GAMEPAD_BUTTON_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_DOWN},
    {SDL_GAMEPAD_BUTTON_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_LEFT},
    {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT},
    {SDL_GAMEPAD_BUTTON_GUIDE, XINPUT_GAMEPAD_GUIDE},
    {SDL_GAMEPAD_BUTTON_TOUCHPAD, XINPUT_GAMEPAD_BACK}
};

const std::vector<SDL_GamepadAxis> AXIS = {
    SDL_GAMEPAD_AXIS_LEFTX,
    SDL_GAMEPAD_AXIS_LEFTY,
    SDL_GAMEPAD_AXIS_RIGHTX,
    SDL_GAMEPAD_AXIS_RIGHTY,
    SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
    SDL_GAMEPAD_AXIS_RIGHT_TRIGGER
};

DWORD dwPacketNumber = 0;

DWORD GetState(DWORD dwUserIndex, XINPUT_STATE* pState, bool extended = false) {

    if (dwUserIndex >= XUSER_MAX_COUNT || pState == nullptr)
        return ERROR_INVALID_PARAMETER;

    SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
    if (!(Flags & SDL_INIT_GAMEPAD)) {
        return ERROR_NOT_READY;
    }

    SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
    if (gamepad == nullptr) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    XINPUT_STATE state = {};
    state.dwPacketNumber = dwPacketNumber + 1;
    if (state.dwPacketNumber == 0xFFFFFFFF) state.dwPacketNumber = 0;

    SDL_UpdateGamepads();

    for (const auto& [sdl_button, xinput_button] : BUTTONS) {
        if (!extended && sdl_button == SDL_GAMEPAD_BUTTON_GUIDE)
            continue;
        if (SDL_GetGamepadButton(gamepad, sdl_button))
            state.Gamepad.wButtons |= xinput_button;
    }

    for (const auto& sdl_axis : AXIS) {

        int value = SDL_GetGamepadAxis(gamepad, sdl_axis);

        if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
            state.Gamepad.sThumbLX = value;
        }
        else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
            state.Gamepad.sThumbLY = std::clamp(-value, XINPUT_GAMEPAD_THUMB_MIN, XINPUT_GAMEPAD_THUMB_MAX); //XInput moving up is positive and SDL is negative
        }
        else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
            state.Gamepad.sThumbRX = value;
        }
        else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
            state.Gamepad.sThumbRY = std::clamp(-value, XINPUT_GAMEPAD_THUMB_MIN, XINPUT_GAMEPAD_THUMB_MAX); //XInput moving up is positive and SDL is negative
        }
        else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
            state.Gamepad.bLeftTrigger = (value * XINPUT_GAMEPAD_TRIGGER_MAX) / SDL_JOYSTICK_AXIS_MAX; // Triggers are 0-255 in XInput
        }
        else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
            state.Gamepad.bRightTrigger = (value * XINPUT_GAMEPAD_TRIGGER_MAX) / SDL_JOYSTICK_AXIS_MAX; // Triggers are 0-255 in XInput
        }
    }

    memcpy(pState, &state, sizeof(XINPUT_STATE));
    dwPacketNumber = state.dwPacketNumber;

    return ERROR_SUCCESS;
}

void GetCapabilities(SDL_Gamepad* gamepad, XINPUT_CAPABILITIES* pCapabilities, bool extended = false) {
    
    SDL_UpdateGamepads();

    pCapabilities->Type = XINPUT_DEVTYPE_GAMEPAD;

    SDL_GamepadType type = SDL_GetGamepadType(gamepad);
    if (type == SDL_GAMEPAD_TYPE_UNKNOWN)
        pCapabilities->SubType = XINPUT_DEVSUBTYPE_UNKNOWN;
    else
        pCapabilities->SubType = XINPUT_DEVSUBTYPE_GAMEPAD;

    if (type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
        pCapabilities->Flags |= XINPUT_CAPS_VOICE_SUPPORTED;
    }

    if (type == SDL_GAMEPAD_TYPE_XBOX360 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
        pCapabilities->Flags |= XINPUT_CAPS_PMD_SUPPORTED;
    }

    SDL_JoystickConnectionState connectionState = SDL_GetGamepadConnectionState(gamepad);
    if (connectionState == SDL_JOYSTICK_CONNECTION_WIRELESS)
        pCapabilities->Flags |= XINPUT_CAPS_WIRELESS;

    SDL_PropertiesID properties = SDL_GetGamepadProperties(gamepad);
    if (SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false)) {
        pCapabilities->Flags |= XINPUT_CAPS_FFB_SUPPORTED;
    }

    for (const auto& [sdl_button, xinput_button] : BUTTONS) {
        if (!extended && sdl_button == SDL_GAMEPAD_BUTTON_GUIDE)
            continue;

        if (SDL_GamepadHasButton(gamepad, sdl_button)) {
            pCapabilities->Gamepad.wButtons |= xinput_button;
        }
        else if (sdl_button == SDL_GAMEPAD_BUTTON_START ||
            sdl_button == SDL_GAMEPAD_BUTTON_BACK ||
            sdl_button == SDL_GAMEPAD_BUTTON_DPAD_UP ||
            sdl_button == SDL_GAMEPAD_BUTTON_DPAD_DOWN ||
            sdl_button == SDL_GAMEPAD_BUTTON_DPAD_LEFT ||
            sdl_button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT) {
            pCapabilities->Flags |= XINPUT_CAPS_NO_NAVIGATION;
        }
    }
}


#ifdef _XINPUT_EXPORTS
extern "C" {
#endif
    DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
       std::cout << "XInputGetState()" << std::endl;
        return GetState(dwUserIndex, pState, false);
    }

    DWORD WINAPI XInputSetState(DWORD dwUserIndex, const XINPUT_VIBRATION* pVibration) {

        std::cout << "XInputSetState()" << std::endl;

        if (dwUserIndex >= XUSER_MAX_COUNT || pVibration == nullptr)
            return ERROR_INVALID_PARAMETER;
        
        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return ERROR_NOT_READY;
        }
        
        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        SDL_RumbleGamepad(gamepad, pVibration->wLeftMotorSpeed, pVibration->wRightMotorSpeed, 0);
        SDL_UpdateGamepads();

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation) {

        std::cout << "XInputGetBatteryInformation()" << std::endl;

        if (dwUserIndex >= XUSER_MAX_COUNT || pBatteryInformation == nullptr)
            return ERROR_INVALID_PARAMETER;

        if (!(devType == BATTERY_DEVTYPE_GAMEPAD || devType == BATTERY_DEVTYPE_HEADSET))
            return ERROR_INVALID_PARAMETER;

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return ERROR_NOT_READY;
        }
        
        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        XINPUT_BATTERY_INFORMATION battery = {};

        if (devType == BATTERY_DEVTYPE_GAMEPAD) {
            int percent = -1;

            SDL_UpdateGamepads();
            SDL_PowerState powerState = SDL_GetGamepadPowerInfo(gamepad, &percent);

            switch (powerState) {
                  case SDL_POWERSTATE_ERROR: {
                      battery.BatteryType = BATTERY_TYPE_DISCONNECTED;
                      break;
                  }
                  case SDL_POWERSTATE_NO_BATTERY: {
                      battery.BatteryType = BATTERY_TYPE_WIRED;
                      break;
                  }
                  case SDL_POWERSTATE_ON_BATTERY:
                  case SDL_POWERSTATE_CHARGING:
                  case SDL_POWERSTATE_CHARGED: {
                      battery.BatteryType = BATTERY_TYPE_NIMH;

                      SDL_GamepadType type = SDL_GetGamepadType(gamepad);
                      if (type == SDL_GAMEPAD_TYPE_XBOX360 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
                          battery.BatteryType = BATTERY_TYPE_ALKALINE;
                      }
                      break;
                  }
                  case SDL_POWERSTATE_UNKNOWN: {
                      battery.BatteryType = BATTERY_TYPE_UNKNOWN;
                      break;
                  }
            }

            if (percent <= 7) battery.BatteryLevel = BATTERY_LEVEL_EMPTY;
            else if (percent > 7 && percent <= 38) battery.BatteryLevel = BATTERY_LEVEL_LOW;
            else if (percent > 38 && percent <= 69) battery.BatteryLevel = BATTERY_LEVEL_MEDIUM;
            else if (percent > 69 && percent <= 100) battery.BatteryLevel = BATTERY_LEVEL_FULL;
        }
        else {
            battery.BatteryType = BATTERY_TYPE_UNKNOWN;
        }

        memcpy(pBatteryInformation, &battery, sizeof(XINPUT_BATTERY_INFORMATION));

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) {

        std::cout << "XInputGetCapabilities()" << std::endl;

        if (dwUserIndex >= XUSER_MAX_COUNT || pCapabilities == nullptr)
            return ERROR_INVALID_PARAMETER;

        if (!(dwFlags == 0 || dwFlags == XINPUT_FLAG_GAMEPAD))
            return ERROR_INVALID_PARAMETER;

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return ERROR_NOT_READY;
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        XINPUT_CAPABILITIES capabilities = {};
        GetCapabilities(gamepad, &capabilities, false);
        memcpy(pCapabilities, &capabilities, sizeof(XINPUT_CAPABILITIES));

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke) {
        std::cout << "XInputGetKeystroke()" << std::endl;
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

//Hidden XInput functions
    DWORD WINAPI XInputGetStateEx(DWORD dwUserIndex, XINPUT_STATE* pState) {
        std::cout << "XInputGetStateEx()" << std::endl;
        return GetState(dwUserIndex, pState, true);
    }
    DWORD WINAPI XInputWaitForGuideButton(DWORD dwUserIndex, DWORD dwFlags, XINPUT_LISTEN_STATE* pState) {
        std::cout << "XInputWaitForGuideButton()" << std::endl;
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
    DWORD WINAPI XInputCancelGuideButtonWait(DWORD dwUserIndex) {
        std::cout << "XInputCancelGuideButtonWait()" << std::endl;
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
    DWORD WINAPI XInputPowerOffController(DWORD dwUserIndex) {
        std::cout << "XInputPowerOffController()" << std::endl;
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
    DWORD WINAPI XInputGetBaseBusInformation(DWORD dwUserIndex, XINPUT_BASE_BUS_INFORMATION* pInfo) {
        std::cout << "XInputGetBaseBusInformation()" << std::endl;
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
    DWORD WINAPI XInputGetCapabilitiesEx(DWORD a1, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilities) {

        std::cout << "XInputGetCapabilitiesEx()" << std::endl;
        
        if (dwUserIndex >= XUSER_MAX_COUNT || pCapabilities == nullptr)
            return ERROR_INVALID_PARAMETER;

        if (!(dwFlags == 0 || dwFlags == XINPUT_FLAG_GAMEPAD))
            return ERROR_INVALID_PARAMETER;

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return ERROR_NOT_READY;
        }
        
        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        XINPUT_CAPABILITIES_EX capabilitiesEx = {};

        GetCapabilities(gamepad, &capabilitiesEx.Capabilities, true);
        capabilitiesEx.VendorId = SDL_GetGamepadVendor(gamepad) | 0x045E;
        capabilitiesEx.ProductId = SDL_GetGamepadProduct(gamepad) | 0x028E;
        capabilitiesEx.ProductVersion = SDL_GetGamepadProductVersion(gamepad) | 0x114;

        memcpy(pCapabilities, &capabilitiesEx, sizeof(XINPUT_CAPABILITIES_EX));

        return ERROR_SUCCESS;
    }

//GDK (XInputOnGameInput)
    DWORD WINAPI XInputSetStateEx(DWORD dwUserIndex, const XINPUT_VIBRATION_EX* pVibration) {

        std::cout << "XInputSetStateEx()" << std::endl;
        
        if (dwUserIndex >= XUSER_MAX_COUNT || pVibration == nullptr)
            return ERROR_INVALID_PARAMETER;

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return ERROR_NOT_READY;
        }
        
        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        SDL_RumbleGamepad(gamepad, pVibration->wLeftMotorSpeed, pVibration->wRightMotorSpeed, 0);
        SDL_RumbleGamepadTriggers(gamepad, pVibration->wLeftTriggerSpeed, pVibration->wRightTriggerSpeed, 0);
        SDL_UpdateGamepads();

        return ERROR_SUCCESS;
    }

 //Deprecated
    void WINAPI XInputEnable(BOOL enable) {
        std::cout << "XInputEnable()" << std::endl;
        return;
    }

    DWORD WINAPI XInputGetAudioDeviceIds(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount) {
        std::cout << "XInputGetAudioDeviceIds()" << std::endl;
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid) {
        std::cout << "XInputGetDSoundAudioDeviceGuids()" << std::endl;
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
#ifdef _XINPUT_EXPORTS
}
#endif