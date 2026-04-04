/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "flags.h"
#include "util.h"

const Flags_t& Flags() {
    static Flags_t flags = [] {
        Flags_t f{};
        f.ledbatterylvl               =   Getenv(L"GAMEPAD_LED") == L"BATTERYLVL";
        f.winmm_layout_xbox           =   Getenv(L"GAMEPAD_API_WINMM_LAYOUT") == L"XBOX";
        f.xinput_always_connected     =   Getenv(L"GAMEPAD_API_XINPUT_DEVICE_CONNECTED") == L"ALWAYS";
        f.gameinput_detour            =   Getenv(L"GAMEPAD_API_GAMEINPUT") == L"HOOK";
        f.xinput_detour               =   Getenv(L"GAMEPAD_API_XINPUT") == L"HOOK";
        f.dinput8_detour              =   Getenv(L"GAMEPAD_API_DINPUT8") == L"HOOK";
        f.dinput_detour               =   Getenv(L"GAMEPAD_API_DINPUT") == L"HOOK";
        f.winmm_detour                =   Getenv(L"GAMEPAD_API_WINMM") == L"HOOK";
        return f;
    }();
    return flags;
}