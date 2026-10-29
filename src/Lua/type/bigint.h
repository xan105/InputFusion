/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
extern "C" {
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
}
#include <cstdint>
#include <type_traits>

namespace {

    template<typename T>
    struct Box {
        T value;
    };

    template<typename T>
    Box<T>* check(lua_State* L, int index, const char* mt);

    template<typename T>
    T checked_add(T a, T b);

    template<typename T>
    T checked_sub(T a, T b);

    template<typename T>
    T checked_mul(T a, T b);

    template<typename T>
    T checked_div(T a, T b);

    template<typename T>
    T checked_mod(T a, T b);

    template<typename T>
    T checked_pow(T base, T exp);

    template<typename T>
    int constructor(lua_State* L, const char* mt);

    int int_ctor(lua_State* L);

    int uint_ctor(lua_State* L);

    template<typename T>
    int arithmetic(lua_State* L, const char* mt, char op);

    template<typename T>
    int add(lua_State* L);
    template<typename T>
    int sub(lua_State* L);
    template<typename T>
    int mul(lua_State* L);
    template<typename T>
    int div(lua_State* L);
    template<typename T>
    int mod(lua_State* L);
    template<typename T>
    int pow(lua_State* L);

    template<typename T>
    int unary_minus(lua_State* L);

    template<typename T>
    int eq(lua_State* L);

    template<typename T>
    int lt(lua_State* L);

    template<typename T>
    int le(lua_State* L);

    template<typename T>
    int tostring(lua_State* L);

    template<typename T>
    void register_type(lua_State* L, const char* mt, lua_CFunction ctor);
}

void lua_pushint64(lua_State* L, int64_t value);
void lua_pushuint64(lua_State* L, uint64_t value);
int64_t luaL_checkint64(lua_State* L, int arg);
uint64_t luaL_checkuint64(lua_State* L, int arg);
void registerINT64(lua_State* L);
void registerUINT64(lua_State* L);