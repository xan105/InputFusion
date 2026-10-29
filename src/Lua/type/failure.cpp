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

#include "failure.h"
#include <string>
#include <sstream>

namespace {

    struct Failure {
        std::string code;
        std::string message;
    };

    int constructor(lua_State* L) {
        const char* code = luaL_optstring(L, 1, "ERR_UNKNOWN");
        const char* message = luaL_optstring(L, 2, "An unknown error occurred");
        lua_pushFailure(L, code, message);
        return 1;
    }

    int index(lua_State* L) {
        Failure* f = (Failure*)luaL_checkudata(L, 1, "Failure");
        const char* key = luaL_checkstring(L, 2);

        if (strcmp(key, "code") == 0) {
            lua_pushstring(L, f->code.c_str());
        }
        else if (strcmp(key, "message") == 0) {
            lua_pushstring(L, f->message.c_str());
        }
        else {
            lua_pushnil(L);
        }
        return 1;
    }

    int tostring(lua_State* L) {
        Failure* f = (Failure*)luaL_checkudata(L, 1, "Failure");
        std::ostringstream ss;
        ss << "[" << f->code << "]: " << f->message;
        lua_pushstring(L, ss.str().c_str());
        return 1;
    }

    int gc(lua_State* L) {
        Failure* f = (Failure*)luaL_checkudata(L, 1, "Failure");
        f->~Failure();
        return 0;
    }

}

void lua_pushFailure(lua_State* L, const char* code, const char* message) {
    Failure* f = (Failure*)lua_newuserdata(L, sizeof(Failure));
    new(f) Failure{code, message};

    luaL_getmetatable(L, "Failure");
    lua_setmetatable(L, -2);
}

void registerFailureType(lua_State* L) {
    luaL_newmetatable(L, "Failure");

    lua_pushcfunction(L, index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, tostring);
    lua_setfield(L, -2, "__tostring");

    lua_pushcfunction(L, gc);
    lua_setfield(L, -2, "__gc");

    lua_pushstring(L, "Protected metatable!");
    lua_setfield(L, -2, "__metatable");

    lua_pushcfunction(L, constructor);
    lua_setfield(L, -2, "__call");

    lua_pushvalue(L, -1); 
    lua_setglobal(L, "Failure");
}