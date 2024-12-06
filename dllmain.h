/*
Copyright (c) Anthony Beaumont
This source code is licensed under the MIT License
found in the LICENSE file in the root directory of this source tree.
*/

#include <Windows.h>
#include <SDL3/SDL.h>
#include <vector>
#include <iostream>
#include "vendor/Detour/detours.h"
#if defined(_X86_)
	#pragma comment(lib, "vendor/Detour/detours.x86.lib")
#elif defined(_AMD64_)
	#pragma comment(lib, "vendor/Detour/detours.x64.lib")
#endif

typedef LONG_PTR(WINAPI* SetWindowLongPtrW_t)(HWND hwnd, int nIndex, LONG_PTR dwNewLong);

void closeGamepads();
void SDL_eventLoop();
bool setDetoursForXInput();
bool setDetoursForWndProcEvent();
LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LONG_PTR WINAPI Detour_SetWindowLongPtrW(HWND hwnd, int nIndex, LONG_PTR dwNewLong);
DWORD WINAPI Main(LPVOID lpReserved);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);