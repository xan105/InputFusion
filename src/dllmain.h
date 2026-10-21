/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

DWORD WINAPI Main(LPVOID lpReserved);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);