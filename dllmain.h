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

#if defined(_X86_)
	typedef LONG(WINAPI* SetWindowLong_t)(HWND hwnd, int nIndex, LONG dwNewLong);
#elif defined(_AMD64_)
	typedef LONG_PTR(WINAPI* SetWindowLong_t)(HWND hwnd, int nIndex, LONG_PTR dwNewLong);
#endif

void closeGamepads();
void SDL_eventLoop();
bool setDetoursForXInput();
bool setDetoursForWndProcEvent();
LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#if defined(_X86_)
	LONG WINAPI Detour_SetWindowLong(HWND hwnd, int nIndex, LONG dwNewLong);
#elif defined(_AMD64_)
	LONG_PTR WINAPI Detour_SetWindowLong(HWND hwnd, int nIndex, LONG_PTR dwNewLong);
#endif
DWORD WINAPI Main(LPVOID lpReserved);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);