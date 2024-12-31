About
=====

This is an attempt at creating an _"open source Steam Input"_. Specifically the Gamepad APIs translating part.

This project aims to hook and re-implement various gamepad APIs such as XInput over [SDL](https://www.libsdl.org/).

The goal is to allow old and new games to support any gamepad controller supported by SDL (over 200+) out of the box with zero configuration.

<p align="center">
  <img src="https://github.com/xan105/InputFusion/raw/main/screenshot/debug.png">
  <em>XInput API calls</em>
</p>

Gamepad APIs support
====================

<details><summary>XInput</summary>

  - XInputGetState ✔️
  - XInputGetStateEx ✔️
  - XInputSetState ✔️
  - XInputSetStateEx¹ ✔️
  - XInputGetBatteryInformation ✔️
  - XInputGetCapabilities ✔️
  - XInputGetCapabilitiesEx ✔️ 
  - XInputGetKeystroke ❌️
  - XInputWaitForGuideButton ❌️
  - XInputCancelGuideButtonWait ❌
  - XInputPowerOffController ❌
  - XInputGetBaseBusInformation ❌
  - XInputEnable 🚫
  - XInputGetAudioDeviceIds 🚫
  - XInputGetDSoundAudioDeviceGuids 🚫

¹ NB: XInputSetStateEx() from GDK _(XInputOnGameInput)_ is implemented and has been arbitrarily set to ordinal 1000. It does not exist in XInput.

</details>

<details><summary>DInput8</summary>

  - DirectInput8Create ✔️
    + IDirectInput8::ConfigureDevices ❌
    + IDirectInput8::CreateDevice ⚠
      - IDirectInputDevice8::Acquire ⚠
      - IDirectInputDevice8::BuildActionMap ❌
      - IDirectInputDevice8::CreateEffect ❌
      - IDirectInputDevice8::EnumCreatedEffectObjects ❌
      - IDirectInputDevice8::EnumEffects ❌
      - IDirectInputDevice8::EnumEffectsInFile ❌
      - IDirectInputDevice8::EnumObjects ⚠
      - IDirectInputDevice8::Escape ❌
      - IDirectInputDevice8::GetCapabilities ✔
      - IDirectInputDevice8::GetDeviceData ❌
      - IDirectInputDevice8::GetDeviceInfo ❌
      - IDirectInputDevice8::GetDeviceState ⚠
      - IDirectInputDevice8::GetEffectInfo ❌
      - IDirectInputDevice8::GetForceFeedbackState ❌
      - IDirectInputDevice8::GetImageInfo ❌
      - IDirectInputDevice8::GetObjectInfo ❌
      - IDirectInputDevice8::GetProperty ❌
      - IDirectInputDevice8::Initialize ⚠
      - IDirectInputDevice8::Poll ✔
      - IDirectInputDevice8::RunControlPanel ❌
      - IDirectInputDevice8::SendDeviceData ❌
      - IDirectInputDevice8::SendForceFeedbackCommand ❌
      - IDirectInputDevice8::SetActionMap ❌
      - IDirectInputDevice8::SetCooperativeLevel ⚠
      - IDirectInputDevice8::SetDataFormat ⚠
      - IDirectInputDevice8::SetEventNotification ❌
      - IDirectInputDevice8::SetProperty ❌
      - IDirectInputDevice8::Unacquire ⚠
      - IDirectInputDevice8::WriteEffectToFile ❌
    + IDirectInput8::EnumDevices ⚠
    + IDirectInput8::EnumDevicesBySemantics ❌
    + IDirectInput8::FindDevice ❌
    + IDirectInput8::GetDeviceStatus ❌
    + IDirectInput8::Initialize ⚠
    + IDirectInput8::RunControlPanel ❌

</details>

<details><summary>DInput (1-7)</summary>
_To Do_
</details>

<details><summary>WinMM</summary>

  - joyConfigChanged ❌
  - joyGetDevCapsA ❌
  - joyGetDevCapsW ❌
  - joyGetNumDevs ❌
  - joyGetPos ❌
  - joyGetPosEx ❌
  - joyGetThreshold ❌
  - joyReleaseCapture ❌
  - joySetCapture ❌
  - joySetThreshold ❌
  
</details>

|✔|⚠|❌|🚫|
|-|-|-|-|
|Implemented|Work in progress|To do|Won't (Deprecated)|

### Out of scope

<details><summary>Steam Input API</summary>
  <br/>
  Steam Input API only games. You need an action set to translate input. Hooking these API is going down the Steam Enulator rabbit hole.
  
  Not sure yet how best to handle this. Meanwhile when using a Steam Emulator that translates Steam Input to XInput you can leverage InputFusion to then translate XInput to SDL.
  
  <p align="center">
  <img src="https://github.com/xan105/InputFusion/raw/main/screenshot/SteamInput_to_XInput.png">
  <em>Steam Input -> XInput -> SDL</em>
  </p>
  
</details>
  
<details><summary>HID / Raw</summary>
  <br/>
  These APIs aren't really like the standardised Gamepad APIs like XInput. They are much akin to low level access.
  
  SDL mostly uses these low level APIs.
  
  If a game uses these APIs to add support for a specific Gamepad;
  The game devs probably have a certain experience in mind and we shouldn't interfere with it.
  
  Many mods and other 3rd party "fix" rely on these low level access to do their job.
  And they often complain about the new Steam Input capabilities of hooking system wide all relevant APIs for gamepad while Steam is running.
  
  As such, I do no think these API are relevant for my project.
  
</details>

<details><summary>WGI (Windows Gaming Input)</summary>
  <br/>
  This API is new and specifically designed to allow support for Gamepad others than Xbox controllers in a standardised way.
  Doesn't seem pertinent to this project for now.
  
</details>

Usage
=====

The DLL can be used either as 
- **A)** a drop-in replacement _(DLL side-loading)_ or
- **B)** by being injected into a target process.

Unless you know what you are doing stick with the first approach.

## A) DLL side-loading

Replace `xinput*.dll` in the game dir if present, otherwise place it next to the game's executable.

This might require a little guess work to find which version of XInput a game is using: xinput1_4.dll, xinput1_3.dll, xinput9_1_0.dll.

The DLL exports every documented XInput functions as well as "hidden" XInput___Ex functions.

_NB: If necessary, you can opt-in to also hook / detour XInput functions to force the game to use the ones from the DLL. See env var section below._

## B) DLL Injection

Set the env var to enable function hooking / detouring before you execute and inject into your target process. _See env var section below._
Otherwise once injected it won't do much more than just init. SDL.

Here is a simple example in Node.js using [xan105/node-remote-thread](https://github.com/xan105/node-remote-thread):

```js
import { env } from "node:process";
import { spawn } from "node:child_process";
import { dirname } from "node:path";
import { createRemoteThread } from "@xan105/create=-remote-thread";

const EXECUTABLE = "G:\\METAPHOR\\METAPHOR.exe";
const ADDON = "G:\\METAPHOR\\InputFusion.dll";
const ARGS = [];

const binary = spawn(EXECUTABLE, ARGS, {
  cwd: dirname(EXECUTABLE),
  stdio:[ "ignore", "ignore", "ignore" ], 
  detached: true,
  env: {
    ...env,
    "GAMEPAD_API_XINPUT": "HOOK"
  }
});

binary.once("spawn", () => {
  binary.unref();
  createRemoteThread(binary.pid, ADDON);
});
```


Env Var
=======

🧪 Mostly experimental features are behind env var flags.

#### `GAMEPAD_LED=BATTERYLVL`

When enabled the LED light of the controller is used to show the battery level of the controller:

  - 100% / Green
  - 75% / Yellow
  - 50% / Orange
  - 25% / Red

Currently only available for PS4/PS5 controller in wireless.

#### `GAMEPAD_API_XINPUT=HOOK`

Enable XInput functions hooking / detouring.<br />
This forces the use of the XInput functions from the DLL when calling XInput functions.

#### `GAMEPAD_API_DINPUT8=HOOK`

Enable DInput8 functions hooking / detouring.

> [!WARNING]  
> The current implementation is very barebone and is based on a Xbox 360 controller, therefore it has the same limitations as a real Xbox 360 controller with DInput such as no force feedback and no individual trigger axis..
> Games have different DInput layout expectation depending on their era and/or gamepads they support.

Caveats
=======

- SDL might still be initializing when the game does it's first Gamepad API call (on startup).

- Games that support more than one input API usually, but not always, do a lot of Win32 APIs sniffing behind the scenes to determine which input API to use.
  Therefore even tho an input API is translated to SDL, your gamepad may still not work due to how a game engine is programmed and how it decides to handle input.
  The focus of this project is **for now** on API translation and not on _poorly engineered_ games compatibility. 

Build
=====

🆚 **Visual Studio 2022**

📦 Vendor dependencies:
  - [Microsoft Detours library](https://github.com/microsoft/Detours)
  - [SDL3 Library](https://github.com/libsdl-org/SDL) recompiled as a static lib.

Solution: `./vc/InputFusion.sln`

The 4 projects inside the solution control the build output:
  - InputFusion -> `InputFusion.dll` (DLL injection)
  - XInput -> `xinput.dll` (DLL sideloading / injection)
  - DInput8 -> `dinput8.dll` (DLL sideloading / injection)
  - WinMM -> `winmm.dll` (DLL sideloading / injection)
  
Output: `./build/${project}/output/${platform}/${config}`