/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "log.h"
#include "util.h"

namespace {

    void InputFusion_SDL_Logger(void* userdata, int category, SDL_LogPriority priority, const char* message) {
        const char* reset = "\033[0m";
        const char* prefix = "[INFO]";
        const char* prefix_color = "\033[90m";
        const char* tag = "";
        const char* tag_color = "\033[0m";

        switch (priority) {
            case SDL_LOG_PRIORITY_WARN: {
                prefix = "[WARN]";
                prefix_color = "\033[33m";
                break;
            }
            case SDL_LOG_PRIORITY_ERROR:
            case SDL_LOG_PRIORITY_CRITICAL: {
                prefix = "[ERROR]";
                prefix_color = "\033[31m";
                break;
            }
            default: {
                break;
            }
        }

        switch (category) {
            case SDL_LOG_CATEGORY_APPLICATION: {
                tag = "<APP>";
                tag_color = "\033[95m";
                break;
            }
            case SDL_LOG_CATEGORY_INPUT: {
                tag = "<INPUT>";
                tag_color = "\033[96m";
                break;
            }
            case LOG_CATEGORY_LUA: {
                tag = "<LUA>";
                tag_color = "\033[94m";
                break;
            }
            default: {
                break;
            }
        }

        SDL_IOStream* io = (SDL_IOStream*)userdata;
        if (io) {
            SDL_IOprintf(io, "%s %s %s\n", prefix, tag, message);
            SDL_FlushIO(io);
        }
        printf("%s%s %s%s%s %s\n", prefix_color, prefix, tag_color, tag, reset, message);
    }
}

SDL_IOStream* InputFusion_Debug() {
    SDL_IOStream* logFile = nullptr;
    #ifdef _DEBUG
        util::EnableConsole();

        SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG);
        SDL_SetLogPriority(LOG_CATEGORY_LUA, SDL_LOG_PRIORITY_DEBUG);

        std::wstring file = util::GetModuleDirectory() + L"InputFusion.log";
        logFile = SDL_IOFromFile(util::WStringToUtf8(file).c_str(), "w");
        SDL_SetLogOutputFunction(InputFusion_SDL_Logger, logFile);
    #else
        SDL_SetLogPriorities(SDL_LOG_PRIORITY_INVALID);
    #endif
    return logFile;
}