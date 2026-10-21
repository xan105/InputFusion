/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dllmain.h"
#include <SDL3/SDL.h>
#include "InputFusion.h"
#include "detours.h"
#include "util.h"

extern HANDLE InputFusion_SDL_Init_Wait;
extern HANDLE InputFusion_SDL_Quit_Wait;

DWORD WINAPI Main(LPVOID lpReserved) {
    #ifdef _DEBUG
        util::EnableConsole();
        std::wstring file = util::GetModuleDirectory() + L"InputFusion.log";
        SDL_IOStream *logFile = SDL_IOFromFile(util::WStringToUtf8(file).c_str(), "w");
        SDL_SetLogOutputFunction(InputFusion_SDL_Logger, logFile);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
    #else
        SDL_SetLogPriorities(SDL_LOG_PRIORITY_ERROR);
    #endif

    InputFusion_SDL_SetHints();
    InputFusion_Detours_SetHooks();

    if (InputFusion_SDL_Init()) {
        InputFusion_SDL_EventLoop();
        InputFusion_SDL_CloseGamepads();
    }
    InputFusion_SDL_Quit();
    
    #ifdef _DEBUG
    if (logFile) {
        SDL_CloseIO(logFile);
    }
    #endif
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