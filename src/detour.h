/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include <Windows.h>
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
bool setDetoursForXInput();
bool setDetoursForDInput8();
void setDetours();