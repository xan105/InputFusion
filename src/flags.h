/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN 
#include <SDL3/SDL.h>

struct Flags_t {
    bool ledbatterylvl              = false;
    bool norumble                   = false;
    bool winmm_layout_xbox          = false;
    bool xinput_always_connected    = false;
    bool xinput_realID              = false;
    bool dinput_nodevices           = false;
    bool gameinput_detour           = false;
    bool xinput_detour              = false;
    bool dinput8_detour             = false;
    bool dinput_detour              = false;
    bool winmm_detour               = false;
};

namespace {
    void SetFlagFromEnv(bool& flag, const wchar_t* name, const wchar_t* value);
}

const Flags_t& Flags();