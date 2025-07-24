/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput8.h"
#include "../util.h"

IDirectInput8W::IDirectInput8W() : m_refCount(1) {
  SDL_Log("IDirectInput8W");
}

STDMETHODIMP IDirectInput8W::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInput8W::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInput8W))
  {
    *ppvObject = static_cast<IDirectInput8W*>(this);
    AddRef();
    return S_OK;
  }

  if (IsEqualGUID(riid, IID_IDirectInputJoyConfig8)){ //Resident Evil (1996) GOG
    SDL_Log("IDirectInput8W::QueryInterface() > IDirectInputJoyConfig8");

    IDirectInputJoyConfig8* pDInputJoyConfig = new(std::nothrow) IDirectInputJoyConfig8;
    if (pDInputJoyConfig == nullptr) return E_NOINTERFACE;
    *ppvObject = static_cast<IDirectInputJoyConfig8*>(pDInputJoyConfig);

    return S_OK;
  }

  SDL_Log("IDirectInput8W::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInput8W::AddRef() {
  SDL_Log("IDirectInput8W::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInput8W::Release() {
  SDL_Log("IDirectInput8W::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInput8W::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice, LPUNKNOWN pUnkOuter) {
  SDL_Log("IDirectInput8W::CreateDevice()");

  IDirectInputDevice8W* pDInputDevice = new(std::nothrow) IDirectInputDevice8W;
  if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
  *lplpDirectInputDevice = static_cast<IDirectInputDevice8W*>(pDInputDevice);

  HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION, rguid);
  if (FAILED(hr)) { return hr; }

  return DI_OK;
}

STDMETHODIMP IDirectInput8W::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags) {
  SDL_Log("IDirectInput8W::EnumDevices()");

  for (int i = 0; i < DIRECTINPUT_USER_MAX_COUNT; i++) { //These are placeholder gamepads so we can connect/disconnect while playing
          DIDEVICEINSTANCEW lpddi = { 0 };
          lpddi.dwSize = sizeof(DIDEVICEINSTANCEW);

          //add a flag to create a ds4 instead ?

          //To ID the gamepad later on when creating a device, I'm encoding the corresponding playerIndex into the fake guid instance
          lpddi.guidInstance = {
            MAKELONG(XBOX360_VID, XBOX360_PID),														// Data1 (VID + PID)
            0x0000,																					// Data2 (reserved)
            0x0000,																					// Data3 (reserved)
            { 0x00, (BYTE)i, 0x50, 0x4C, 0x41, 0x59, 0x45, 0x52 }	                                // Data4 (ASCII "PLAYER")
          };
          lpddi.guidProduct = {
            MAKELONG(XBOX360_VID, XBOX360_PID),														// Data1 VID + PID
            0x0000,																					// Data2 (reserved)
            0x0000,																					// Data3 (reserved)
            { 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 }										// Data4 (ASCII "PIDVID")
          };

          lpddi.dwDevType = (MAKEWORD(DI8DEVTYPE_GAMEPAD, DI8DEVTYPEGAMEPAD_STANDARD) | DIDEVTYPE_HID); //0x00010215
          wcscpy_s(lpddi.tszInstanceName, _countof(lpddi.tszInstanceName), XBOX360_INSTANCE_NAMEW);
          wcscpy_s(lpddi.tszProductName, _countof(lpddi.tszProductName), XBOX360_PRODUCT_NAMEW);
          lpddi.guidFFDriver = GUID_NULL;
          lpddi.wUsagePage = 0x01;
          lpddi.wUsage = 0x05;

          if (lpCallback(&lpddi, pvRef) == DIENUM_STOP){ break; }
  }

  return DI_OK;
}

STDMETHODIMP IDirectInput8W::GetDeviceStatus(REFGUID rguidInstance) {
  SDL_Log("IDirectInput8W::GetDeviceStatus()");
  return DI_OK;
}


STDMETHODIMP IDirectInput8W::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
  SDL_Log("IDirectInput8W::RunControlPanel()");

  if (dwFlags != 0) return DIERR_INVALIDPARAM;
  return DI_OK;
}

STDMETHODIMP IDirectInput8W::Initialize(HINSTANCE hinst, DWORD dwVersion) {
  SDL_Log("IDirectInput8W::Initialize()");
  
  //if (dwVersion < DIRECTINPUT_VERSION) return DIERR_OLDDIRECTINPUTVERSION;
  SDL_Log("DInput version: 0x%04X", dwVersion);
  return DI_OK;
}

STDMETHODIMP IDirectInput8W::FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance) {
  SDL_Log("IDirectInput8W::FindDevice()");
  return DI_OK;
}

STDMETHODIMP IDirectInput8W::EnumDevicesBySemantics(LPCWSTR ptszUserName, LPDIACTIONFORMATW lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBW lpCallback, LPVOID pvRef, DWORD dwFlags) {
  SDL_Log("IDirectInput8W::EnumDevicesBySemantics()");
  return DI_OK;
}

STDMETHODIMP IDirectInput8W::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData) {
  SDL_Log("IDirectInput8W::ConfigureDevices()");
  //IDirectInput8::ConfigureDevices is deprecated. This method always fails on Windows Vista and later versions of Windows.
  return E_NOTIMPL;
}