/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "flags.h"
#include "util.h"

const Flags_t& Flags() {
    static Flags_t flags = [] {
        Flags_t flag{};
        flag.ledbatterylvl               =   Getenv(L"GAMEPAD_LED") == L"BATTERYLVL";
        flag.rumble                      =   Getenv(L"GAMEPAD_RUMBLE") == L"NONE";
        flag.winmm_layout_xbox           =   Getenv(L"GAMEPAD_API_WINMM_LAYOUT") == L"XBOX";
        flag.xinput_always_connected     =   Getenv(L"GAMEPAD_API_XINPUT_DEVICE_CONNECTED") == L"ALWAYS";
        flag.gameinput_detour            =   Getenv(L"GAMEPAD_API_GAMEINPUT") == L"HOOK";
        flag.xinput_detour               =   Getenv(L"GAMEPAD_API_XINPUT") == L"HOOK";
        flag.dinput8_detour              =   Getenv(L"GAMEPAD_API_DINPUT8") == L"HOOK";
        flag.dinput_detour               =   Getenv(L"GAMEPAD_API_DINPUT") == L"HOOK";
        flag.winmm_detour                =   Getenv(L"GAMEPAD_API_WINMM") == L"HOOK";
        return flag;
    }();
    return flags;
}