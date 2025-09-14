About
=====

This is an attempt at creating an _"open source Steam Input"_. Specifically the Gamepad APIs translating part.

This project aims to hook and re-implement various gamepad APIs such as XInput over [SDL3](https://www.libsdl.org/).

The goal is to allow old and new games to support any gamepad controller supported by SDL3 (over 200+) out of the box with zero configuration.

If you are a game developper and you'd like your game to support a variety of gamepad controllers without rewriting your game, this project might be of interest to you.

<p align="center">
  <img src="https://github.com/xan105/InputFusion/raw/main/screenshot/debug.png">
  <em>XInput API calls ("Debug mode")</em>
</p>

Gamepad APIs support
====================

- ~~GameInput~~
- XInput
- DInput8
- DInput(1-7)
- WinMM

NB: DInput and DInput8 are still a work in progress. Depending on the game, "your mileage may vary", as they say.
  
## Out of scope (for now)

<details><summary>Steam Input API</summary>
  <br/>
  Steam Input API only games. You need an action set to translate input. Hooking these API is going down the Steam Enulator rabbit hole.
  
  Not sure yet how best to handle this. Meanwhile when using a Steam Emulator that translates Steam Input to XInput you can leverage InputFusion to then translate XInput to SDL.
  
  <p align="center">
  <img src="https://github.com/xan105/InputFusion/raw/main/screenshot/SteamInput_to_XInput.png">
  <em>Steam Input -> XInput -> SDL ("Debug mode")</em>
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
  
  As such, I do no think these API are relevant for my project (for now).
  
</details>

<details><summary>WGI: Windows.Gaming.Input Namespace</summary>
  <br/>
  Originaly designed for UWP/WinRT apps, now legacy, it can be used in win32 apps with some effort.
</details>

Usage
=====

InputFusion can be used either as 
- **A)** a drop-in replacement _(DLL side-loading)_ or
- **B)** by being injected into a target process.

Unless you know what you are doing stick with the first approach.

## A) DLL side-loading

Use `xinput*.dll` or `dinput8.dll` or `winmm.dll` depending on which API the game is using.

Replace the corresponding DLL in the game directory if present, otherwise place it next to the game's executable.

In the case of XInput, this might require a little guess work to find which version of XInput the game is using: xinput1_4.dll, xinput1_3.dll, xinput9_1_0.dll.

_NB: If necessary, you can opt-in to also hook/detour API calls to force the game to use the ones from the DLL. See `Env Var` section below._

## B) DLL Injection

> [!IMPORTANT]
> Before executing and injecting into your target process, you must specify which API you intend to hook/detour.
> This is achieved by setting the corresponding environment variable for the desired API.
>
> If you do not configure the environment variable before injection, the process will only initialize SDL without performing any meaningful actions.
> For details please refer to the `Env Var` section below.

You need a DLL injector to inject `InputFusion` into the target process.

A quick google search will find you plenty on GitHub.<br/>
🐧 Linux: the classic combo `createRemoteThread()` + `LoadLibrary()` from `Kernel32` works under Wine/Proton.

Alternatively, here are some of my own:

- [xan105/Mini-Launcher](https://github.com/xan105/Mini-Launcher):

  > CLI launcher with DLL Injection, Lua Scripting, Splash screen, and other goodies.

- [xan105/node-remote-thread](https://github.com/xan105/node-remote-thread):

  > Node.js NAPI Native addon for Windows DLL injection with support for Wow64 and Unicode path.

> [!TIP]
> Consider changing the file extension from `.dll` to `.asi` to help prevent false positive with Windows Defender.

<details><summary>Example (xan105/Mini-Launcher)</summary>

```json
{
  "bin": "Binaries/NMS.exe",
  "env": {
    "GAMEPAD_API_XINPUT": "HOOK"
  },
  "addons": [
    { "path": "Binaries/InputFusion.asi", "required": true }
  ]
}
```
</details>

<details><summary>Example (xan105/node-remote-thread)</summary>

```js
import { env } from "node:process";
import { spawn } from "node:child_process";
import { dirname } from "node:path";
import { createRemoteThread } from "@xan105/remote-thread";

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
</details>

Gamepad Layout
==============

Games have different layout expectation depending on their era and/or gamepads they support.
For practical reasons, this project tries to mimic a Xbox controller unless otherwise specified.
Very old and legacy APIs tend to have a more custom/retro layout to match their era.

💡 Please be advised that SDL has also the ability to change the mapping of a gamepad: see [Input Re-mapping](#input-re-mapping) below.

- XInput:

As an Xbox controller

- DInput8:

As an Xbox controller therefore it has the same limitations as a real Xbox controller with DInput such as no individual trigger axis.

- DInput (1-7):

As DInput8.

- WinMM:

> [!NOTE] 
> After considering the games from that era that could be played with a gamepad. I somewhat deviated from the "usual" modern mapping for this implementation.

The D-Pad and the left joystick both map the X/Y axis, usually used for movement.
The right joystick maps to Z/R axes and to the POV when pressed + _direction_, usually used to move the camera point of view.
The right and left trigger are mapped to buttons.
Buttons order is tweaked to allow triggers in games supporting a limited amount of buttons and start/options to usually end up as the PAUSE button.

> [!TIP]
> If for some reasons you'd like the modern Xbox controller layout with WinMM, you can use the env. var. `GAMEPAD_API_WINMM_LAYOUT=XBOX` (See below).

Input Re-mapping
================

SDL has the ability to change the mapping of an existing gamepad or add support for gamepads that SDL is unaware of.
To manually do so the end user can, for example, use the env var `SDL_GAMECONTROLLERCONFIG` or `SDL_GAMECONTROLLERCONFIG_FILE`.

The mapping string has the format: `GUID,name,mapping`. 

Example of a valid mapping for a Dualshock4 v2:
```
"03008fe54c050000a00b000000016800,*,a:b0,b:b1,back:b4,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,guide:b5,leftshoulder:b9,leftstick:b7,lefttrigger:a4,leftx:a0,lefty:a1,rightshoulder:b10,rightstick:b8,righttrigger:a5,rightx:a2,righty:a3,start:b6,x:b2,y:b3,touchpad:b11,crc:e58f,platform:Windows,"
```

🎮 Buttons can be used as axes and vice versa.

Example to swap the D-PAD with the Left Joystick for retro gaming:
```
[...] dpup:-a1,dpdown:+a1,dpleft:-a0,dpright:+a0,-leftx:h0.8,+leftx:h0.2,-lefty:h0.1,+lefty:h0.4, [...]
```

📄 For more details, please kindly see the [SDL documentation](https://wiki.libsdl.org/SDL3/SDL_AddGamepadMapping).

> [!TIP]
> [SDL testcontroller](https://github.com/libsdl-org/SDL/tree/main/test) is a GUI tool that can be used to create a string mapping (Setup Mapping > Copy).
> A precompiled build can be found in this repo `/test/SDL_testcontroller` folder.

<p align="center">
  <img src="https://github.com/xan105/InputFusion/raw/main/screenshot/SDL_testcontroller.png">
  <em>SDL testcontroller</em>
</p>

Env Var
=======

🧪 Mostly experimental features are behind env var flags.

#### `GAMEPAD_LED=BATTERYLVL`

**BATTERYLVL**

When enabled the LED light of the controller is used to show the battery level of the controller:

  - 100% / Green
  - 75% / Yellow
  - 50% / Orange
  - 25% / Red
  
Currently only available for PS4/PS5 controller in wireless.

`DLL: Any`

#### `GAMEPAD_API_XINPUT=HOOK`

Enable XInput functions hooking / detouring.<br />
This forces the use of the XInput functions from the DLL when calling XInput functions.

`DLL: InputFusion, XInput`

#### `GAMEPAD_API_DINPUT8=HOOK`

Enable DInput8 functions hooking / detouring.
This forces the use of the DInput8 functions from the DLL when calling DInput8 functions.

`DLL: InputFusion, Dinput8, XInput`

#### `GAMEPAD_API_DINPUT=HOOK`

Enable DInput 1-7 functions hooking / detouring.
This forces the use of the DInput functions from the DLL when calling DInput functions.

`DLL: InputFusion, Dinput`

#### `GAMEPAD_API_WINMM=HOOK`

Enable WinMM functions hooking / detouring.
This forces the use of the WinMM functions from the DLL when calling WinMM Joystick related functions.

`DLL: InputFusion, WinMM`

#### `GAMEPAD_API_WINMM_LAYOUT=XBOX`

Use the modern Xbox controller layout with WinMM. Useful for modern games that decided to use a legacy API for a Xbox controller such as [Deadly Premonition](https://store.steampowered.com/app/247660/Deadly_Premonition_The_Directors_Cut/).

`DLL: InputFusion, WinMM`

Linux (Wine/Proton)
===================

Wine/Proton already translates Windows gamepad API calls. So it's a bit redundant, but yes this project does work under Wine/Proton.
You just need to tell Wine/Proton to load the dll instead of its built-in:

Example:

```
WINEDLLOVERRIDES="xinput1_3=n,b
```

NB: In case of DLL injection you need a DLL injector that can run under Wine/Proton.
The classic combo `createRemoteThread()` + `LoadLibrary()` from Kernel32 works under Wine/Proton.
A quick google search will find you plenty on GitHub. Otherwise may I suggest one of my own: [xan105/Mini-Launcher](https://github.com/xan105/Mini-Launcher)

Caveats
=======

- SDL might still be initializing when the game does it's first Gamepad API call (on startup).

- Games that support more than one input API usually, but not always, do a lot of Win32 APIs sniffing behind the scenes to determine which input API to use.
  Therefore even tho an input API is translated to SDL, your gamepad may still not work due to how a game engine is programmed and how it decides to handle input.
  The focus of this project is **for now** on API translation and not on compatibility for _poorly engineered_ games. 

Build
=====

🆚 **Visual Studio 2022**

📦 Vendor dependencies:
  - [Microsoft Detours library](https://github.com/microsoft/Detours)
  - [SDL3 Library](https://github.com/libsdl-org/SDL) recompiled as a static lib.

Solution: `./vc/InputFusion.sln`

The 4 projects inside the solution control the build output:
  - InputFusion -> `InputFusion.dll` (DLL injection)
  - XInput      -> `xinput.dll` (DLL sideloading / injection)
  - DInput8     -> `dinput8.dll` (DLL sideloading / injection)
  - DInput      -> `dinput.dll` (DLL sideloading / injection)
  - WinMM       -> `winmm.dll` (DLL sideloading / injection)
  
Output: `./build/${project}/output/${platform}/${config}`
