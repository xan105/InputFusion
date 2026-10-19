/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "flags.h"
#include "util.h"
#include "../vendor/SimpleINI/SimpleIni.h"

static void SetFlagFromEnv(bool& flag, const wchar_t* name, const wchar_t* value)
{
    const std::wstring env = Getenv(name);
    if (env.empty()) return;
    flag = (env == value);
}

const Flags_t& Flags() {
    static const Flags_t flags = [] {
        Flags_t flag{};

        CSimpleIniW ini;
        ini.SetUnicode();

        std::wstring config_file = Getenv(L"INPUTFUSION_CONFIG_FILE_PATH");
        std::wstring default_file = L"InputFusion.ini";
        if (config_file.empty()) config_file = GetSelfDir() + default_file;

        if (ini.LoadFile(config_file.c_str()) == SI_OK) {
            flag.ledbatterylvl              = ini.GetBoolValue(L"SDL",      L"GAMEPAD_LED_BATTERY_LVL",     false);
            flag.norumble                   = ini.GetBoolValue(L"SDL",      L"DISABLE_GAMEPAD_RUMBLE",      false);
            flag.delegate                   = ini.GetBoolValue(L"SDL",      L"DELEGATE",                    false);
            flag.winmm_layout_xbox          = ini.GetBoolValue(L"WinMM",    L"XBOX_LAYOUT",                 false);
            flag.xinput_always_connected    = ini.GetBoolValue(L"XInput",   L"DEVICE_ALWAYS_CONNECTED",     false);
            flag.dinput_nodevices           = ini.GetBoolValue(L"DInput8",  L"NO_DEVICES",                  false);
            flag.gameinput_detour           = ini.GetBoolValue(L"API",      L"GAMEINPUT",                   false);
            flag.xinput_detour              = ini.GetBoolValue(L"API",      L"XINPUT",                      false);
            flag.dinput8_detour             = ini.GetBoolValue(L"API",      L"DINPUT8",                     false);
            flag.dinput_detour              = ini.GetBoolValue(L"API",      L"DINPUT",                      false);
            flag.winmm_detour               = ini.GetBoolValue(L"API",      L"WINMM",                       false);
        }

        SetFlagFromEnv(flag.ledbatterylvl,              L"GAMEPAD_LED",                         L"BATTERYLVL");
        SetFlagFromEnv(flag.norumble,                   L"GAMEPAD_RUMBLE",                      L"DISABLE");
        SetFlagFromEnv(flag.delegate,                   L"GAMEPAD_API_DELEGATE",                L"ENABLE");
        SetFlagFromEnv(flag.winmm_layout_xbox,          L"GAMEPAD_API_WINMM_LAYOUT",            L"XBOX");
        SetFlagFromEnv(flag.xinput_always_connected,    L"GAMEPAD_API_XINPUT_DEVICE_CONNECTED", L"ALWAYS");
        SetFlagFromEnv(flag.dinput_nodevices,           L"GAMEPAD_API_DINPUT8_NODEVICES",       L"ENABLE");
        SetFlagFromEnv(flag.gameinput_detour,           L"GAMEPAD_API_GAMEINPUT",               L"HOOK");
        SetFlagFromEnv(flag.xinput_detour,              L"GAMEPAD_API_XINPUT",                  L"HOOK");
        SetFlagFromEnv(flag.dinput8_detour,             L"GAMEPAD_API_DINPUT8",                 L"HOOK");
        SetFlagFromEnv(flag.dinput_detour,              L"GAMEPAD_API_DINPUT",                  L"HOOK");
        SetFlagFromEnv(flag.winmm_detour,               L"GAMEPAD_API_WINMM",                   L"HOOK");

        SDL_Log("Flags:\n"
            "- ledbatterylvl: %d\n"
            "- norumble: %d\n"
            "- delegate: %d\n"
            "- winmm_layout_xbox: %d\n"
            "- xinput_always_connected: %d\n"
            "- dinput_nodevices: %d\n"
            "- gameinput_detour: %d\n"
            "- xinput_detour: %d\n"
            "- dinput8_detour: %d\n"
            "- dinput_detour: %d\n"
            "- winmm_detour: %d\n",
            flag.ledbatterylvl,
            flag.norumble,
            flag.delegate,
            flag.winmm_layout_xbox,
            flag.xinput_always_connected,
            flag.dinput_nodevices,
            flag.gameinput_detour,
            flag.xinput_detour,
            flag.dinput8_detour,
            flag.dinput_detour,
            flag.winmm_detour
        );

        return flag;
    }();
    return flags;
}