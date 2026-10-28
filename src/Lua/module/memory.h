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
#include <vector>
#include <psapi.h>

namespace {
	namespace Memory {
		bool Patch(uintptr_t address, const std::vector<BYTE>& patch, HANDLE hProcess);
		template <typename T> T FindPattern(uintptr_t baseAddress, size_t sizeOfImage, const std::vector<int>& pattern);
		template <typename T> std::vector<T> ParseHexStringTo(const std::string& input);
		MODULEINFO GetModuleInfo(const std::wstring& moduleName);
	}
	template <typename T> T SafeRead(uintptr_t address);
	int MemoryWrite(lua_State* L);
	int MemoryFind(lua_State* L);
	int MemoryReadAs(lua_State* L);
	int GetBaseAddress(lua_State* L);
}

LUALIB_API int luaopen_memory(lua_State* L);