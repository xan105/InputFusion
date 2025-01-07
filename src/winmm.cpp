/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "winmm.h"

const std::unordered_map<SDL_GamepadButton, DWORD> BUTTONS = {
        {SDL_GAMEPAD_BUTTON_SOUTH, 0},
        {SDL_GAMEPAD_BUTTON_EAST, 1},
        {SDL_GAMEPAD_BUTTON_WEST, 2},
        {SDL_GAMEPAD_BUTTON_NORTH, 3},
        {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, 4},
        {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, 5},
        {SDL_GAMEPAD_BUTTON_BACK, 6},
        {SDL_GAMEPAD_BUTTON_START, 7},
        {SDL_GAMEPAD_BUTTON_LEFT_STICK, 8},
        {SDL_GAMEPAD_BUTTON_RIGHT_STICK, 9},
        {SDL_GAMEPAD_BUTTON_TOUCHPAD, 6}
};

JOYSTICK Joysticks[MAXJOY] = {};

UINT absDiff(UINT a, UINT b) {
    return a > b ? a - b : b - a;
}

void CALLBACK joystick_capture(HWND hWnd, UINT wMsg, UINT_PTR wTimer, DWORD dwTime) {
    //NB: No events are send for the pov and the axes other than X, Y, Z but they are available through joyGetPosEx()
    std::cout << "joystick_capture()" << std::endl;

    for (int i = 0; i < MAXJOY; i++)
    {
        if (Joysticks[i].capture != hWnd) continue;

        JOYINFO info = {};
        if (joyGetPos(i, &info) != JOYERR_NOERROR) continue;
        LONG pos = MAKELONG(info.wXpos, info.wYpos);

        if (!Joysticks[i].changed ||
            absDiff(Joysticks[i].info.wXpos, info.wXpos) > Joysticks[i].threshold ||
            absDiff(Joysticks[i].info.wYpos, info.wYpos) > Joysticks[i].threshold)
        {
            SendMessageA(Joysticks[i].capture, MM_JOY1MOVE + i, info.wButtons, pos);
            Joysticks[i].info.wXpos = info.wXpos;
            Joysticks[i].info.wYpos = info.wYpos;
        }

        if (!Joysticks[i].changed ||
            absDiff(Joysticks[i].info.wZpos, info.wZpos) > Joysticks[i].threshold)
        {
            SendMessageA(Joysticks[i].capture, MM_JOY1ZMOVE + i, info.wButtons, pos);
            Joysticks[i].info.wZpos = info.wZpos;
        }

        UINT buttonChange = Joysticks[i].info.wButtons ^ info.wButtons;
        if (buttonChange != 0) {
            if (info.wButtons & buttonChange) {
                SendMessageA(Joysticks[i].capture, MM_JOY1BUTTONDOWN + i, (buttonChange << 8) | (info.wButtons & buttonChange), pos);
            }
            if (Joysticks[i].info.wButtons & buttonChange) {
                SendMessageA(Joysticks[i].capture, MM_JOY1BUTTONUP + i, (buttonChange << 8) | (Joysticks[i].info.wButtons & buttonChange), pos);
            }
            Joysticks[i].info.wButtons = info.wButtons;
        }
    }
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (GetParent(hwnd) == NULL) {
        SendMessageA(hwnd, WM_USER, 0, (LPARAM)JOY_CONFIGCHANGED_MSGSTRING);
    }
    return TRUE;
}

#ifdef WINMM_EXPORTS
extern "C" {
#endif

    MMRESULT WINAPI joyConfigChanged(DWORD dwFlags) {
        std::cout << "joyConfigChanged()" << std::endl;

        if (dwFlags != 0) return JOYERR_PARMS;
        EnumWindows(EnumWindowsProc, 0);
        return JOYERR_NOERROR;
    }

    MMRESULT WINAPI joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc) {
        std::cout << "joyGetDevCapsA()" << std::endl;

        if (uJoyID > MAXJOY) return MMSYSERR_INVALPARAM;
        if (pjc == nullptr) return MMSYSERR_INVALPARAM;
        if (cbjc != sizeof(JOYCAPSA)) return JOYERR_PARMS;

        ZeroMemory(pjc, cbjc);
        strcpy_s(pjc->szRegKey, _countof(pjc->szRegKey), "DINPUT.DLL");
        if (uJoyID == ~(UINT_PTR)0) return JOYERR_NOERROR; //is -1 ?

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return MMSYSERR_NODRIVER;
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(static_cast<int>(uJoyID));
        if (gamepad == nullptr) {
            return MMSYSERR_NODRIVER;
        }

        pjc->wMid = SDL_GetGamepadVendor(gamepad) | 0x045E;
        pjc->wPid = SDL_GetGamepadProduct(gamepad) | 0x028E;
        strcpy_s(pjc->szPname, _countof(pjc->szPname), "Microsoft PC-joystick driver");
        pjc->wXmin = 0;
        pjc->wXmax = 0xffff;
        pjc->wYmin = 0;
        pjc->wYmax = 0xffff;
        pjc->wZmin = 0;
        pjc->wZmax = 0xffff;
        pjc->wNumButtons = 10;
        pjc->wPeriodMin = JOY_PERIOD_MIN;
        pjc->wPeriodMax = JOY_PERIOD_MAX;
        pjc->wRmin = 0;
        pjc->wRmax = 0;
        pjc->wUmin = 0;
        pjc->wUmax = 0;
        pjc->wVmin = 0;
        pjc->wVmax = 0;
        pjc->wCaps = JOYCAPS_HASZ | JOYCAPS_HASPOV;
        pjc->wMaxAxes = 5;
        pjc->wNumAxes = 5;
        pjc->wMaxButtons = 10;
        strcpy_s(pjc->szOEMVxD, _countof(pjc->szOEMVxD), "");

        return JOYERR_NOERROR;
    }

    MMRESULT WINAPI joyGetDevCapsW(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc) {
        std::cout << "joyGetDevCapsW()" << std::endl;

        if (uJoyID > MAXJOY) return MMSYSERR_INVALPARAM;
        if (pjc == nullptr) return MMSYSERR_INVALPARAM;
        if (cbjc != sizeof(JOYCAPSW)) return JOYERR_PARMS;

        ZeroMemory(pjc, cbjc);
        wcscpy_s(pjc->szRegKey, _countof(pjc->szRegKey), L"DINPUT.DLL");
        if (uJoyID == ~(UINT_PTR)0) return JOYERR_NOERROR; //is -1 ?

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return MMSYSERR_NODRIVER;
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(static_cast<int>(uJoyID));
        if (gamepad == nullptr) {
            return MMSYSERR_NODRIVER;
        }

        pjc->wMid = SDL_GetGamepadVendor(gamepad) | 0x045E;
        pjc->wPid = SDL_GetGamepadProduct(gamepad) | 0x028E;
        wcscpy_s(pjc->szPname, _countof(pjc->szPname), L"Microsoft PC-joystick driver");
        pjc->wXmin = 0;
        pjc->wXmax = 0xffff;
        pjc->wYmin = 0;
        pjc->wYmax = 0xffff;
        pjc->wZmin = 0;
        pjc->wZmax = 0xffff;
        pjc->wNumButtons = 10;
        pjc->wPeriodMin = JOY_PERIOD_MIN;
        pjc->wPeriodMax = JOY_PERIOD_MAX;
        pjc->wRmin = 0;
        pjc->wRmax = 0;
        pjc->wUmin = 0;
        pjc->wUmax = 0;
        pjc->wVmin = 0;
        pjc->wVmax = 0;
        pjc->wCaps = JOYCAPS_HASZ | JOYCAPS_HASPOV;
        pjc->wMaxAxes = 5;
        pjc->wNumAxes = 5;
        pjc->wMaxButtons = 10;
        wcscpy_s(pjc->szOEMVxD, _countof(pjc->szOEMVxD), L"");

        return JOYERR_NOERROR;
    }

    UINT WINAPI joyGetNumDevs(void) {
        std::cout << "joyGetNumDevs()" << std::endl;
        return MAXJOY - 1;
    }

    MMRESULT WINAPI joyGetPos(UINT uJoyID, LPJOYINFO pji) {
        std::cout << "joyGetPos()" << std::endl;

        if (uJoyID >= MAXJOY) return JOYERR_PARMS;
        if (pji == nullptr) return MMSYSERR_INVALPARAM;

        JOYINFOEX pjiEx = { 0 };
        pjiEx.dwSize = sizeof(JOYINFOEX);
        pjiEx.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNBUTTONS;

        MMRESULT result = joyGetPosEx(uJoyID, &pjiEx);
        if (result != JOYERR_NOERROR) return result;

        ZeroMemory(pji, sizeof(JOYINFO));
        pji->wXpos = pjiEx.dwXpos;
        pji->wYpos = pjiEx.dwYpos;
        pji->wZpos = pjiEx.dwZpos;
        pji->wButtons = pjiEx.dwButtons;

        return JOYERR_NOERROR;
    }

    MMRESULT WINAPI joyGetPosEx(UINT uJoyID, LPJOYINFOEX pjiEx) {
        std::cout << "joyGetPosEx()" << std::endl;

        if (uJoyID >= MAXJOY) return JOYERR_PARMS;
        if (pjiEx == nullptr) return MMSYSERR_INVALPARAM;
        if (pjiEx->dwSize != sizeof(JOYINFOEX)) return MMSYSERR_INVALPARAM;

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return MMSYSERR_NODRIVER;
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(uJoyID);
        if (gamepad == nullptr) {
            return JOYERR_UNPLUGGED;
        }

        SDL_UpdateGamepads();

        if (pjiEx->dwFlags & JOY_RETURNX) {
            bool left = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
            bool right = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
            bool up = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
            bool down = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);

            if (left && right) {
                pjiEx->dwXpos = 0x8000; //cancel opposite
            }
            else if (left && (up || down)) {
                pjiEx->dwXpos = 0x2000;
            }
            else if (right && (up || down)) {
                pjiEx->dwXpos = 0xDFFF;
            }
            else if (left) {
                pjiEx->dwXpos = 0x0;
            }
            else if (right) {
                pjiEx->dwXpos = 0xffff;
            }
            else {
                int value = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
                pjiEx->dwXpos = (DWORD)(value + 0x8000);
            }
        }
        if (pjiEx->dwFlags & JOY_RETURNY) {
            bool left = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
            bool right = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
            bool up = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
            bool down = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);

            if (up && down) {
                pjiEx->dwYpos = 0x8000; //cancel opposite
            }
            else if (up && (left || right)) {
                pjiEx->dwYpos = 0x2000;
            }
            else if (down && (left || right)) {
                pjiEx->dwYpos = 0xDFFF;
            }
            else if (up) {
                pjiEx->dwYpos = 0x0;
            }
            else if (down) {
                pjiEx->dwYpos = 0xffff;
            }
            else {
                int value = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);
                pjiEx->dwYpos = (DWORD)(value + 0x8000);
            }
        }
        if (pjiEx->dwFlags & JOY_RETURNZ) {
            int lt = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
            int rt = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
            pjiEx->dwZpos = (0x8000 + rt) - lt;
        }
        if (pjiEx->dwFlags & JOY_RETURNR) {
            pjiEx->dwRpos = 0;
        }
        if (pjiEx->dwFlags & JOY_RETURNU) {
            pjiEx->dwUpos = 0;
        }
        if (pjiEx->dwFlags & JOY_RETURNV) {
            pjiEx->dwVpos = 0;
        }

        if (pjiEx->dwFlags & JOY_RETURNPOV)
        {
            const float deadZone = 8000 / 32767.0f;

            float rX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f;
            float rY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f;
            float magnitude = std::sqrt(rX * rX + rY * rY);

            pjiEx->dwPOV = 0xFFFF;

            if (magnitude >= deadZone) {
                if (rX > 0.5f && std::abs(rY) < 0.5f) {
                    pjiEx->dwPOV = 0x2328;  // Right
                }
                else if (rX < -0.5f && std::abs(rY) < 0.5f) {
                    pjiEx->dwPOV = 0x6978;  // Left
                }
                else if (rY > 0.5f && std::abs(rX) < 0.5f) {
                    pjiEx->dwPOV = 0x4650;  // Down
                }
                else if (rY < -0.5f && std::abs(rX) < 0.5f) {
                    pjiEx->dwPOV = 0x0;  // Up
                }
                else if (rX > 0.5f && rY > 0.5f) {
                    pjiEx->dwPOV = 0x34BC;  // Down-Right
                }
                else if (rX > 0.5f && rY < -0.5f) {
                    pjiEx->dwPOV = 0x1194;  // Up-Right
                }
                else if (rX < -0.5f && rY > 0.5f) {
                    pjiEx->dwPOV = 0x57E4;  // Down-Left
                }
                else if (rX < -0.5f && rY < -0.5f) {
                    pjiEx->dwPOV = 0x7B0C;  // Up-Left
                }
            }
        }

        if (pjiEx->dwFlags & JOY_RETURNBUTTONS)
        {
            pjiEx->dwButtons = 0;
            for (const auto& [sdl_button, index] : BUTTONS) {
                if (SDL_GetGamepadButton(gamepad, sdl_button)) {
                    pjiEx->dwButtons |= 1 << index;
                }
            }
            pjiEx->dwButtonNumber = static_cast<DWORD>(std::bitset<32>(pjiEx->dwButtons).count());
        }

        return JOYERR_NOERROR;
    }

    MMRESULT WINAPI joyGetThreshold(UINT uJoyID, LPUINT puThreshold) {
        std::cout << "joyGetThreshold()" << std::endl;

        if (puThreshold == nullptr) return MMSYSERR_INVALPARAM;
        if (uJoyID > MAXJOY) return MMSYSERR_INVALPARAM;

        *puThreshold = Joysticks[uJoyID].threshold;

        return JOYERR_NOERROR;
    }

    MMRESULT WINAPI joySetThreshold(UINT uJoyID, UINT uThreshold) {
        std::cout << "joySetThreshold()" << std::endl;

        if (uJoyID > MAXJOY) return JOYERR_PARMS;

        Joysticks[uJoyID].threshold = uThreshold;

        return JOYERR_NOERROR;
    }

    MMRESULT WINAPI joyReleaseCapture(UINT uJoyID) {
        std::cout << "joyReleaseCapture()" << std::endl;

        if (uJoyID > MAXJOY) return JOYERR_PARMS;

        if (Joysticks[uJoyID].capture) {
            KillTimer(Joysticks[uJoyID].capture, Joysticks[uJoyID].timer);
            Joysticks[uJoyID].info = {};
            Joysticks[uJoyID].capture = 0;
            Joysticks[uJoyID].timer = 0;
            Joysticks[uJoyID].changed = 0;
        }

        return JOYERR_NOERROR;
    }

    MMRESULT WINAPI joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged) {
        std::cout << "joySetCapture()" << std::endl;

        if (hwnd == nullptr) return JOYERR_PARMS;
        if (uJoyID > MAXJOY) return JOYERR_PARMS;
        if (uPeriod < JOY_PERIOD_MIN || uPeriod > JOY_PERIOD_MAX) return JOYERR_PARMS;

        if (Joysticks[uJoyID].capture || !IsWindow(hwnd)) return JOYERR_NOCANDO;

        SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
        if (!(Flags & SDL_INIT_GAMEPAD)) {
            return MMSYSERR_NODRIVER;
        }

        SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(uJoyID);
        if (gamepad == nullptr) {
            return JOYERR_UNPLUGGED;
        }

        Joysticks[uJoyID].timer = SetTimer(hwnd, 0, uPeriod, joystick_capture);
        if (Joysticks[uJoyID].timer == 0) return JOYERR_NOCANDO;

        Joysticks[uJoyID].capture = hwnd;
        Joysticks[uJoyID].changed = fChanged;

        return JOYERR_NOERROR;
    }

#ifdef WINMM_EXPORTS
}
#endif