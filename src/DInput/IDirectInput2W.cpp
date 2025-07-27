/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInput2W::IDirectInput2W() : m_refCount(1) {
  SDL_Log("IDirectInput2W");
  proxy = new(std::nothrow) IDirectInput8W();
}

IDirectInput2W::~IDirectInput2W() {
  SDL_Log("~IDirectInput2W");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInput2W::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInput2W::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInput2W))
  {
    *ppvObject = static_cast<IDirectInput2W*>(this);
    AddRef();
    return S_OK;
  }
  
  if (IsEqualGUID(riid, IID_IDirectInputJoyConfig)) { //Resident Evil (1996) GOG
    SDL_Log("IDirectInput2W::QueryInterface() > IDirectInputJoyConfig");

    IDirectInputJoyConfig* pDInputJoyConfig = new(std::nothrow) IDirectInputJoyConfig;
    if (pDInputJoyConfig == nullptr) return E_NOINTERFACE;
    *ppvObject = static_cast<IDirectInputJoyConfig*>(pDInputJoyConfig);

    return S_OK;
  }

  SDL_Log("IDirectInput2W::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInput2W::AddRef() {
  SDL_Log("IDirectInput2W::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInput2W::Release() {
  SDL_Log("IDirectInput2W::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInput2W::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE2W *lplpDirectInputDevice, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInput2W::CreateDevice() <%s>", GUIDToString(rguid).c_str());
  
  IDirectInputDevice2W* pDInputDevice = new(std::nothrow) IDirectInputDevice2W;
  if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
  *lplpDirectInputDevice = static_cast<IDirectInputDevice2W*>(pDInputDevice);

  HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION_500, rguid);
  if (FAILED(hr)) { return hr; }

  return DI_OK;
}

STDMETHODIMP IDirectInput2W::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInput2W::EnumDevices()");
  
  if (proxy){
      return proxy->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2W::GetDeviceStatus(REFGUID rguidInstance){
  SDL_Log("IDirectInput2W::GetDeviceStatus()");
  
  if (proxy){
      return proxy->GetDeviceStatus(rguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2W::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInput2W::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2W::Initialize(HINSTANCE hinst, DWORD dwVersion){
  SDL_Log("IDirectInput2W::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2W::FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance){
  SDL_Log("IDirectInput2W::FindDevice()");
  
  if (proxy){
      return proxy->FindDevice(rguidClass, ptszName, pguidInstance);
  }
  
  return E_POINTER;
}