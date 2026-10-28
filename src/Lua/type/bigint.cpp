/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "bigint.h"
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <limits>
#include <string>

namespace {

    using I64 = std::int64_t;
    using U64 = std::uint64_t;

    const char* INT_MT = "Int64";
    const char* UINT_MT = "Uint64";

    template<typename T>
    Box<T>* check(lua_State* L, int index, const char* mt) {
        return static_cast<Box<T>*>(luaL_checkudata(L, index, mt));
    }

    template<typename T>
    T checked_add(T a, T b) {
        if constexpr (std::is_signed<T>::value) {
            if ((b > 0 && a > std::numeric_limits<T>::max() - b) ||
                (b < 0 && a < std::numeric_limits<T>::min() - b))
                throw "Integer overflow";
        } else {
            if (a > std::numeric_limits<T>::max() - b)
                throw "Integer overflow";
        }
        return a + b;
    }

    template<typename T>
    T checked_sub(T a, T b) {
        if constexpr (std::is_signed<T>::value) {
            if ((b < 0 && a > std::numeric_limits<T>::max() + b) ||
                (b > 0 && a < std::numeric_limits<T>::min() + b))
                throw "Integer overflow";
        } else {
            if (a < b)
                throw "Integer underflow";
        }
        return a - b;
    }

    template<typename T>
    T checked_mul(T a, T b) {
        if (a == 0 || b == 0)
            return 0;

        if constexpr (std::is_signed<T>::value) {
            const T lo = std::numeric_limits<T>::min();
            const T hi = std::numeric_limits<T>::max();

            if ((a == -1 && b == lo) || (b == -1 && a == lo))
                throw "Integer overflow";

            if (a > 0) {
                if (b > 0 ? a > hi / b : b < lo / a)
                    throw "Integer overflow";
            } else {
                if (b > 0 ? a < lo / b : a < hi / b)
                    throw "Integer overflow";
            }
        } else {
            if (a > std::numeric_limits<T>::max() / b)
                throw "Integer overflow";
        }

        return a * b;
    }

    template<typename T>
    T checked_div(T a, T b) {
        if (b == 0)
            throw "Division by zero";

        if constexpr (std::is_signed<T>::value) {
            if (a == std::numeric_limits<T>::min() && b == -1)
                throw "Integer overflow";
        }

        return a / b;
    }

    template<typename T>
    T checked_mod(T a, T b) {
        if (b == 0)
            throw "Division by zero";

        // Avoid the signed MIN % -1 overflow case.
        if constexpr (std::is_signed<T>::value) {
            if (a == std::numeric_limits<T>::min() && b == -1)
                return 0;
        }

        return a % b;
    }

    template<typename T>
    T checked_pow(T base, T exp) {
        if constexpr (std::is_signed<T>::value) {
            if (exp < 0)
                throw "Negative integer exponent";
        }

        T result = 1;
        while (exp != 0) {
            if (exp % 2 != 0)
                result = checked_mul(result, base);
            exp /= 2;
            if (exp != 0)
                base = checked_mul(base, base);
        }
        return result;
    }

    template<typename T>
    int constructor(lua_State* L, const char* mt) {
        T value{};

        if (lua_type(L, 1) == LUA_TSTRING) {
            const char* s = lua_tostring(L, 1);
            char* end = nullptr;
            errno = 0;

            if constexpr (std::is_signed<T>::value) {
                long long v = std::strtoll(s, &end, 10);
                if (errno == ERANGE || end == s || *end != '\0')
                    return luaL_error(L, "Invalid Int64 decimal string");
                value = static_cast<T>(v);
            } else {
                if (*s == '-')
                    return luaL_error(L, "Uint64 cannot be negative");

                unsigned long long v = std::strtoull(s, &end, 10);
                if (errno == ERANGE || end == s || *end != '\0')
                    return luaL_error(L, "Invalid Uint64 decimal string");
                value = static_cast<T>(v);
            }
        } else if (lua_isnumber(L, 1)) {
            // Numeric Lua literals may already have lost precision above 2^53.
            lua_Number n = lua_tonumber(L, 1);
            long double x = static_cast<long double>(n);

            if (!std::isfinite(static_cast<double>(n)) ||
                std::floor(x) != x)
                return luaL_error(L, "Expected a finite integer");

            if constexpr (std::is_signed<T>::value) {
                if (x < static_cast<long double>(
                            std::numeric_limits<T>::min()) ||
                    x > static_cast<long double>(
                            std::numeric_limits<T>::max()))
                    return luaL_error(L, "Int64 out of range");
            } else {
                if (x < 0 ||
                    x > static_cast<long double>(
                            std::numeric_limits<T>::max()))
                    return luaL_error(L, "Uint64 out of range");
            }

            value = static_cast<T>(n);
        } else {
            return luaL_error(L, "Expected integer or decimal string");
        }

        auto* box = static_cast<Box<T>*>(
            lua_newuserdata(L, sizeof(Box<T>)));
        box->value = value;

        luaL_getmetatable(L, mt);
        lua_setmetatable(L, -2);
        return 1;
    }

    int int_ctor(lua_State* L) {
        return constructor<I64>(L, INT_MT);
    }

    int uint_ctor(lua_State* L) {
        return constructor<U64>(L, UINT_MT);
    }

    template<typename T>
    int arithmetic(lua_State* L, const char* mt, char op) {
        auto* a = check<T>(L, 1, mt);
        auto* b = check<T>(L, 2, mt);

        try {
            T r{};
            switch (op) {
            case '+': r = checked_add(a->value, b->value); break;
            case '-': r = checked_sub(a->value, b->value); break;
            case '*': r = checked_mul(a->value, b->value); break;
            case '/': r = checked_div(a->value, b->value); break;
            case '%': r = checked_mod(a->value, b->value); break;
            case '^': r = checked_pow(a->value, b->value); break;
            default: return luaL_error(L, "Unknown arithmetic operator");
            }

            auto* result = static_cast<Box<T>*>(
                lua_newuserdata(L, sizeof(Box<T>)));
            result->value = r;
            luaL_getmetatable(L, mt);
            lua_setmetatable(L, -2);
            return 1;
        } catch (const char* error) {
            return luaL_error(L, error);
        }
    }

    template<typename T>
    int add(lua_State* L) {
        return arithmetic<T>(L, std::is_signed<T>::value ? INT_MT : UINT_MT, '+');
    }
    template<typename T>
    int sub(lua_State* L) {
        return arithmetic<T>(L, std::is_signed<T>::value ? INT_MT : UINT_MT, '-');
    }
    template<typename T>
    int mul(lua_State* L) {
        return arithmetic<T>(L, std::is_signed<T>::value ? INT_MT : UINT_MT, '*');
    }
    template<typename T>
    int div(lua_State* L) {
        return arithmetic<T>(L, std::is_signed<T>::value ? INT_MT : UINT_MT, '/');
    }
    template<typename T>
    int mod(lua_State* L) {
        return arithmetic<T>(L, std::is_signed<T>::value ? INT_MT : UINT_MT, '%');
    }
    template<typename T>
    int pow(lua_State* L) {
        return arithmetic<T>(L, std::is_signed<T>::value ? INT_MT : UINT_MT, '^');
    }

    template<typename T>
    int unary_minus(lua_State* L) {
        const char* mt = std::is_signed<T>::value ? INT_MT : UINT_MT;
        auto* a = check<T>(L, 1, mt);

        if constexpr (!std::is_signed<T>::value) {
            return luaL_error(L, "Unary minus is not supported for Uint64");
        } else {
            if (a->value == std::numeric_limits<T>::min())
                return luaL_error(L, "Integer overflow");

            auto* result = static_cast<Box<T>*>(
                lua_newuserdata(L, sizeof(Box<T>)));
            result->value = -a->value;
            luaL_getmetatable(L, mt);
            lua_setmetatable(L, -2);
            return 1;
        }
    }

    template<typename T>
    int eq(lua_State* L) {
        const char* mt = std::is_signed<T>::value ? INT_MT : UINT_MT;
        return check<T>(L, 1, mt)->value == check<T>(L, 2, mt)->value;
    }

    template<typename T>
    int lt(lua_State* L) {
        const char* mt = std::is_signed<T>::value ? INT_MT : UINT_MT;
        return check<T>(L, 1, mt)->value < check<T>(L, 2, mt)->value;
    }

    template<typename T>
    int le(lua_State* L) {
        const char* mt = std::is_signed<T>::value ? INT_MT : UINT_MT;
        return check<T>(L, 1, mt)->value <= check<T>(L, 2, mt)->value;
    }

    template<typename T>
    int tostring(lua_State* L) {
        const char* mt = std::is_signed<T>::value ? INT_MT : UINT_MT;
        T v = check<T>(L, 1, mt)->value;

        std::string s;
        if constexpr (std::is_signed<T>::value)
            s = std::to_string(static_cast<long long>(v));
        else
            s = std::to_string(static_cast<unsigned long long>(v));

        lua_pushlstring(L, s.c_str(), s.size());
        return 1;
    }

    template<typename T>
    void register_type(lua_State* L, const char* mt, lua_CFunction ctor) {
        luaL_newmetatable(L, mt);

        lua_pushcfunction(L, add<T>);
        lua_setfield(L, -2, "__add");
        lua_pushcfunction(L, sub<T>);
        lua_setfield(L, -2, "__sub");
        lua_pushcfunction(L, mul<T>);
        lua_setfield(L, -2, "__mul");
        lua_pushcfunction(L, div<T>);
        lua_setfield(L, -2, "__div");
        lua_pushcfunction(L, mod<T>);
        lua_setfield(L, -2, "__mod");
        lua_pushcfunction(L, pow<T>);
        lua_setfield(L, -2, "__pow");
        lua_pushcfunction(L, unary_minus<T>);
        lua_setfield(L, -2, "__unm");

        lua_pushcfunction(L, eq<T>);
        lua_setfield(L, -2, "__eq");
        lua_pushcfunction(L, lt<T>);
        lua_setfield(L, -2, "__lt");
        lua_pushcfunction(L, le<T>);
        lua_setfield(L, -2, "__le");

        lua_pushcfunction(L, tostring<T>);
        lua_setfield(L, -2, "__tostring");

        lua_pushliteral(L, "protected");
        lua_setfield(L, -2, "__metatable");

        lua_pop(L, 1);

        lua_pushcfunction(L, ctor);
        lua_setglobal(L, mt);
    }
}

int lua_pushint64(lua_State* L, int64_t value)
{
    auto* box = static_cast<Box<I64>*>(lua_newuserdata(L, sizeof(Box<I64>)));
    box->value = value;

    luaL_getmetatable(L, INT_MT);
    lua_setmetatable(L, -2);
    
    return 1;
}

int lua_pushuint64(lua_State* L, uint64_t value)
{
    auto* box = static_cast<Box<U64>*>(lua_newuserdata(L, sizeof(Box<U64>)));
    box->value = value;

    luaL_getmetatable(L, UINT_MT);
    lua_setmetatable(L, -2);
    
    return 1;
}

int64_t luaL_checkint64(lua_State* L, int arg) {
    auto* box = check<I64>(L, arg, INT_MT);
    return box->value;
}

uint64_t luaL_checkuint64(lua_State* L, int arg) {
    auto* box = check<U64>(L, arg, UINT_MT);
    return box->value;
}

void registerINT64(lua_State* L) {
  register_type<I64>(L, INT_MT, int_ctor);
}

void registerUINT64(lua_State* L) {
  register_type<U64>(L, UINT_MT, uint_ctor);
}