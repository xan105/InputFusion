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

IDirectInputDevice8A::IDirectInputDevice8A() : m_refCount(1) {
  SDL_Log("IDirectInputDevice8A");
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

STDMETHODIMP IDirectInputDevice8A::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputDevice8A::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if (IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputDevice8A))
  {
    *ppvObject = static_cast<IDirectInputDevice8A*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputDevice8A::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDevice8A::AddRef() {
  SDL_Log("IDirectInputDevice8A::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDevice8A::Release() {
  SDL_Log("IDirectInputDevice8A::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputDevice8A::GetCapabilities(LPDIDEVCAPS lpDIDevCaps) {
  SDL_Log("IDirectInputDevice8A::GetCapabilities()");

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

STDMETHODIMP IDirectInputDevice8A::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::EnumObjects()");

  DIDEVICEOBJECTINSTANCEA lpddoi = { 0 };
  lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEA);

  bool DIENUM = lpCallback(&lpddoi, pvRef);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph) {
  SDL_Log("IDirectInputDevice8A::GetProperty()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph) {

    SDL_Log("IDirectInputDevice8A::SetProperty()");

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
    
    //return DIERR_UNSUPPORTED;
    return DI_OK; //(dino crisis 1 needs kbm first)
}

STDMETHODIMP IDirectInputDevice8A::Acquire() {
  SDL_Log("IDirectInputDevice8A::Acquire() for %d", this->playerIndex);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::Unacquire() {
  SDL_Log("IDirectInputDevice8A::Unacquire()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetDeviceState(DWORD cbData, LPVOID lpvData) {
  SDL_Log("IDirectInputDevice8A::GetDeviceState() for %d", this->playerIndex);

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
      SDL_Log("IDirectInputDevice8A::GetDeviceState() > GetKeyboardState()");
      return DI_OK; 
    } else {
      return DIERR_INPUTLOST;
    }*/
    return DI_OK;
  }
  if (cbData != sizeof(DIJOYSTATE) && cbData != sizeof(DIJOYSTATE2))  {
    SDL_Log("IDirectInputDevice8A::GetDeviceState() > Unknown data format!");
    return DIERR_INVALIDPARAM;
  }
  if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;

  if (cbData == sizeof(DIJOYSTATE)) {
    SDL_Log("IDirectInputDevice8A::GetDeviceState() > DIJOYSTATE");
  }
  if (cbData == sizeof(DIJOYSTATE2)) {
    SDL_Log("IDirectInputDevice8A::GetDeviceState() > DIJOYSTATE2");
  }
  
  ZeroMemory(lpvData, cbData);

  SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
  if (!(Flags & SDL_INIT_GAMEPAD)) {
      return DI_OK;
  }

  SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(this->playerIndex);
  if (gamepad == nullptr) {
      return DI_OK;
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
    float normalized = value / (float)(value < 0 ? -SDL_JOYSTICK_AXIS_MIN : SDL_JOYSTICK_AXIS_MAX);
    //NB: what about the triggers (to-do)

    if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
      LONG linear = static_cast<LONG>(this->AXIS_LEFTX_MIN + (normalized + 1.0f) * (this->AXIS_LEFTX_MAX - this->AXIS_LEFTX_MIN) / 2.0f);
      if (cbData == sizeof(DIJOYSTATE)) {
          ((DIJOYSTATE*)lpvData)->lX = linear;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
          ((DIJOYSTATE2*)lpvData)->lX = linear;
      }
    }
    else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
      LONG linear = static_cast<LONG>(this->AXIS_LEFTY_MIN + (normalized + 1.0f) * (this->AXIS_LEFTY_MAX - this->AXIS_LEFTY_MIN) / 2.0f);
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lY = linear;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lY = linear;
      }
    }
    else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
      LONG linear = static_cast<LONG>(this->AXIS_RIGHTX_MIN + (normalized + 1.0f) * (this->AXIS_RIGHTX_MAX - this->AXIS_RIGHTX_MIN) / 2.0f);
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lRx = linear;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lRx = linear;
      }
    }
    else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
      LONG linear = static_cast<LONG>(this->AXIS_RIGHTY_MIN + (normalized + 1.0f) * (this->AXIS_RIGHTY_MAX - this->AXIS_RIGHTY_MIN) / 2.0f);
      if (cbData == sizeof(DIJOYSTATE)) {
        ((DIJOYSTATE*)lpvData)->lRy = linear;
      }
      else if (cbData == sizeof(DIJOYSTATE2)) {
        ((DIJOYSTATE2*)lpvData)->lRy = linear;
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
  
  SDL_Log("IDirectInputDevice8A::GetDeviceState() > OK");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::GetDeviceData()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::SetDataFormat(LPCDIDATAFORMAT lpdf) {
  SDL_Log("IDirectInputDevice8A::SetDataFormat()");

  //if (IsEqualGUID(*lpdf->rgodf->pguid, c_dfDIJoystick)

  if (lpdf == nullptr) return DIERR_INVALIDPARAM;
  //if (this->playerIndex < 0) return DIERR_NOTINITIALIZED; //dino crisis 1 needs kbm first
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::SetEventNotification(HANDLE hEvent) {
  SDL_Log("IDirectInputDevice8A::SetEventNotification()");

  //SDL_UpdateGamepads();
  //SetEvent(&hEvent);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::SetCooperativeLevel(HWND hwnd, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::SetCooperativeLevel()");

  //if (hwnd == nullptr) return DIERR_INVALIDPARAM;
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow) {
  SDL_Log("IDirectInputDevice8A::GetObjectInfo()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi) {
  SDL_Log("IDirectInputDevice8A::GetDeviceInfo()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::RunControlPanel()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid) {
  SDL_Log("IDirectInputDevice8A::Initialize()");

  if (IsEqualGUID(rguid, GUID_SysKeyboard)) {
      SDL_Log("HELLO KBM");
      return DI_OK; //fixme later: let it through
  }

  if (this->playerIndex >= 0) return S_FALSE; //device already initialized

  if (rguid.Data1 != MAKELONG(XBOX360_VID, XBOX360_PID) ||
    std::string(reinterpret_cast<const char*>(rguid.Data4)+2, 6) != "PLAYER") return DIERR_DEVICENOTREG;

  this->playerIndex = rguid.Data4[1];
  if (this->playerIndex < 0) {
    SDL_Log("IDirectInputDevice8A::Initialize() > DIERR_DEVICENOTREG");
    return DIERR_DEVICENOTREG;
  }
  
  SDL_Log("IDirectInputDevice8A::Initialize() > Set player to %i", this->playerIndex);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter) {
  SDL_Log("IDirectInputDevice8A::CreateEffect()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType) {
  SDL_Log("IDirectInputDevice8A::EnumEffects()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid) {
  SDL_Log("IDirectInputDevice8A::GetEffectInfo()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::GetForceFeedbackState(LPDWORD pdwOut) {
  SDL_Log("IDirectInputDevice8A::GetForceFeedbackState()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::SendForceFeedbackCommand(DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::SendForceFeedbackCommand()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl) {
  SDL_Log("IDirectInputDevice8A::EnumCreatedEffectObjects()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::Escape(LPDIEFFESCAPE pesc) {
  SDL_Log("IDirectInputDevice8A::Escape()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::Poll() {
  SDL_Log("IDirectInputDevice8A::Poll()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl) {
  SDL_Log("IDirectInputDevice8A::SendDeviceData()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::EnumEffectsInFile()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPCDIFILEEFFECT rgDiFileEft, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::WriteEffectToFile()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::BuildActionMap(LPDIACTIONFORMATA lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::BuildActionMap()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::SetActionMap(LPCDIACTIONFORMATA lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8A::SetActionMap()");
  return DIERR_UNSUPPORTED;
}

STDMETHODIMP IDirectInputDevice8A::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA lpdiDevImageInfoHeader) {
  SDL_Log("IDirectInputDevice8A::GetImageInfo()");
  return DIERR_UNSUPPORTED;
}