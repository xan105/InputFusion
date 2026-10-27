/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.

This file is based upon the original source code of
[AnyLua](https://github.com/xan105/AnyLua) distributed under the MIT License.

AnyLua source: https://github.com/xan105/AnyLua.git
Mirror: https://download.xan105.com/AnyLua/Source%20code.zip

InputFusion source: https://github.com/xan105/InputFusion.git
Mirror: https://download.xan105.com/InputFusion/Source%20code.zip
*/

#include "console.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../lua.h"

namespace {

    const char* reset       = "\033[0m";
    const char* yellow      = "\033[33m";  // Numbers, Boolean
    const char* magenta     = "\033[35m";  // Date (unused)
    const char* underline   = "\033[4m";   // Module (unused)
    const char* green       = "\033[32m";  // String
    const char* cyan        = "\033[36m";  // Functions/Userdata
    const char* red         = "\033[31m";  // Errors
    const char* bold        = "\033[1m";   // nil
    const char* grey        = "\033[90m";  // Undefined

    std::string format(lua_State* L, int idx, int depth = 0) {
        int type = lua_type(L, idx);
        std::ostringstream out;

        switch (type) {
        case LUA_TTABLE: {
            out << "{\n";
            int absIndex = idx > 0 ? idx : lua_gettop(L) + idx + 1;
            lua_pushnil(L);
            while (lua_next(L, absIndex)) {
                int keyIndex = lua_gettop(L) - 1;
                int valueIndex = lua_gettop(L);
                for (int i = 0; i < depth + 1; i++) out << "  ";
                int keyType = lua_type(L, keyIndex);
                switch (keyType) {
                case LUA_TSTRING:
                    out << lua_tostring(L, keyIndex);
                    break;
                case LUA_TNUMBER:
                    out << lua_tonumber(L, keyIndex);
                    break;
                default:
                    out << lua_typename(L, keyType);
                    break;
                }
                out << ": " << format(L, valueIndex, depth + 1) << ",\n";
                lua_pop(L, 1);
            }
            for (int i = 0; i < depth; i++) out << "  ";
            out << "}";
            break;
        }
        case LUA_TSTRING:
            out << green << "\"" << lua_tostring(L, idx) << "\"" << reset;
            break;
        case LUA_TNUMBER:
            out << yellow << lua_tostring(L, idx) << reset;
            break;
        case LUA_TBOOLEAN:
            out << yellow << (lua_toboolean(L, idx) ? "true" : "false") << reset;
            break;
        case LUA_TNIL:
            out << bold << "nil" << reset;
            break;
        case LUA_TFUNCTION:
        case LUA_TUSERDATA:
        case LUA_TLIGHTUSERDATA:
            out << cyan << lua_typename(L, type) << reset;
            break;
        default:
            out << grey << lua_tostring(L, idx) << reset;
            break;
        }

        return out.str();
    }

    int print(lua_State* L, LOG_LEVEL level) {
        int nargs = lua_gettop(L);
        std::ostringstream out;

        for (int i = 1; i <= nargs; i++) {
            out << format(L, i, 0);
            if (i < nargs) out << " ";
        }

        std::string msg = out.str();

        switch (level) {
            case LOG_LEVEL_INFO: {
                SDL_LogInfo(LOG_CATEGORY_LUA, "%s", msg.c_str());
                break;
            }
            case LOG_LEVEL_WARN: {
                SDL_LogWarn(LOG_CATEGORY_LUA, "%s", msg.c_str());
                break;
            }
            case LOG_LEVEL_ERROR: {
                SDL_LogError(LOG_CATEGORY_LUA, "%s", msg.c_str());
                break;
            }
            default: {
                SDL_LogInfo(LOG_CATEGORY_LUA, "%s", msg.c_str());
                break;
            }
        }

        return 0;
    }

    int console_log(lua_State* L) {
        return print(L, LOG_LEVEL_INFO);
    }
    int console_warn(lua_State* L) {
        return print(L, LOG_LEVEL_WARN);
    }
    int console_error(lua_State* L) {
        return print(L, LOG_LEVEL_ERROR);
    }
}

void registerConsole(lua_State* L) {
    lua_newtable(L);

    lua_pushcfunction(L, console_log);
    lua_setfield(L, -2, "log");

    lua_pushcfunction(L, console_warn);
    lua_setfield(L, -2, "warn");

    lua_pushcfunction(L, console_error);
    lua_setfield(L, -2, "error");

    lua_setglobal(L, "console");

    //Override built-in and alias it to console.log
    lua_pushcfunction(L, console_log);
    lua_setglobal(L, "print");
}