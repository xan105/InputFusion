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

#include "lua.h"
#include <filesystem>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "../flags.h"
#include "../util.h"
#include "type/failure.h"
#include "global/console.h"

namespace {
    lua_State* L = nullptr;

    void load_std_libs(lua_State* L) {
        lua_pushcfunction(L, luaopen_package);
        lua_call(L, 0, 0);

        lua_pushcfunction(L, luaopen_base);
        lua_call(L, 0, 0);

        lua_pushcfunction(L, luaopen_table);
        lua_call(L, 0, 0);

        lua_pushcfunction(L, luaopen_string);
        lua_call(L, 0, 0);

        lua_pushcfunction(L, luaopen_math);
        lua_call(L, 0, 0);

        //LuaJIT-specific
        lua_pushcfunction(L, luaopen_bit);
        lua_call(L, 0, 0);

        lua_pushcfunction(L, luaopen_jit);
        lua_call(L, 0, 0);
    }
}

void InputFusion_Lua_Init(){
    if (Flags().lua_file.empty()) return;
    SDL_LogInfo(LOG_CATEGORY_LUA, "Loading LuaJIT");

    std::wstring lua_file = Flags().lua_file;
    if (!std::filesystem::path(lua_file).is_absolute()) {
        lua_file = util::GetModuleDirectory() + lua_file;
    }

    L = luaL_newstate();
    load_std_libs(L);

    // Custom type
    registerFailureType(L);
    // Extend Globals
    registerConsole(L);



    SDL_LogInfo(LOG_CATEGORY_LUA, "Starting Lua file [\"%s\"]", util::WStringToUtf8(lua_file).c_str());
    if (luaL_dofile(L, util::WStringToUtf8(lua_file).c_str()) != LUA_OK) {
        std::string error = lua_tostring(L, -1);
        SDL_LogError(LOG_CATEGORY_LUA, "%s", error.c_str());
        MessageBoxA(NULL, error.c_str(), "Lua Error", MB_ICONERROR | MB_OK);
        lua_pop(L, 1);
        InputFusion_Lua_Quit();
        return;
    }
}

void InputFusion_Lua_Quit() {
    SDL_LogInfo(LOG_CATEGORY_LUA, "lua_close");
    if (L != nullptr) {
        lua_close(L);
    }
}