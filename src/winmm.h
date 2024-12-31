/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include <Windows.h>
#include <SDL3/SDL.h>
#include <iostream>

#define MMSYSERR_NOERROR 0
#define MMSYSERR_ERROR  1
#define MMSYSERR_BADDEVICEID 2
#define MMSYSERR_NOTENABLED 3
#define MMSYSERR_NODRIVER -2
#define MMSYSERR_INVALPARAM -4

#define JOYERR_NOERROR 0
#define JOYERR_PARMS -2
#define JOYERR_UNPLUGGED -3
#define JOYERR_NOCANDO -4

#define JOY_CONFIGCHANGED_MSGSTRING "MSJSTICK_VJOYD_MSGSTR"

#define MAXPNAMELEN 32
#define MAX_JOYSTICKOEMVXDNAME 260

#define MAXJOY 15

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

MMRESULT joyConfigChanged(DWORD dwFlags);
MMRESULT joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc);
MMRESULT joyGetDevCapsW(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc);
UINT joyGetNumDevs(void);
MMRESULT joyGetPos(UINT uJoyID, LPJOYINFO pji);
MMRESULT joyGetPosEx(UINT uJoyID, LPJOYINFOEX pjiEx);
MMRESULT joyGetThreshold(UINT uJoyID, LPUINT puThreshold);
MMRESULT joyReleaseCapture(UINT uJoyID);
MMRESULT joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fCapture);
MMRESULT joySetThreshold(UINT uJoyID, UINT uThreshold);