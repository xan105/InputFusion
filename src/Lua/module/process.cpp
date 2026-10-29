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

#include "process.h"
#include <shellapi.h>
#include <string>
#include "../../util.h"

namespace {

    int CreateTable_Args(lua_State* L) {
        int nArgs = 0;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &nArgs);

        if (!argv) {
            lua_newtable(L);
            return 1;
        }

        lua_createtable(L, nArgs, 0);

        for (int i = 0; i < nArgs; ++i) {
            std::string arg = util::WStringToUtf8(argv[i]);

            lua_pushlstring(L, arg.data(), arg.size());
            lua_rawseti(L, -2, i + 1);
        }

        LocalFree(argv);
        return 1;
    }

    int CreateTable_Env(lua_State* L) {
        lua_newtable(L);

        LPWCH env = GetEnvironmentStringsW();
        if (!env) {
            return 1;
        }

        LPWCH pEnv = env;

        while (*env) {
            std::wstring str(env);
            size_t pos = str.find(L'=');

            if (pos != std::wstring::npos && pos > 0) {
                std::wstring key = str.substr(0, pos);
                std::wstring val = str.substr(pos + 1);

                std::string keyUtf8 = util::WStringToUtf8(key);
                std::string valUtf8 = util::WStringToUtf8(val);

                lua_pushlstring(L, valUtf8.data(), valUtf8.size());
                lua_setfield(L, -2, keyUtf8.c_str());
            }

            env += str.size() + 1;
        }

        FreeEnvironmentStringsW(pEnv);
        return 1;
    }
}

LUALIB_API int luaopen_process(lua_State* L) {

    lua_newtable(L);

    lua_pushinteger(L, GetCurrentProcessId());
    lua_setfield(L, -2, "pid");

    lua_pushstring(L, util::WStringToUtf8(util::GetCurrentProcessName()).c_str());
    lua_setfield(L, -2, "bin");

    lua_pushstring(L, util::WStringToUtf8(util::GetModuleDirectory()).c_str());
    lua_setfield(L, -2, "dir");

    lua_pushstring(L, util::WStringToUtf8(util::GetCurrentWorkingDir()).c_str());
    lua_setfield(L, -2, "cwd");

    CreateTable_Args(L);
    lua_setfield(L, -2, "args");

    CreateTable_Env(L);
    lua_setfield(L, -2, "env");

    return 1;
}