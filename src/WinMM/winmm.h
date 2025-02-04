/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <SDL3/SDL.h>
#include <unordered_map>
#include <cmath>
#include <bitset>

#define MMSYSERR_NOERROR       0
#define MMSYSERR_ERROR         1
#define MMSYSERR_BADDEVICEID   2
#define MMSYSERR_NOTENABLED    3
#define MMSYSERR_NODRIVER     -2
#define MMSYSERR_INVALPARAM   -4

#define JOYERR_NOERROR     0
#define JOYERR_PARMS      -2
#define JOYERR_UNPLUGGED  -3
#define JOYERR_NOCANDO    -4

#define JOY_CONFIGCHANGED_MSGSTRING ("MSJSTICK_VJOYD_MSGSTR")

#define MAXPNAMELEN             32
#define MAX_JOYSTICKOEMVXDNAME  260

#define JOYCAPS_HASZ        0x00000001
#define JOYCAPS_HASR        0x00000002
#define JOYCAPS_HASU        0x00000004
#define JOYCAPS_HASV        0x00000008
#define JOYCAPS_HASPOV      0x00000010
#define JOYCAPS_POV4DIR     0x00000020
#define JOYCAPS_POVCTS      0x00000040

#define MM_JOY1MOVE         0x3A0
#define MM_JOY1ZMOVE        0x3A2
#define MM_JOY1BUTTONDOWN   0x3B5
#define MM_JOY1BUTTONUP     0x3B7

#define JOY_RETURNX		      0x00000001
#define JOY_RETURNY		      0x00000002
#define JOY_RETURNZ		      0x00000004
#define JOY_RETURNR		      0x00000008
#define JOY_RETURNU		      0x00000010
#define JOY_RETURNV		      0x00000020
#define JOY_RETURNPOV		    0x00000040
#define JOY_RETURNBUTTONS	  0x00000080
#define JOY_RETURNALL		    (JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ | JOY_RETURNR | JOY_RETURNU | JOY_RETURNV | JOY_RETURNPOV | JOY_RETURNBUTTONS)

#define MAXJOY 16

#define JOY_PERIOD_MIN	10
#define JOY_PERIOD_MAX	1000

typedef struct JOYINFO {
  UINT wXpos;
  UINT wYpos;
  UINT wZpos;
  UINT wButtons;
} JOYINFO, *PJOYINFO, *NPJOYINFO, *LPJOYINFO;

typedef struct JOYINFOEX {
  DWORD dwSize;
  DWORD dwFlags;
  DWORD dwXpos;
  DWORD dwYpos;
  DWORD dwZpos;
  DWORD dwRpos;
  DWORD dwUpos;
  DWORD dwVpos;
  DWORD dwButtons;
  DWORD dwButtonNumber;
  DWORD dwPOV;
  DWORD dwReserved1;
  DWORD dwReserved2;
} JOYINFOEX, *PJOYINFOEX, *NPJOYINFOEX, *LPJOYINFOEX;

typedef struct JOYCAPSA {
  WORD wMid;
  WORD wPid;
  char szPname[MAXPNAMELEN];
  UINT wXmin;
  UINT wXmax;
  UINT wYmin;
  UINT wYmax;
  UINT wZmin;
  UINT wZmax;
  UINT wNumButtons;
  UINT wPeriodMin;
  UINT wPeriodMax;
  UINT wRmin;
  UINT wRmax;
  UINT wUmin;
  UINT wUmax;
  UINT wVmin;
  UINT wVmax;
  UINT wCaps;
  UINT wMaxAxes;
  UINT wNumAxes;
  UINT wMaxButtons;
  char szRegKey[MAXPNAMELEN];
  char szOEMVxD[MAX_JOYSTICKOEMVXDNAME];
} JOYCAPSA, *PJOYCAPSA, *NPJOYCAPSA, *LPJOYCAPSA;

typedef struct JOYCAPSW {
  WORD  wMid;
  WORD  wPid;
  WCHAR szPname[MAXPNAMELEN];
  UINT  wXmin;
  UINT  wXmax;
  UINT  wYmin;
  UINT  wYmax;
  UINT  wZmin;
  UINT  wZmax;
  UINT  wNumButtons;
  UINT  wPeriodMin;
  UINT  wPeriodMax;
  UINT  wRmin;
  UINT  wRmax;
  UINT  wUmin;
  UINT  wUmax;
  UINT  wVmin;
  UINT  wVmax;
  UINT  wCaps;
  UINT  wMaxAxes;
  UINT  wNumAxes;
  UINT  wMaxButtons;
  WCHAR szRegKey[MAXPNAMELEN];
  WCHAR szOEMVxD[MAX_JOYSTICKOEMVXDNAME];
} JOYCAPSW, *PJOYCAPSW, *NPJOYCAPSW, *LPJOYCAPSW;

typedef UINT MMRESULT;

typedef struct JOYSTICK {
    JOYINFO info;
    HWND capture;
    UINT_PTR timer;
    UINT threshold;
    BOOL changed;
} JOYSTICK;

typedef MMRESULT (WINAPI *joyConfigChanged_t)(DWORD dwFlags);
typedef MMRESULT (WINAPI *joyGetDevCapsA_t)(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc);
typedef MMRESULT (WINAPI *joyGetDevCapsW_t)(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc);
typedef UINT (WINAPI *joyGetNumDevs_t)(void);
typedef MMRESULT (WINAPI *joyGetPos_t)(UINT uJoyID, LPJOYINFO pji);
typedef MMRESULT (WINAPI *joyGetPosEx_t)(UINT uJoyID, LPJOYINFOEX pjiEx);
typedef MMRESULT (WINAPI *joyGetThreshold_t)(UINT uJoyID, LPUINT puThreshold);
typedef MMRESULT (WINAPI *joyReleaseCapture_t)(UINT uJoyID);
typedef MMRESULT (WINAPI *joySetCapture_t)(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fCapture);
typedef MMRESULT (WINAPI *joySetThreshold_t)(UINT uJoyID, UINT uThreshold);

#ifdef WINMM_EXPORTS
extern "C" {
#endif
MMRESULT WINAPI joyConfigChanged(DWORD dwFlags);
MMRESULT WINAPI joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc);
MMRESULT WINAPI joyGetDevCapsW(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc);
UINT WINAPI joyGetNumDevs(void);
MMRESULT WINAPI joyGetPos(UINT uJoyID, LPJOYINFO pji);
MMRESULT WINAPI joyGetPosEx(UINT uJoyID, LPJOYINFOEX pjiEx);
MMRESULT WINAPI joyGetThreshold(UINT uJoyID, LPUINT puThreshold);
MMRESULT WINAPI joyReleaseCapture(UINT uJoyID);
MMRESULT WINAPI joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fCapture);
MMRESULT WINAPI joySetThreshold(UINT uJoyID, UINT uThreshold);
#ifdef WINMM_EXPORTS
}
#endif

UINT absDiff(UINT a, UINT b);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
void CALLBACK joystick_capture(HWND hWnd, UINT wMsg, UINT_PTR wTimer, DWORD dwTime);