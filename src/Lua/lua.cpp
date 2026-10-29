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
#include "../log.h"
#include "../flags.h"
#include "../util.h"
#include "type/failure.h"
#include "type/bigint.h"
#include "global/console.h"
#include "global/sleep.h"
#include "global/array.h"
#include "module/memory.h"
#include "module/process.h"

namespace {
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

        lua_pushcfunction(L, luaopen_ffi);
        lua_call(L, 0, 0);

        lua_pushcfunction(L, luaopen_jit);
        lua_call(L, 0, 0);
    }

    void preloadModule(lua_State* L, const char* name, lua_CFunction func) {
        lua_getglobal(L, "package");       // Get package table
        lua_getfield(L, -1, "preload");    // Get package.preload table
        lua_pushcfunction(L, func);        // Push the module initializer function
        lua_setfield(L, -2, name);         // package.preload[module_name] = module_func
        lua_pop(L, 2);                     // Pop package and preload tables
    }
}

void InputFusion_Lua_Run(){
    if (Flags().lua_file.empty()) return;
    SDL_LogInfo(LOG_CATEGORY_LUA, "Loading LuaJIT");

    std::wstring lua_file = Flags().lua_file;
    if (!std::filesystem::path(lua_file).is_absolute()) {
        lua_file = util::GetModuleDirectory() + lua_file;
    }

    lua_State* L = luaL_newstate();
    load_std_libs(L);

    // Custom type
    registerFailureType(L);
    registerINT64(L);
    registerUINT64(L);

    // Extend Globals
    registerConsole(L);
    registerSleep(L);
    registerArray(L);

    // Custom LUA module
    preloadModule(L, "process", luaopen_process);
    preloadModule(L, "memory", luaopen_memory);
    //preloadModule(L, "SDL/gamepad", luaopen_sdl_gamepad);

    // Load and run main lua file
    SDL_LogInfo(LOG_CATEGORY_LUA, "Starting Lua file [\"%s\"]", util::WStringToUtf8(lua_file).c_str());
    if (luaL_dofile(L, util::WStringToUtf8(lua_file).c_str()) != LUA_OK) {
        std::string error = lua_tostring(L, -1);
        lua_pop(L, 1);
        lua_close(L);
        SDL_LogError(LOG_CATEGORY_LUA, "%s", error.c_str());
        MessageBoxA(NULL, error.c_str(), "Lua Error", MB_ICONERROR | MB_OK);
        ExitProcess(1);
    }
    lua_close(L);
}