/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "log.h"
#include "util.h"
#include <regex>

namespace {

    std::string strip_ansi(const char* message) {
        static const std::regex ansi(R"(\x1B\[[0-?]*[ -/]*[@-~])");
        return std::regex_replace(message, ansi, "");
    }

    void InputFusion_SDL_Logger(void* userdata, int category, SDL_LogPriority priority, const char* message) {

        const char* prefix = "\033[90m[INFO]\033[0m";
        const char* tag = "";

        switch (priority) {
            case SDL_LOG_PRIORITY_WARN: {
                prefix = "\033[33m[WARN]\033[0m";
                break;
            }
            case SDL_LOG_PRIORITY_ERROR:
            case SDL_LOG_PRIORITY_CRITICAL: {
                prefix = "\033[31m[ERROR]\033[0m";
                break;
            }
            default: {
                break;
            }
        }

        switch (category) {
            case SDL_LOG_CATEGORY_APPLICATION: {
                tag = "\033[95m<APP>\033[0m";
                break;
            }
            case SDL_LOG_CATEGORY_INPUT: {
                tag = "\033[96m<INPUT>\033[0m";
                break;
            }
            case LOG_CATEGORY_LUA: {
                tag = "\033[94m<LUA>\033[0m";
                break;
            }
            default: {
                break;
            }
        }

        SDL_IOStream* io = (SDL_IOStream*)userdata;
        if (io) {
            SDL_IOprintf(io, "%s %s %s\n", strip_ansi(prefix).c_str(), strip_ansi(tag).c_str(), strip_ansi(message).c_str());
            SDL_FlushIO(io);
        }
        printf("%s %s %s\n", prefix, tag, message);
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