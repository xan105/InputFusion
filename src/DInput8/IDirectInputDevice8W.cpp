/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput8.h"
#include <unordered_map>
#include <algorithm>
#include "../util.h"

const std::unordered_map<SDL_GamepadButton, DWORD> BUTTONS = {
  {SDL_GAMEPAD_BUTTON_SOUTH, 0},
  {SDL_GAMEPAD_BUTTON_EAST, 1},
  {SDL_GAMEPAD_BUTTON_WEST, 2},
  {SDL_GAMEPAD_BUTTON_NORTH, 3},
  {SDL_GAMEPAD_BUTTON_BACK, 6},
  {SDL_GAMEPAD_BUTTON_START, 7},
  {SDL_GAMEPAD_BUTTON_LEFT_STICK, 8},
  {SDL_GAMEPAD_BUTTON_RIGHT_STICK, 9},
  {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, 4},
  {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, 5},
  {SDL_GAMEPAD_BUTTON_TOUCHPAD, 6}
};

const std::vector<SDL_GamepadAxis> AXIS = {
  SDL_GAMEPAD_AXIS_LEFTX,
  SDL_GAMEPAD_AXIS_LEFTY,
  SDL_GAMEPAD_AXIS_RIGHTX,
  SDL_GAMEPAD_AXIS_RIGHTY,
  SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
  SDL_GAMEPAD_AXIS_RIGHT_TRIGGER
};

IDirectInputDevice8W::IDirectInputDevice8W() : m_refCount(1) {
  SDL_Log("IDirectInputDevice8W");
  this->playerIndex = -1;
  this->AXIS_LEFTX_MIN = (LONG)SDL_JOYSTICK_AXIS_MIN;
  this->AXIS_LEFTX_MAX = (LONG)SDL_JOYSTICK_AXIS_MAX;
  this->AXIS_LEFTY_MIN = (LONG)SDL_JOYSTICK_AXIS_MIN;
  this->AXIS_LEFTY_MAX = (LONG)SDL_JOYSTICK_AXIS_MAX;
  this->AXIS_RIGHTX_MIN = (LONG)SDL_JOYSTICK_AXIS_MIN;
  this->AXIS_RIGHTX_MAX = (LONG)SDL_JOYSTICK_AXIS_MAX;
  this->AXIS_RIGHTY_MIN = (LONG)SDL_JOYSTICK_AXIS_MIN;
  this->AXIS_RIGHTY_MAX = (LONG)SDL_JOYSTICK_AXIS_MAX;
}

STDMETHODIMP IDirectInputDevice8W::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputDevice8W::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if (IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputDevice8W))
  {
    *ppvObject = static_cast<IDirectInputDevice8W*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputDevice8A::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDevice8W::AddRef() {
  SDL_Log("IDirectInputDevice8W::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDevice8W::Release() {
  SDL_Log("IDirectInputDevice8W::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}


STDMETHODIMP IDirectInputDevice8W::GetCapabilities(LPDIDEVCAPS lpDIDevCaps) {
  SDL_Log("IDirectInputDevice8W::GetCapabilities()");

  if (lpDIDevCaps == nullptr) return E_POINTER;
  if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;

  if (lpDIDevCaps->dwSize == sizeof(DIDEVCAPS)) {
    ZeroMemory(lpDIDevCaps, sizeof(DIDEVCAPS));
    lpDIDevCaps->dwSize = sizeof(DIDEVCAPS);
  }
  else if (lpDIDevCaps->dwSize == sizeof(DIDEVCAPS_DX3)) {
    ZeroMemory(lpDIDevCaps, sizeof(DIDEVCAPS_DX3));
    lpDIDevCaps->dwSize = sizeof(DIDEVCAPS_DX3);
  }
  else {
    return DIERR_INVALIDPARAM;
  }

  lpDIDevCaps->dwFlags = DIDC_ATTACHED | DIDC_EMULATED;
  lpDIDevCaps->dwDevType = (MAKEWORD(DI8DEVTYPE_GAMEPAD, DI8DEVTYPEGAMEPAD_STANDARD) | DIDEVTYPE_HID);
  lpDIDevCaps->dwAxes = 5;
  lpDIDevCaps->dwButtons = 10;
  lpDIDevCaps->dwPOVs = 1;

  if (lpDIDevCaps->dwSize == sizeof(DIDEVCAPS)) {
    lpDIDevCaps->dwFFSamplePeriod = 0;
    lpDIDevCaps->dwFFMinTimeResolution = 0;
    lpDIDevCaps->dwFirmwareRevision = 0;
    lpDIDevCaps->dwHardwareRevision = 0;
    lpDIDevCaps->dwFFDriverVersion = 0;
  }

  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::EnumObjects()");

  //Offsets below assume DIJOYSTATE
  
  if (lpCallback == nullptr) return E_POINTER;

  if (dwFlags & DIDFT_ALL || dwFlags & DIDFT_AXIS) {
        static const struct {
            GUID guid;
            DWORD offset;
            const wchar_t* name;
        } Axes[] = {
            { GUID_XAxis, 0,   L"X Axis" },
            { GUID_YAxis, 4,   L"Y Axis" },
            { GUID_ZAxis, 8,   L"Z Axis" },
            { GUID_RxAxis, 12, L"X Rotation" },
            { GUID_RyAxis, 16, L"Y Rotation" },
            { GUID_RzAxis, 20, L"Z Rotation" },
        };
  
        for (int i = 0; i < std::size(Axes); ++i) {
            DIDEVICEOBJECTINSTANCEW lpddoi = { 0 };
            lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEW);
            lpddoi.guidType = Axes[i].guid;
            lpddoi.dwOfs = Axes[i].offset;
            lpddoi.dwType = DIDFT_AXIS | DIDFT_MAKEINSTANCE(i);
            wcscpy_s(lpddoi.tszName, MAX_PATH, Axes[i].name);
            if (lpCallback(&lpddoi, pvRef) == DIENUM_STOP) return DI_OK;
        }
        if (dwFlags & DIDFT_AXIS) return DI_OK;
    }

  if (dwFlags & DIDFT_ALL || dwFlags & DIDFT_POV) {
        static const DWORD POV[] = { 32, 36, 40, 44 };
  
        for (int i = 0; i < 4; ++i) {
            DIDEVICEOBJECTINSTANCEW lpddoi = { 0 };
            lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEW);
            lpddoi.guidType = GUID_POV;
            lpddoi.dwOfs = POV[i];
            lpddoi.dwType = DIDFT_POV | DIDFT_MAKEINSTANCE(i);
            swprintf_s(lpddoi.tszName, MAX_PATH, L"POV %d", i);
            if (lpCallback(&lpddoi, pvRef) == DIENUM_STOP) return DI_OK;
        }
        if (dwFlags & DIDFT_POV) return DI_OK;
  }

  if (dwFlags & DIDFT_ALL || dwFlags & DIDFT_BUTTON) {
        for (int i = 0; i < 32; ++i) {
            DIDEVICEOBJECTINSTANCEW lpddoi = { 0 };
            lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEW);
            lpddoi.guidType = GUID_Button;
            lpddoi.dwOfs = 48 + i;
            lpddoi.dwType = DIDFT_BUTTON | DIDFT_MAKEINSTANCE(i);
            swprintf_s(lpddoi.tszName, MAX_PATH, L"Button %d", i);
            if (lpCallback(&lpddoi, pvRef) == DIENUM_STOP) return DI_OK;
        }
        if (dwFlags & DIDFT_BUTTON) return DI_OK;
  }

  DIDEVICEOBJECTINSTANCEW lpddoi = { 0 };
  lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEW);

  bool DIENUM = lpCallback(&lpddoi, pvRef);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph) {
  SDL_Log("IDirectInputDevice8W::GetProperty()");

  if (pdiph == nullptr) return E_POINTER;

  //Check if it is just a small integer disguised as a pointer and not an actual GUID pointer.
  if (((ULONG_PTR)(&rguidProp) >> 16) != 0) return DI_OK;

  WORD propID = LOWORD(reinterpret_cast<ULONG_PTR>(&rguidProp));

  switch (propID) {
  case DIPROP_GRANULARITY: {
      SDL_Log("GetProperty: DIPROP_GRANULARITY");
      DIPROPDWORD* pd = reinterpret_cast<DIPROPDWORD*>(pdiph);
      pd->dwData = 1;
      break;
  }
  default:
      SDL_Log("GetProperty: Unsupported property ID = %hu", propID);
      break;
  };

  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph) {
  SDL_Log("IDirectInputDevice8W::SetProperty()");
  
    if (pdiph == nullptr) return E_POINTER;
    if (pdiph->dwHeaderSize != sizeof(DIPROPHEADER)) return DIERR_INVALIDPARAM;

    //Check if it is just a small integer disguised as a pointer and not an actual GUID pointer.
    if (((ULONG_PTR)(&rguidProp) >> 16) != 0) return DI_OK;

    WORD propID = LOWORD(reinterpret_cast<ULONG_PTR>(&rguidProp));

    switch (propID) {
    case DIPROP_RANGE:
        SDL_Log("SetProperty: DIPROP_RANGE");
        if (pdiph->dwSize == sizeof(DIPROPRANGE)) {
            DIPROPRANGE* range = (DIPROPRANGE*)pdiph;
            SDL_Log("    Range: lMin = %ld, lMax = %ld for 0x%08X as %d", range->lMin, range->lMax, range->diph.dwObj, range->diph.dwHow);

            if (range->diph.dwHow == DIPH_BYOFFSET) {
                switch (range->diph.dwObj) {
                //Assuming DIJOYSTATE
                case 0x00: //lX
                    this->AXIS_LEFTX_MIN = range->lMin;
                    this->AXIS_LEFTX_MAX = range->lMax;
                    break;
                case 0x04: //lY
                    this->AXIS_LEFTY_MIN = range->lMin;
                    this->AXIS_LEFTY_MAX = range->lMax;
                    break;
                case 0x08: //lZ
                    break;
                case 0x0C: //lRx
                    this->AXIS_RIGHTX_MIN = range->lMin;
                    this->AXIS_RIGHTX_MAX = range->lMax;
                    break;
                case 0x10: //lRy
                    this->AXIS_RIGHTY_MIN = range->lMin;
                    this->AXIS_RIGHTY_MAX = range->lMax;
                    break;
                case 0x14: //lRz
                    break;
                }
            }
        }
        break;
    default:
        SDL_Log("SetProperty: Unsupported property ID = %hu", propID);
        break;
    };
    
    return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::Acquire() {
  SDL_Log("IDirectInputDevice8W::Acquire() for %d", this->playerIndex);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::Unacquire() {
  SDL_Log("IDirectInputDevice8W::Unacquire()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceState(DWORD cbData, LPVOID lpvData) {
  SDL_Log("IDirectInputDevice8W::GetDeviceState() for %d", this->playerIndex);

  //IDirectInputDevice8::SetDataFormat
  //c_dfDIJoystick -> DIJOYSTATE
  //c_dfDIJoystick2 -> DIJOYSTATE2

  if (lpvData == nullptr) return DIERR_INVALIDPARAM;
  if (cbData == 256) { //Keyboard | c_dfDIKeyboard
    /*ZeroMemory(lpvData, cbData);
    if (GetKeyboardState((PBYTE)lpvData)){
      //Strip off unwanted low-bit 0x01 (if any)
      BYTE* keys = (BYTE*)lpvData;
      for (int i = 0; i < 256; ++i) {
        keys[i] = (keys[i] & 0x80) ? 0x80 : 0x00;
      }
      SDL_Log("IDirectInputDevice8W::GetDeviceState() > GetKeyboardState()");
      return DI_OK; 
    } else {
      return DIERR_INPUTLOST;
    }*/
    return DI_OK;
  }
  
  SDL_Log("IDirectInputDevice8W::GetDeviceState() > Size: %u | expected %d or %d", cbData, sizeof(DIJOYSTATE), sizeof(DIJOYSTATE2));
  
  if (cbData != sizeof(DIJOYSTATE) && cbData != sizeof(DIJOYSTATE2))  {
    SDL_Log("IDirectInputDevice8W::GetDeviceState() > Unknown data format!");
    return DIERR_INVALIDPARAM;
  }
  if (cbData == sizeof(DIJOYSTATE)) {
    SDL_Log("IDirectInputDevice8W::GetDeviceState() > DIJOYSTATE");
  }
  else if (cbData == sizeof(DIJOYSTATE2)) {
    SDL_Log("IDirectInputDevice8W::GetDeviceState() > DIJOYSTATE2");
  }
  
  if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;

  ZeroMemory(lpvData, cbData);

  SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
  if (!(Flags & SDL_INIT_GAMEPAD)) {
    return E_PENDING;
  }

  SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(this->playerIndex);
  if (gamepad == nullptr) {
    return DIERR_NOTACQUIRED;
  }
  
  SDL_UpdateGamepads();

  for (const auto& [sdl_button, index] : BUTTONS) {
    if (SDL_GetGamepadButton(gamepad, sdl_button)) {
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->rgbButtons[index] = 0x80;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->rgbButtons[index] = 0x80;
      }
    }
  }

  DWORD pov = 0xFFFF; //Centered (no direction pressed)
  bool up = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
  bool down = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
  bool left = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
  bool right = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);

  if (up && down || left && right) {
    pov = 0xFFFF; // Opposing directions are cancelled out
  }
  else if (up && left) {
    pov = 0x0007;
  }
  else if (up && right) {
    pov = 0x0001;
  }
  else if (down && left) {
    pov = 0x0005;
  }
  else if (down && right) {
    pov = 0x0003;
  }
  else if (up) {
    pov = 0x0000;
  }
  else if (down) {
    pov = 0x0004;
  }
  else if (left) {
    pov = 0x0006;
  }
  else if (right) {
    pov = 0x0002;
  }

  if (cbData == sizeof(DIJOYSTATE)) {
    ((DIJOYSTATE*)lpvData)->rgdwPOV[0] = pov;
  }
  else if (cbData == sizeof(DIJOYSTATE2)) {
    ((DIJOYSTATE2*)lpvData)->rgdwPOV[0] = pov;
  }

  for (const auto& sdl_axis : AXIS) {
    int value = SDL_GetGamepadAxis(gamepad, sdl_axis);
    if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lX = value;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lX = value;
      }
    }
    else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lY = value;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lY = value;
      }
    }
    else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lRx = value;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lRx = value;
      }
    }
    else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lRy = value;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lRy = value;
      }
    }
    // Combine left trigger and right trigger as one axis for DInput Xbox controllers
    else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lZ = std::clamp(-value, -32768, 0);
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lZ = std::clamp(-value, -32768, 0);
      }
    }
    else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lZ = std::clamp(value, 0, 32767);
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lZ = std::clamp(value, 0, 32767);
      }
    }
  }
  
  SDL_Log("IDirectInputDevice8W::GetDeviceState() > OK");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::GetDeviceData()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::SetDataFormat(LPCDIDATAFORMAT lpdf) {
  SDL_Log("IDirectInputDevice8W::SetDataFormat()");
  
  //if (IsEqualGUID(*lpdf->rgodf->pguid, c_dfDIJoystick)
  
  if (lpdf == nullptr) return E_POINTER;
  //if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SetEventNotification(HANDLE hEvent) {
  SDL_Log("IDirectInputDevice8W::SetEventNotification()");
  //SDL_UpdateGamepads();
  //SetEvent(&hEvent);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SetCooperativeLevel(HWND hwnd, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::SetCooperativeLevel()");
  
  //if (hwnd == nullptr) return DIERR_INVALIDPARAM;
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow) {
  SDL_Log("IDirectInputDevice8W::GetObjectInfo()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi) {
  SDL_Log("IDirectInputDevice8W::GetDeviceInfo()");
  
  if (pdidi == nullptr) return E_POINTER;
  if (pdidi->dwSize != sizeof(DIDEVICEINSTANCEW)) return DIERR_INVALIDPARAM;
  
  pdidi->guidInstance = {
    MAKELONG(XBOX360_VID, XBOX360_PID),														            // Data1 (VID + PID)
    0x0000,																					                          // Data2 (reserved)
    0x0000,																					                          // Data3 (reserved)
    { 0x00, (BYTE)this->playerIndex, 0x50, 0x4C, 0x41, 0x59, 0x45, 0x52 }	    // Data4 (ASCII "PLAYER")
  };
  
  pdidi->guidProduct = {
    MAKELONG(XBOX360_VID, XBOX360_PID),														// Data1 VID + PID
    0x0000,																					              // Data2 (reserved)
    0x0000,																					              // Data3 (reserved)
    { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 }						// Data4 (ASCII "PIDVID")
  };
  
  pdidi->dwDevType = (MAKEWORD(DI8DEVTYPE_GAMEPAD, DI8DEVTYPEGAMEPAD_STANDARD) | DIDEVTYPE_HID); //0x00010215
  wcscpy_s(pdidi->tszInstanceName, _countof(pdidi->tszInstanceName), XBOX360_INSTANCE_NAMEW);
  wcscpy_s(pdidi->tszProductName, _countof(pdidi->tszProductName), XBOX360_PRODUCT_NAMEW);
  pdidi->guidFFDriver = GUID_NULL;
  pdidi->wUsagePage = 0x01;
  pdidi->wUsage = 0x05;
  
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::RunControlPanel()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid) {
  SDL_Log("IDirectInputDevice8W::Initialize()");

  if (IsEqualGUID(rguid, GUID_SysKeyboard)) {
      SDL_Log("KBM");
      return DI_OK; //fixme later: let it through
  }

  if (this->playerIndex >= 0) return S_FALSE; //device already initialized

  if (rguid.Data1 != MAKELONG(XBOX360_VID, XBOX360_PID) ||
    std::string(reinterpret_cast<const char*>(rguid.Data4) + 2, 6) != "PLAYER") return DIERR_DEVICENOTREG;

  this->playerIndex = rguid.Data4[1];
  if (this->playerIndex < 0) {
    SDL_Log("IDirectInputDevice8W::Initialize() > DIERR_DEVICENOTREG");
    return DIERR_DEVICENOTREG;
  }

  SDL_Log("IDirectInputDevice8W::Initialize() > Set player to %i", this->playerIndex);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter) {
  SDL_Log("IDirectInputDevice8W::CreateEffect()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType) {
  SDL_Log("IDirectInputDevice8W::EnumEffects()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid) {
  SDL_Log("IDirectInputDevice8W::GetEffectInfo()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::GetForceFeedbackState(LPDWORD pdwOut) {
  SDL_Log("IDirectInputDevice8W::GetForceFeedbackState()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::SendForceFeedbackCommand(DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::SendForceFeedbackCommand()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl) {
  SDL_Log("IDirectInputDevice8W::EnumCreatedEffectObjects()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::Escape(LPDIEFFESCAPE pesc) {
  SDL_Log("IDirectInputDevice8W::Escape()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::Poll() {
  SDL_Log("IDirectInputDevice8W::Poll()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl) {
  SDL_Log("IDirectInputDevice8W::SendDeviceData()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::EnumEffectsInFile(LPCWSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::EnumEffectsInFile()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::WriteEffectToFile(LPCWSTR lpszFileName, DWORD dwEntries, LPCDIFILEEFFECT rgDiFileEft, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::WriteEffectToFile()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::BuildActionMap(LPDIACTIONFORMATW lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::BuildActionMap()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::SetActionMap(LPCDIACTIONFORMATW lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::SetActionMap()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8W::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW lpdiDevImageInfoHeader) {
  SDL_Log("IDirectInputDevice8W::GetImageInfo()");
  return DIERR_UNSUPPORTED;
}