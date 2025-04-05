/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput8.h"
#include <unordered_map>
#include <algorithm>

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

  DIDEVICEOBJECTINSTANCEW lpddoi = { 0 };
  lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEW);
  bool DIENUM = lpCallback(&lpddoi, pvRef);

  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph) {
  SDL_Log("IDirectInputDevice8W::GetProperty()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph) {
  SDL_Log("IDirectInputDevice8W::SetProperty()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Acquire() {
  SDL_Log("IDirectInputDevice8W::Acquire()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::Unacquire() {
  SDL_Log("IDirectInputDevice8W::Unacquire()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceState(DWORD cbData, LPVOID lpvData) {
  SDL_Log("IDirectInputDevice8W::GetDeviceState()");

  //IDirectInputDevice8::SetDataFormat
  //c_dfDIJoystick -> DIJOYSTATE
  //c_dfDIJoystick2 -> DIJOYSTATE2

  if (lpvData == nullptr) return DIERR_INVALIDPARAM;
  //if (cbData != sizeof(DIJOYSTATE) && cbData != sizeof(DIJOYSTATE2)) return DIERR_INVALIDPARAM;
  if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;

  SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
  if (!(Flags & SDL_INIT_GAMEPAD)) {
    return E_PENDING;
  }

  SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(this->playerIndex);
  if (gamepad == nullptr) {
    return DIERR_NOTACQUIRED;
  }
  
  SDL_UpdateGamepads();

  //ZeroMemory(lpvData, cbData);

  if (cbData == sizeof(DIJOYSTATE)) {
    SDL_Log("IDirectInputDevice8W::GetDeviceState() > DIJOYSTATE");
  }
  else if (cbData == sizeof(DIJOYSTATE2)) {
    SDL_Log("IDirectInputDevice8W::GetDeviceState() > DIJOYSTATE2");
  }

  SDL_Log("IDirectInputDevice8W::GetDeviceState() > Size: %u", cbData); //re = 60 ??!! maybe because previous call did not give correctly gamepad capabilities or its own DIDATAFORMAT with SetDataFormat() ??

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
  
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::GetDeviceData()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SetDataFormat(LPCDIDATAFORMAT lpdf) {
  SDL_Log("IDirectInputDevice8W::SetDataFormat()");
  //if (IsEqualGUID(*lpdf->rgodf->pguid, c_dfDIJoystick)
  
  if (lpdf == nullptr) return DIERR_INVALIDPARAM;
  if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;
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
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi) {
  SDL_Log("IDirectInputDevice8W::GetDeviceInfo()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::RunControlPanel()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid) {
  SDL_Log("IDirectInputDevice8W::Initialize()");

  if (this->playerIndex >= 0) return S_FALSE; //device already initialized

  //REMOVE TEMPORARY
  /*if (rguid.Data1 != MAKELONG(XBOX360_VID, XBOX360_PID) ||
    std::string(reinterpret_cast<const char*>(rguid.Data4) + 2, 6) != "PLAYER") return DIERR_DEVICENOTREG;

  this->playerIndex = rguid.Data4[1];
  if (this->playerIndex < 0) return DIERR_DEVICENOTREG;*/

  this->playerIndex = 0;

  SDL_Log("IDirectInputDevice8W::Initialize() > Set player to %i", this->playerIndex);
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter) {
  SDL_Log("IDirectInputDevice8W::CreateEffect()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType) {
  SDL_Log("IDirectInputDevice8W::EnumEffects()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid) {
  SDL_Log("IDirectInputDevice8W::GetEffectInfo()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetForceFeedbackState(LPDWORD pdwOut) {
  SDL_Log("IDirectInputDevice8W::GetForceFeedbackState()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SendForceFeedbackCommand(DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::SendForceFeedbackCommand()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl) {
  SDL_Log("IDirectInputDevice8W::EnumCreatedEffectObjects()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Escape(LPDIEFFESCAPE pesc) {
  SDL_Log("IDirectInputDevice8W::Escape()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Poll() {
  SDL_Log("IDirectInputDevice8W::Poll()");
  return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl) {
  SDL_Log("IDirectInputDevice8W::SendDeviceData()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::EnumEffectsInFile(LPCWSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::EnumEffectsInFile()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::WriteEffectToFile(LPCWSTR lpszFileName, DWORD dwEntries, LPCDIFILEEFFECT rgDiFileEft, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::WriteEffectToFile()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::BuildActionMap(LPDIACTIONFORMATW lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::BuildActionMap()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SetActionMap(LPCDIACTIONFORMATW lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags) {
  SDL_Log("IDirectInputDevice8W::SetActionMap()");
  return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW lpdiDevImageInfoHeader) {
  SDL_Log("IDirectInputDevice8W::GetImageInfo()");
  return E_NOTIMPL;
}