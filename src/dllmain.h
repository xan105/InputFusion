/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include <Windows.h>
#include <SDL3/SDL.h>
#include <iostream>

void closeGamepads();
void SDL_eventLoop();
DWORD WINAPI Main(LPVOID lpReserved);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);