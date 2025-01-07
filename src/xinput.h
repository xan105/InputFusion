/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include <Windows.h>
#include <SDL3/SDL.h>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#define XINPUT_GAMEPAD_THUMB_MAX            32767
#define XINPUT_GAMEPAD_THUMB_MIN            -32768
#define XINPUT_GAMEPAD_TRIGGER_MAX          255

#define XINPUT_DEVTYPE_GAMEPAD              0x01
#define XINPUT_DEVSUBTYPE_GAMEPAD           0x01
#define XINPUT_DEVSUBTYPE_UNKNOWN           0x00
#define XINPUT_DEVSUBTYPE_WHEEL             0x02
#define XINPUT_DEVSUBTYPE_ARCADE_STICK      0x03
#define XINPUT_DEVSUBTYPE_FLIGHT_STICK      0x04
#define XINPUT_DEVSUBTYPE_DANCE_PAD         0x05
#define XINPUT_DEVSUBTYPE_GUITAR            0x06
#define XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE  0x07
#define XINPUT_DEVSUBTYPE_DRUM_KIT          0x08
#define XINPUT_DEVSUBTYPE_GUITAR_BASS       0x0B
#define XINPUT_DEVSUBTYPE_ARCADE_PAD        0x13

#define XINPUT_CAPS_VOICE_SUPPORTED         0x0004
#define XINPUT_CAPS_FFB_SUPPORTED           0x0001
#define XINPUT_CAPS_WIRELESS                0x0002
#define XINPUT_CAPS_PMD_SUPPORTED           0x0008
#define XINPUT_CAPS_NO_NAVIGATION           0x0010

#define XINPUT_GAMEPAD_DPAD_UP              0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN            0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT            0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT           0x0008
#define XINPUT_GAMEPAD_START                0x0010
#define XINPUT_GAMEPAD_BACK                 0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB           0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB          0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER        0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER       0x0200
#define XINPUT_GAMEPAD_A                    0x1000
#define XINPUT_GAMEPAD_B                    0x2000
#define XINPUT_GAMEPAD_X                    0x4000
#define XINPUT_GAMEPAD_Y                    0x8000
#define XINPUT_GAMEPAD_GUIDE                0x0400

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

#define XINPUT_FLAG_GAMEPAD                 0x00000001

#define BATTERY_DEVTYPE_GAMEPAD             0x00
#define BATTERY_DEVTYPE_HEADSET             0x01

#define BATTERY_TYPE_DISCONNECTED           0x00
#define BATTERY_TYPE_WIRED                  0x01
#define BATTERY_TYPE_ALKALINE               0x02
#define BATTERY_TYPE_NIMH                   0x03
#define BATTERY_TYPE_UNKNOWN                0xFF

#define BATTERY_LEVEL_EMPTY                 0x00
#define BATTERY_LEVEL_LOW                   0x01
#define BATTERY_LEVEL_MEDIUM                0x02
#define BATTERY_LEVEL_FULL                  0x03

#define XUSER_MAX_COUNT                     4
#define XUSER_INDEX_ANY                     0x000000FF

#define VK_PAD_A                            0x5800
#define VK_PAD_B                            0x5801
#define VK_PAD_X                            0x5802
#define VK_PAD_Y                            0x5803
#define VK_PAD_RSHOULDER                    0x5804
#define VK_PAD_LSHOULDER                    0x5805
#define VK_PAD_LTRIGGER                     0x5806
#define VK_PAD_RTRIGGER                     0x5807

#define VK_PAD_DPAD_UP                      0x5810
#define VK_PAD_DPAD_DOWN                    0x5811
#define VK_PAD_DPAD_LEFT                    0x5812
#define VK_PAD_DPAD_RIGHT                   0x5813
#define VK_PAD_START                        0x5814
#define VK_PAD_BACK                         0x5815
#define VK_PAD_LTHUMB_PRESS                 0x5816
#define VK_PAD_RTHUMB_PRESS                 0x5817

#define VK_PAD_LTHUMB_UP                    0x5820
#define VK_PAD_LTHUMB_DOWN                  0x5821
#define VK_PAD_LTHUMB_RIGHT                 0x5822
#define VK_PAD_LTHUMB_LEFT                  0x5823
#define VK_PAD_LTHUMB_UPLEFT                0x5824
#define VK_PAD_LTHUMB_UPRIGHT               0x5825
#define VK_PAD_LTHUMB_DOWNRIGHT             0x5826
#define VK_PAD_LTHUMB_DOWNLEFT              0x5827

#define VK_PAD_RTHUMB_UP                    0x5830
#define VK_PAD_RTHUMB_DOWN                  0x5831
#define VK_PAD_RTHUMB_RIGHT                 0x5832
#define VK_PAD_RTHUMB_LEFT                  0x5833
#define VK_PAD_RTHUMB_UPLEFT                0x5834
#define VK_PAD_RTHUMB_UPRIGHT               0x5835
#define VK_PAD_RTHUMB_DOWNRIGHT             0x5836
#define VK_PAD_RTHUMB_DOWNLEFT              0x5837

#define XINPUT_KEYSTROKE_KEYDOWN            0x0001
#define XINPUT_KEYSTROKE_KEYUP              0x0002
#define XINPUT_KEYSTROKE_REPEAT             0x0004

typedef struct _XINPUT_GAMEPAD
{
    WORD    wButtons;
    BYTE    bLeftTrigger;
    BYTE    bRightTrigger;
    SHORT   sThumbLX;
    SHORT   sThumbLY;
    SHORT   sThumbRX;
    SHORT   sThumbRY;
} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;

typedef struct _XINPUT_STATE
{
    DWORD           dwPacketNumber;
    XINPUT_GAMEPAD  Gamepad;
} XINPUT_STATE, *PXINPUT_STATE;

typedef struct _XINPUT_VIBRATION
{
    WORD    wLeftMotorSpeed;
    WORD    wRightMotorSpeed;
} XINPUT_VIBRATION, *PXINPUT_VIBRATION;

typedef struct _XINPUT_VIBRATION_EX
{
    WORD    wLeftMotorSpeed;
    WORD    wRightMotorSpeed;
    WORD    wLeftTriggerSpeed; 
    WORD    wRightTriggerSpeed;
} XINPUT_VIBRATION_EX, *PXINPUT_VIBRATION_EX;

typedef struct _XINPUT_CAPABILITIES
{
    BYTE                Type;
    BYTE                SubType;
    WORD                Flags;
    XINPUT_GAMEPAD      Gamepad;
    XINPUT_VIBRATION    Vibration;
} XINPUT_CAPABILITIES, *PXINPUT_CAPABILITIES;

typedef struct _XINPUT_CAPABILITIES_EX
{
    XINPUT_CAPABILITIES   Capabilities;
    WORD                  VendorId;
    WORD                  ProductId;
    WORD                  ProductVersion;
    WORD                  unk1; //unknown
    DWORD                 unk2; //unknown
} XINPUT_CAPABILITIES_EX, *PXINPUT_CAPABILITIES_EX;

typedef struct _XINPUT_BATTERY_INFORMATION
{
    BYTE    BatteryType;
    BYTE    BatteryLevel;
} XINPUT_BATTERY_INFORMATION, *PXINPUT_BATTERY_INFORMATION;

typedef struct _XINPUT_KEYSTROKE
{
    WORD    VirtualKey;
    WCHAR   Unicode;
    WORD    Flags;
    BYTE    UserIndex;
    BYTE    HidCode;
} XINPUT_KEYSTROKE, *PXINPUT_KEYSTROKE;

typedef struct _XINPUT_LISTEN_STATE
{
    DWORD   Status;
    DWORD   unk1; //unknown
    WORD    unk2; //unknown
    BYTE    unk3; //unknown
    BYTE    unk4; //unknown
    WORD    unk5; //unknown
    WORD    unk6; //unknown
    WORD    unk7; //unknown
    WORD    unk8; //unknown
} XINPUT_LISTEN_STATE, *PXINPUT_LISTEN_STATE;

typedef struct _XINPUT_BASE_BUS_INFORMATION
{
    WORD    VendorId; //unknown
    WORD    ProductId; //unknown
    WORD    InputId; //unknown
    WORD    Field_6; //unknown
    DWORD   Field_8; //unknown
    BYTE    Field_C; //unknown
    BYTE    Field_D; //unknown
    BYTE    Field_E; //unknown
    BYTE    Field_F; //unknown
} XINPUT_BASE_BUS_INFORMATION, *PXINPUT_BASE_BUS_INFORMATION;

typedef DWORD(WINAPI* XInputEnable_t)(BOOL enabled);
typedef DWORD(WINAPI* XInputGetAudioDeviceIds_t)(DWORD dwUserIndex, WCHAR* pRenderDeviceId, WCHAR* pCaptureDeviceId);
typedef DWORD(WINAPI* XInputGetBatteryInformation_t)(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation);
typedef DWORD(WINAPI* XInputGetKeystroke_t)(DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE* pKeystroke);
typedef DWORD(WINAPI* XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD(WINAPI* XInputSetState_t)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
typedef DWORD(WINAPI* XInputGetCapabilities_t)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);
typedef DWORD(WINAPI* XInputGetDSoundAudioDeviceGuids_t)(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid);
typedef DWORD(WINAPI* XInputGetStateEx_t)(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD(WINAPI* XInputWaitForGuideButton_t)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_LISTEN_STATE* pState);
typedef DWORD(WINAPI* XInputCancelGuideButtonWait_t)(DWORD dwUserIndex);
typedef DWORD(WINAPI* XInputPowerOffController_t)(DWORD dwUserIndex);
typedef DWORD(WINAPI* XInputGetBaseBusInformation_t)(DWORD dwUserIndex, XINPUT_BASE_BUS_INFORMATION* pInfo);
typedef DWORD(WINAPI* XInputGetCapabilitiesEx_t)(DWORD a1, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilities);

DWORD GetState(DWORD dwUserIndex, XINPUT_STATE* pState, bool extended);
void GetCapabilities(SDL_Gamepad* gamepad, XINPUT_CAPABILITIES* pCapabilities, bool extended);

#ifdef XINPUT_EXPORTS
extern "C" {
#endif
    void WINAPI XInputEnable(BOOL enable);
    DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
    DWORD WINAPI XInputSetState(DWORD dwUserIndex, const XINPUT_VIBRATION* pVibration);
    DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation);
    DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);
    DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke);
    DWORD WINAPI XInputGetStateEx(DWORD dwUserIndex, XINPUT_STATE* pState);
    DWORD WINAPI XInputWaitForGuideButton(DWORD dwUserIndex, DWORD dwFlags, XINPUT_LISTEN_STATE* pState);
    DWORD WINAPI XInputCancelGuideButtonWait(DWORD dwUserIndex);
    DWORD WINAPI XInputPowerOffController(DWORD dwUserIndex);
    DWORD WINAPI XInputGetBaseBusInformation(DWORD dwUserIndex, XINPUT_BASE_BUS_INFORMATION* pInfo);
    DWORD WINAPI XInputGetCapabilitiesEx(DWORD a1, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilities);
    DWORD WINAPI XInputSetStateEx(DWORD dwUserIndex, const XINPUT_VIBRATION_EX* pVibration);
    DWORD WINAPI XInputGetAudioDeviceIds(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount);
    DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid);
#ifdef XINPUT_EXPORTS
}
#endif