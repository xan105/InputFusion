/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

typedef void (WINAPI* ExitProcess_t)(UINT uExitCode);

namespace {
    void WINAPI InputFusion_ExitProcess(UINT uExitCode);

    bool InputFusion_Detours_Attach(PVOID* ppPointer, PVOID pDetour);
    bool InputFusion_Detours_Hook_ExitProcess();
    bool InputFusion_Detours_Hook_GameInput();
    bool InputFusion_Detours_Hook_XInput();
    bool InputFusion_Detours_Hook_DInput8();
    bool InputFusion_Detours_Hook_DInput();
    bool InputFusion_Detours_Hook_Winmm();
}

void InputFusion_Detours_SetHooks();