/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "flags.h"
#include <SDL3/SDL.h>
#include "util.h"
#include "../vendor/SimpleINI/SimpleIni.h"

namespace {
    void SetFlagFromEnv(bool& flag, const wchar_t* name, const wchar_t* value)
    {
        const std::wstring env = util::GetEnvVar(name);
        if (env.empty()) return;
        flag = (env == value);
    }
}

const Flags_t& Flags() {
    static const Flags_t flags = [] {
        Flags_t flag{};

        CSimpleIniW ini;
        ini.SetUnicode();
        ini.SetQuotes(true);

        std::wstring config_file = util::GetEnvVar(L"INPUTFUSION_CONFIG_FILE");
        if (config_file.empty()) config_file = util::GetModuleDirectory() + L"InputFusion.ini";

        if (ini.LoadFile(config_file.c_str()) == SI_OK) {
            flag.ledbatterylvl              = ini.GetBoolValue(L"SDL",      L"GAMEPAD_LED_BATTERY_LVL",     false);
            flag.norumble                   = ini.GetBoolValue(L"SDL",      L"DISABLE_GAMEPAD_RUMBLE",      false);
            flag.winmm_layout_xbox          = ini.GetBoolValue(L"WinMM",    L"XBOX_LAYOUT",                 false);
            flag.xinput_always_connected    = ini.GetBoolValue(L"XInput",   L"DEVICE_ALWAYS_CONNECTED",     false);
            flag.xinput_realID              = ini.GetBoolValue(L"XInput",   L"DEVICE_VID_PID",              false);
            flag.dinput_nodevices           = ini.GetBoolValue(L"DInput8",  L"NO_DEVICES",                  false);
            flag.gameinput_detour           = ini.GetBoolValue(L"API",      L"GAMEINPUT",                   false);
            flag.xinput_detour              = ini.GetBoolValue(L"API",      L"XINPUT",                      false);
            flag.dinput8_detour             = ini.GetBoolValue(L"API",      L"DINPUT8",                     false);
            flag.dinput_detour              = ini.GetBoolValue(L"API",      L"DINPUT",                      false);
            flag.winmm_detour               = ini.GetBoolValue(L"API",      L"WINMM",                       false);

            flag.lua_file                   = ini.GetValue(L"Lua", L"file", L"");
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Parsed flags from [\"%s\"]", util::WStringToUtf8(config_file).c_str());
        } else {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to parse flags from [\"%s\"]", util::WStringToUtf8(config_file).c_str());
        }

        SetFlagFromEnv(flag.ledbatterylvl,              L"GAMEPAD_LED",                         L"BATTERYLVL");
        SetFlagFromEnv(flag.norumble,                   L"GAMEPAD_RUMBLE",                      L"DISABLE");
        SetFlagFromEnv(flag.winmm_layout_xbox,          L"GAMEPAD_API_WINMM_LAYOUT",            L"XBOX");
        SetFlagFromEnv(flag.xinput_always_connected,    L"GAMEPAD_API_XINPUT_DEVICE_CONNECTED", L"ALWAYS");
        SetFlagFromEnv(flag.xinput_realID,              L"GAMEPAD_API_XINPUT_VID_PID",          L"DEVICE");
        SetFlagFromEnv(flag.dinput_nodevices,           L"GAMEPAD_API_DINPUT8_NODEVICES",       L"ENABLE");
        SetFlagFromEnv(flag.gameinput_detour,           L"GAMEPAD_API_GAMEINPUT",               L"HOOK");
        SetFlagFromEnv(flag.xinput_detour,              L"GAMEPAD_API_XINPUT",                  L"HOOK");
        SetFlagFromEnv(flag.dinput8_detour,             L"GAMEPAD_API_DINPUT8",                 L"HOOK");
        SetFlagFromEnv(flag.dinput_detour,              L"GAMEPAD_API_DINPUT",                  L"HOOK");
        SetFlagFromEnv(flag.winmm_detour,               L"GAMEPAD_API_WINMM",                   L"HOOK");

        const std::wstring lua_file = util::GetEnvVar(L"INPUTFUSION_LUA_FILE");
        if (!lua_file.empty()) {
            flag.lua_file = lua_file;
        }

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Parsed flags from env. var.");
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "Flags:\n"
            "- ledbatterylvl: \t\t%d\n"
            "- norumble: \t\t\t%d\n"
            "- winmm_layout_xbox: \t\t%d\n"
            "- xinput_always_connected: \t%d\n"
            "- xinput_realID: \t\t%d\n"
            "- dinput_nodevices: \t\t%d\n"
            "- gameinput_detour: \t\t%d\n"
            "- xinput_detour: \t\t%d\n"
            "- dinput8_detour: \t\t%d\n"
            "- dinput_detour: \t\t%d\n"
            "- winmm_detour: \t\t%d\n"
            "- lua_file: [\"%s\"]\n",
            flag.ledbatterylvl,
            flag.norumble,
            flag.winmm_layout_xbox,
            flag.xinput_always_connected,
            flag.xinput_realID,
            flag.dinput_nodevices,
            flag.gameinput_detour,
            flag.xinput_detour,
            flag.dinput8_detour,
            flag.dinput_detour,
            flag.winmm_detour,
            util::WStringToUtf8(flag.lua_file).c_str()
        );

        return flag;
    }();
    return flags;
}