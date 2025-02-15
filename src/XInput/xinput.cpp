/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

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

#ifdef XINPUT_EXPORTS
extern "C" {
#endif
    DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
       SDL_Log("XInputGetState(%u, %p)", dwUserIndex, pState);

       DWORD result = XInputGetStateEx(dwUserIndex, pState);
       if (result != ERROR_SUCCESS) return result;

       //Remove GUIDE button
       pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_GUIDE;

       return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputGetStateEx(DWORD dwUserIndex, XINPUT_STATE* pState) {
        SDL_Log("XInputGetStateEx(%u, %p)", dwUserIndex, pState);

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

        SDL_UpdateGamepads();

        ZeroMemory(pState, sizeof(XINPUT_STATE));
        pState->dwPacketNumber = dwPacketNumber + 1;
        if (pState->dwPacketNumber == 0xFFFFFFFF) pState->dwPacketNumber = 0;

        for (const auto& [sdl_button, xinput_button] : BUTTONS) {
            if (SDL_GetGamepadButton(gamepad, sdl_button))
                pState->Gamepad.wButtons |= xinput_button;
        }

        for (const auto& sdl_axis : AXIS) {
            int value = SDL_GetGamepadAxis(gamepad, sdl_axis);

            if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
                pState->Gamepad.sThumbLX = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
                pState->Gamepad.sThumbLY = std::clamp(-value, XINPUT_GAMEPAD_THUMB_MIN, XINPUT_GAMEPAD_THUMB_MAX); //XInput moving up is positive and SDL is negative
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
                pState->Gamepad.sThumbRX = value;
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
                pState->Gamepad.sThumbRY = std::clamp(-value, XINPUT_GAMEPAD_THUMB_MIN, XINPUT_GAMEPAD_THUMB_MAX); //XInput moving up is positive and SDL is negative
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
                pState->Gamepad.bLeftTrigger = (value * XINPUT_GAMEPAD_TRIGGER_MAX) / SDL_JOYSTICK_AXIS_MAX; // Triggers are 0-255 in XInput
            }
            else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
                pState->Gamepad.bRightTrigger = (value * XINPUT_GAMEPAD_TRIGGER_MAX) / SDL_JOYSTICK_AXIS_MAX; // Triggers are 0-255 in XInput
            }
        }

        dwPacketNumber = pState->dwPacketNumber;
        return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputSetState(DWORD dwUserIndex, const XINPUT_VIBRATION* pVibration) {
        SDL_Log("XInputSetState(%u, %p)", dwUserIndex, pVibration);

        if (pVibration == nullptr) return ERROR_INVALID_PARAMETER;

        XINPUT_VIBRATION_EX vibrationEx = {};
        vibrationEx.wLeftMotorSpeed = pVibration->wLeftMotorSpeed;
        vibrationEx.wRightMotorSpeed = pVibration->wRightMotorSpeed;
        vibrationEx.wLeftTriggerSpeed = 0;
        vibrationEx.wRightTriggerSpeed = 0;

        return XInputSetStateEx(dwUserIndex, &vibrationEx);
    }

    DWORD WINAPI XInputSetStateEx(DWORD dwUserIndex, const XINPUT_VIBRATION_EX* pVibration) { //GDK (XInputOnGameInput)
        SDL_Log("XInputSetStateEx(%u, %p)", dwUserIndex, pVibration);

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

    DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) {
        SDL_Log("XInputGetCapabilities(%u, %u, %p)", dwUserIndex, dwFlags, pCapabilities);

        if (pCapabilities == nullptr) return ERROR_INVALID_PARAMETER;

        XINPUT_CAPABILITIES_EX capabilitiesEx = {};

        DWORD result = XInputGetCapabilitiesEx(1, dwUserIndex, dwFlags, &capabilitiesEx);
        if (result != ERROR_SUCCESS) return result;

        //Remove GUIDE button
        capabilitiesEx.Capabilities.Gamepad.wButtons &= ~XINPUT_GAMEPAD_GUIDE;

        *pCapabilities = capabilitiesEx.Capabilities;
        return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputGetCapabilitiesEx(DWORD a1, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilities) {
        SDL_Log("XInputGetCapabilitiesEx(%u, %u, %u, %p)", a1, dwUserIndex, dwFlags, pCapabilities);

        if (dwUserIndex >= XUSER_MAX_COUNT || pCapabilities == nullptr)
            return ERROR_INVALID_PARAMETER;

        if (dwFlags > XINPUT_FLAG_GAMEPAD)
            return ERROR_INVALID_PARAMETER;

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return ERROR_NOT_READY;
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(dwUserIndex);
        if (gamepad == nullptr) {
            return ERROR_DEVICE_NOT_CONNECTED;
        }

        SDL_UpdateGamepads();

        ZeroMemory(pCapabilities, sizeof(XINPUT_CAPABILITIES_EX));
        pCapabilities->VendorId = SDL_GetGamepadVendor(gamepad) | 0x045E;
        pCapabilities->ProductId = SDL_GetGamepadProduct(gamepad) | 0x028E;
        pCapabilities->ProductVersion = SDL_GetGamepadProductVersion(gamepad) | 0x114;
        pCapabilities->Capabilities.Type = XINPUT_DEVTYPE_GAMEPAD;

        SDL_GamepadType type = SDL_GetGamepadType(gamepad);
        if (type == SDL_GAMEPAD_TYPE_UNKNOWN)
            pCapabilities->Capabilities.SubType = XINPUT_DEVSUBTYPE_UNKNOWN;
        else
            pCapabilities->Capabilities.SubType = XINPUT_DEVSUBTYPE_GAMEPAD;

        if (type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_VOICE_SUPPORTED;
        }

        if (type == SDL_GAMEPAD_TYPE_XBOX360 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_PMD_SUPPORTED;
        }

        SDL_JoystickConnectionState connectionState = SDL_GetGamepadConnectionState(gamepad);
        if (connectionState == SDL_JOYSTICK_CONNECTION_WIRELESS)
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_WIRELESS;

        SDL_PropertiesID properties = SDL_GetGamepadProperties(gamepad);
        if (SDL_GetBooleanProperty(properties, SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false)) {
            pCapabilities->Capabilities.Flags |= XINPUT_CAPS_FFB_SUPPORTED;
            pCapabilities->Capabilities.Vibration.wLeftMotorSpeed = 255;
            pCapabilities->Capabilities.Vibration.wRightMotorSpeed = 255;
        }

        for (const auto& [sdl_button, xinput_button] : BUTTONS) {
            if (SDL_GamepadHasButton(gamepad, sdl_button)) {
                pCapabilities->Capabilities.Gamepad.wButtons |= xinput_button;
            }
            else if (sdl_button == SDL_GAMEPAD_BUTTON_START ||
                sdl_button == SDL_GAMEPAD_BUTTON_BACK ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_UP ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_DOWN ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_LEFT ||
                sdl_button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT) {
                pCapabilities->Capabilities.Flags |= XINPUT_CAPS_NO_NAVIGATION;
            }
        }

        if (SDL_GamepadHasAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER)) pCapabilities->Capabilities.Gamepad.bLeftTrigger = 255;
        if (SDL_GamepadHasAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)) pCapabilities->Capabilities.Gamepad.bRightTrigger = 255;

        if (SDL_GamepadHasAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX)) pCapabilities->Capabilities.Gamepad.sThumbLX = -64;
        if (SDL_GamepadHasAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY)) pCapabilities->Capabilities.Gamepad.sThumbLY = -64;

        if (SDL_GamepadHasAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX)) pCapabilities->Capabilities.Gamepad.sThumbRX = -64;
        if (SDL_GamepadHasAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY)) pCapabilities->Capabilities.Gamepad.sThumbRY = -64;

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation) {
        SDL_Log("XInputGetBatteryInformation(%u, %u, %p)", dwUserIndex, devType, pBatteryInformation);

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

        SDL_UpdateGamepads();

        ZeroMemory(pBatteryInformation, sizeof(XINPUT_BATTERY_INFORMATION));

        if (devType == BATTERY_DEVTYPE_GAMEPAD) {
            int percent = -1;
            SDL_PowerState powerState = SDL_GetGamepadPowerInfo(gamepad, &percent);

            switch (powerState) {
                  case SDL_POWERSTATE_ERROR: {
                      pBatteryInformation->BatteryType = BATTERY_TYPE_DISCONNECTED;
                      break;
                  }
                  case SDL_POWERSTATE_NO_BATTERY: {
                      pBatteryInformation->BatteryType = BATTERY_TYPE_WIRED;
                      break;
                  }
                  case SDL_POWERSTATE_ON_BATTERY:
                  case SDL_POWERSTATE_CHARGING:
                  case SDL_POWERSTATE_CHARGED: {
                      pBatteryInformation->BatteryType = BATTERY_TYPE_NIMH;

                      SDL_GamepadType type = SDL_GetGamepadType(gamepad);
                      if (type == SDL_GAMEPAD_TYPE_XBOX360 || type == SDL_GAMEPAD_TYPE_XBOXONE) {
                          pBatteryInformation->BatteryType = BATTERY_TYPE_ALKALINE;
                      }
                      break;
                  }
                  case SDL_POWERSTATE_UNKNOWN: {
                      pBatteryInformation->BatteryType = BATTERY_TYPE_UNKNOWN;
                      break;
                  }
            }

            if (percent <= 7) pBatteryInformation->BatteryLevel = BATTERY_LEVEL_EMPTY;
            else if (percent > 7 && percent <= 38) pBatteryInformation->BatteryLevel = BATTERY_LEVEL_LOW;
            else if (percent > 38 && percent <= 69) pBatteryInformation->BatteryLevel = BATTERY_LEVEL_MEDIUM;
            else if (percent > 69 && percent <= 100) pBatteryInformation->BatteryLevel = BATTERY_LEVEL_FULL;
        }
        else {
            pBatteryInformation->BatteryType = BATTERY_TYPE_UNKNOWN;
        }

        return ERROR_SUCCESS;
    }

    DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke) {
        SDL_Log("XInputGetKeystroke(%u, %u, %p)", dwUserIndex, dwReserved, pKeystroke);
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    DWORD WINAPI XInputWaitForGuideButton(DWORD dwUserIndex, DWORD dwFlags, XINPUT_LISTEN_STATE* pState) {
        SDL_Log("XInputWaitForGuideButton(%u, %u, %p)", dwUserIndex, dwFlags, pState);
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    DWORD WINAPI XInputCancelGuideButtonWait(DWORD dwUserIndex) {
        SDL_Log("XInputCancelGuideButtonWait(%u)", dwUserIndex);
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    DWORD WINAPI XInputPowerOffController(DWORD dwUserIndex) {
        SDL_Log("XInputPowerOffController(%u)", dwUserIndex);
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    DWORD WINAPI XInputGetBaseBusInformation(DWORD dwUserIndex, XINPUT_BASE_BUS_INFORMATION* pInfo) {
        SDL_Log("XInputGetBaseBusInformation(%u, %p)", dwUserIndex, pInfo);
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

//Deprecated
    void WINAPI XInputEnable(BOOL enable) {
        SDL_Log("XInputEnable(%d)", enable);
        return;
    }

    DWORD WINAPI XInputGetAudioDeviceIds(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount) {
        SDL_Log("XInputGetAudioDeviceIds(%u, %p, %p ,%p, %p)", dwUserIndex, pRenderDeviceId, pRenderCount, pCaptureDeviceId, pCaptureCount);
        return ERROR_CALL_NOT_IMPLEMENTED;
    }

    DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid) {
        SDL_Log("XInputGetDSoundAudioDeviceGuids(%u, %p, %p)", dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid);
        return ERROR_CALL_NOT_IMPLEMENTED;
    }
#ifdef XINPUT_EXPORTS
}
#endif