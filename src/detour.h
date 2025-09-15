/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
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
#if defined(INPUTFUSION_EXPORTS) || defined(DINPUT_EXPORTS)
bool setDetoursForDInput();
#endif
#if defined(INPUTFUSION_EXPORTS) || defined(WINMM_EXPORTS)
bool setDetoursForWinmm();
#endif
void setDetours();