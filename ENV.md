#### `GAMEPAD_LED=BATTERYLVL`

**BATTERYLVL**

When enabled the LED light of the controller is used to show the battery level of the controller:

  - 100% / Green
  - 75% / Yellow
  - 50% / Orange
  - 25% / Red
  
Currently only available for PS4/PS5 controller in wireless.

#### `GAMEPAD_RUMBLE=NONE`

Disable motor vibration. This includes _"Impulse Triggers"_ on newer Xbox controllers.

#### `GAMEPAD_API_GAMEINPUT=HOOK`

Enable GameInput functions hooking / detouring.<br />
This forces the use of the GameInput functions from the DLL when calling GameInput functions.

⚠️ This will also disable GameInput usage within SDL to prevent conflict due to API hooking (SDL_HINT_JOYSTICK_GAMEINPUT=0).

#### `GAMEPAD_API_XINPUT=HOOK`

Enable XInput functions hooking / detouring.<br />
This forces the use of the XInput functions from the DLL when calling XInput functions.

⚠️ This will also disable XInput usage within SDL to prevent conflict due to API hooking (SDL_HINT_XINPUT_ENABLED=0).

#### `GAMEPAD_API_XINPUT_DEVICE_CONNECTED=ALWAYS`

All XInput slots will report as connected, returning dummy data when necessary.  
This helps with games that do not properly handle gamepad disconnection (hot reload), as they may not retry after receiving **ERROR_DEVICE_NOT_CONNECTED**.

#### `GAMEPAD_API_XINPUT_VID_PID=DEVICE`

When enabled `XInputGetCapabilitiesEx()` returns the real VID/PID (Vendor ID / Product ID) of the device, as reported by SDL or fallbacks to a Xbox 360 controller when they aren't available.

By default InputFusion returns the VID/PID of a Xbox 360 controller.

#### `GAMEPAD_API_DINPUT8=HOOK`

Enable DInput8 functions hooking / detouring.
This forces the use of the DInput8 functions from the DLL when calling DInput8 functions.

⚠️ This will also disable DirectInput usage within SDL to prevent conflict due to API hooking (SDL_HINT_JOYSTICK_DIRECTINPUT=0).

#### `GAMEPAD_API_DINPUT8_NODEVICES=ENABLE`

Do not return any devices when enumerating devices with DInput8.

#### `GAMEPAD_API_DINPUT=HOOK`

Enable DInput 1-7 functions hooking / detouring.
This forces the use of the DInput functions from the DLL when calling DInput functions.

#### `GAMEPAD_API_WINMM=HOOK`

Enable WinMM functions hooking / detouring.
This forces the use of the WinMM functions from the DLL when calling WinMM Joystick related functions.

#### `GAMEPAD_API_WINMM_LAYOUT=XBOX`

Use the modern Xbox controller layout with WinMM. Useful for modern games that decided to use a legacy API for a Xbox controller such as [Deadly Premonition](https://store.steampowered.com/app/247660/Deadly_Premonition_The_Directors_Cut/).

#### `INPUTFUSION_LUA_FILE=/path/to/file`

Lua script file path (absolute or relative).