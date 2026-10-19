/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <string>
#include <mutex>
#include <SDL3/SDL.h>

static HMODULE LoadSystemLibraryW(const wchar_t* moduleName);
static HMODULE GetSelfModule();
static void delegateGameInput();
static void delegateXInput();
static void delegateDInput8();

bool IsSDL3(void* returnAddress);
void delegateSDLInputCalls();