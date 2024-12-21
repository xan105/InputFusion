/*
Copyright (c) Anthony Beaumont
This source code is licensed under the MIT License
found in the LICENSE file in the root directory of this source tree.
*/

#include <Windows.h>
#include <iostream>
#include "vendor/Detour/detours.h"
#if defined(_X86_)
#pragma comment(lib, "vendor/Detour/detours.x86.lib")
#elif defined(_AMD64_)
#pragma comment(lib, "vendor/Detour/detours.x64.lib")
#endif

#if defined(_X86_)
typedef LONG(WINAPI* SetWindowLong_t)(HWND hwnd, int nIndex, LONG dwNewLong);
#elif defined(_AMD64_)
typedef LONG_PTR(WINAPI* SetWindowLong_t)(HWND hwnd, int nIndex, LONG_PTR dwNewLong);
#endif

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#if defined(_X86_)
LONG WINAPI Detour_SetWindowLong(HWND hwnd, int nIndex, LONG dwNewLong);
#elif defined(_AMD64_)
LONG_PTR WINAPI Detour_SetWindowLong(HWND hwnd, int nIndex, LONG_PTR dwNewLong);
#endif


typedef void (WINAPI* ExitProcess_t)(UINT uExitCode);
void WINAPI Detour_ExitProcess(UINT uExitCode);


void setDetoursForWndProcEvent();
void setDetoursForXInput();
bool setDetours();