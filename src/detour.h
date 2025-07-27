/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <SDL3/SDL.h>
#include <atomic>
#include "../vendor/Detour/detours.h"
#if defined(_X86_)
#pragma comment(lib, "../vendor/Detour/detours.x86.lib")
#elif defined(_AMD64_)
#pragma comment(lib, "../vendor/Detour/detours.x64.lib")
#endif
#if defined(INPUTFUSION_EXPORTS)
#include "XInput/xinput.h"
#include "DInput8/dinput8.h"
#include "WinMM/winmm.h"
#elif defined(XINPUT_EXPORTS)
#include "XInput/xinput.h"
#include "DInput8/dinput8.h"
#elif defined(DINPUT8_EXPORTS)
#include "DInput8/dinput8.h"
#elif defined(WINMM_EXPORTS)
#include "WinMM/winmm.h"
#endif
#include "util.h"

typedef void (WINAPI* ExitProcess_t)(UINT uExitCode);

void WINAPI Detour_ExitProcess(UINT uExitCode);
bool takeDetour(PVOID* ppPointer, PVOID pDetour);
bool setDetoursExitProcess();
#if defined(INPUTFUSION_EXPORTS) || defined(XINPUT_EXPORTS)
bool setDetoursForXInput();
#endif
#if defined(INPUTFUSION_EXPORTS) || defined(DINPUT8_EXPORTS) || defined(XINPUT_EXPORTS)
bool setDetoursForDInput8();
#endif
#if defined(INPUTFUSION_EXPORTS) || defined(WINMM_EXPORTS)
bool setDetoursForWinmm();
#endif
void setDetours();