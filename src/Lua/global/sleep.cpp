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

#include "sleep.h"
#include <chrono>
#include <thread>

namespace {
    int sleep(lua_State *L) {
        lua_Integer ms = luaL_checkinteger(L, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        return 0;
    }
}

void registerSleep(lua_State* L) {
    lua_register(L, "sleep", sleep);
}