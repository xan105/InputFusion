/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL3/SDL.h>
#include <string>

typedef enum LOG_CATEGORY {
    LOG_CATEGORY_LUA = SDL_LOG_CATEGORY_CUSTOM
} LOG_CATEGORY;

namespace {
    std::string strip_ansi(const char* message);
    void InputFusion_SDL_Logger(void* userdata, int category, SDL_LogPriority priority, const char* message);
}

SDL_IOStream* InputFusion_Debug();