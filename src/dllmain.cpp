/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dllmain.h"
#include "InputFusion.h"
#include "detours.h"
#include "lua/lua.h"
#include "log.h"

#include <thread>

extern HANDLE InputFusion_SDL_Init_Wait;
extern HANDLE InputFusion_SDL_Quit_Wait;

DWORD WINAPI Main(LPVOID lpReserved) {
    SDL_IOStream* logFile = InputFusion_Debug();
    InputFusion_SDL_SetHints();
    InputFusion_Detours_SetHooks();
    std::jthread lua_thread(InputFusion_Lua_Run);
    if (InputFusion_SDL_Init()) {
        InputFusion_SDL_EventLoop();
        InputFusion_SDL_CloseGamepads();
    }
    InputFusion_SDL_Quit(logFile);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            InputFusion_SDL_Init_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            InputFusion_SDL_Quit_Wait = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            HANDLE hThread = CreateThread(nullptr, 0, Main, hModule, 0, nullptr);
            if (hThread) {
                CloseHandle(hThread);
            }
            break;
        }
    }
    return TRUE;
}