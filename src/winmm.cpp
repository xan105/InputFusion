#include "winmm.h"

std::atomic<UINT> threshold[MAXJOY] = {0};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (GetParent(hwnd) == NULL) {
        SendMessage(hwnd, WM_USER, 0, (LPARAM)JOY_CONFIGCHANGED_MSGSTRING);
    }
    return TRUE;
}

MMRESULT joyConfigChanged(DWORD dwFlags){
  if (dwFlags != 0) return JOYERR_PARMS;
  EnumWindows(EnumWindowsProc, 0);
  return JOYERR_NOERROR;
}

MMRESULT joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc){
  if (pjc == nullptr) return MMSYSERR_INVALPARAM;
  if (uJoyID > MAXJOY || uJoyID < -1) return MMSYSERR_INVALPARAM;

  //to do

  return JOYERR_NOERROR;
}

MMRESULT joyGetDevCapsW(UINT_PTR uJoyID, LPJOYCAPSW pjc, UINT cbjc){
  if (pjc == nullptr) return MMSYSERR_INVALPARAM;
  if (uJoyID > MAXJOY || uJoyID < -1) return MMSYSERR_INVALPARAM;

  //to do

  return JOYERR_NOERROR;
}

UINT joyGetNumDevs(void){
  return MAXJOY;
}

MMRESULT joyGetPos(UINT uJoyID, LPJOYINFO pji){
  if (pji == nullptr) return MMSYSERR_INVALPARAM;
  if (uJoyID >= MAXJOY) return MMSYSERR_INVALPARAM;
  
  //to do
  
  return JOYERR_NOERROR;
}

MMRESULT joyGetPosEx(UINT uJoyID, LPJOYINFOEX pjiEx){
  if (pjiEx == nullptr) return MMSYSERR_INVALPARAM;
  if (uJoyID >= MAXJOY) return MMSYSERR_INVALPARAM;
  
  //to do
  
  return JOYERR_NOERROR;
}

MMRESULT joyGetThreshold(UINT uJoyID, LPUINT puThreshold){
  if (puThreshold == nullptr) return MMSYSERR_INVALPARAM;
  if (uJoyID >= MAXJOY) return MMSYSERR_INVALPARAM;
  
  *puThreshold = threshold[uJoyID];
  
  return JOYERR_NOERROR;
}

MMRESULT joySetThreshold(UINT uJoyID, UINT uThreshold){;
  if (uJoyID >= MAXJOY) return JOYERR_PARMS;
  
  threshold[uJoyID] = uThreshold;
  
  return JOYERR_NOERROR;
}

MMRESULT joyReleaseCapture(UINT uJoyID){
//to do
}

MMRESULT joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fCapture){
//to do
}