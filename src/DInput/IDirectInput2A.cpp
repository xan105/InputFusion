/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInput2A::IDirectInput2A() : m_refCount(1) {
  SDL_Log("IDirectInput2A");
  proxy = new(std::nothrow) IDirectInput8A();
}

IDirectInput2A::~IDirectInput2A() {
  SDL_Log("~IDirectInput2A");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInput2A::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInput2A::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInput2A))
  {
    *ppvObject = static_cast<IDirectInput2A*>(this);
    AddRef();
    return S_OK;
  }
  
  if (IsEqualGUID(riid, IID_IDirectInputJoyConfig)) { //Resident Evil (1996) GOG
    SDL_Log("IDirectInput2A::QueryInterface() > IDirectInputJoyConfig");

    IDirectInputJoyConfig* pDInputJoyConfig = new(std::nothrow) IDirectInputJoyConfig;
    if (pDInputJoyConfig == nullptr) return E_NOINTERFACE;
    *ppvObject = static_cast<IDirectInputJoyConfig*>(pDInputJoyConfig);

    return S_OK;
  }

  SDL_Log("IDirectInput2A::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInput2A::AddRef() {
  SDL_Log("IDirectInput2A::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInput2A::Release() {
  SDL_Log("IDirectInput2A::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInput2A::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE2A *lplpDirectInputDevice, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInput2A::CreateDevice() <%s>", GUIDToString(rguid).c_str());
  
  IDirectInputDevice2A* pDInputDevice = new(std::nothrow) IDirectInputDevice2A;
  if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
  *lplpDirectInputDevice = static_cast<IDirectInputDevice2A*>(pDInputDevice);

  HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION_500, rguid);
  if (FAILED(hr)) { return hr; }

  return DI_OK;
}

STDMETHODIMP IDirectInput2A::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInput2A::EnumDevices()");
  
  if (proxy){
      return proxy->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2A::GetDeviceStatus(REFGUID rguidInstance){
  SDL_Log("IDirectInput2A::GetDeviceStatus()");
  
  if (proxy){
      return proxy->GetDeviceStatus(rguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2A::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInput2A::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2A::Initialize(HINSTANCE hinst, DWORD dwVersion){
  SDL_Log("IDirectInput2A::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput2A::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance){
  SDL_Log("IDirectInput2A::FindDevice()");
  
  if (proxy){
      return proxy->FindDevice(rguidClass, ptszName, pguidInstance);
  }
  
  return E_POINTER;
}