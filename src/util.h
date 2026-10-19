/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <string>
#include <sstream>
#include <iomanip>

void enableConsole();
std::wstring Getenv(LPCWSTR name);
std::wstring GetSelfDir();
std::string GUIDToString(REFIID riid);
std::wstring GetModuleFileNameWString(HMODULE hMod);
