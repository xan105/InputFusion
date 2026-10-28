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

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
extern "C" {
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
}
#include <string>

namespace {

    typedef enum LOG_LEVEL {
        LOG_LEVEL_INFO = 0,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR
    } LOG_LEVEL;

    std::string format(lua_State* L, int idx, int depth);
    int print(lua_State* L, LOG_LEVEL level);
    int console_log(lua_State* L);
    int console_warn(lua_State* L);
    int console_error(lua_State* L);
    bool isUserdataType(lua_State* L, int idx, const char* name);
}

void registerConsole(lua_State* L);