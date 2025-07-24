/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInput7A::IDirectInput7A() : m_refCount(1) {
  SDL_Log("IDirectInput7A");
  proxy = new(std::nothrow) IDirectInput8A();
}

IDirectInput7A::~IDirectInput7A() {
  SDL_Log("~IDirectInput7A");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInput7A::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInput7A::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInput7A))
  {
    *ppvObject = static_cast<IDirectInput7A*>(this);
    AddRef();
    return S_OK;
  }
  
  if (IsEqualGUID(riid, IID_IDirectInputJoyConfig)) { //Resident Evil (1996) GOG
    SDL_Log("IDirectInput7A::QueryInterface() > IDirectInputJoyConfig");

    IDirectInputJoyConfig* pDInputJoyConfig = new(std::nothrow) IDirectInputJoyConfig;
    if (pDInputJoyConfig == nullptr) return E_NOINTERFACE;
    *ppvObject = static_cast<IDirectInputJoyConfig*>(pDInputJoyConfig);

    return S_OK;
  }

  SDL_Log("IDirectInput7A::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInput7A::AddRef() {
  SDL_Log("IDirectInput7A::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInput7A::Release() {
  SDL_Log("IDirectInput7A::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInput7A::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE7A *lplpDirectInputDevice, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInput7A::CreateDevice() <%s>", GUIDToString(rguid).c_str());

  IDirectInputDevice7A* pDInputDevice = new(std::nothrow) IDirectInputDevice7A;
  if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
  *lplpDirectInputDevice = static_cast<IDirectInputDevice7A*>(pDInputDevice);

  HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION_700, rguid);
  if (FAILED(hr)) { return hr; }

  return DI_OK;
}

STDMETHODIMP IDirectInput7A::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInput7A::EnumDevices()");
  
  if (proxy){
      return proxy->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7A::GetDeviceStatus(REFGUID rguidInstance){
  SDL_Log("IDirectInput7A::GetDeviceStatus()");
  
  if (proxy){
      return proxy->GetDeviceStatus(rguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7A::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInput7A::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7A::Initialize(HINSTANCE hinst, DWORD dwVersion){
  SDL_Log("IDirectInput7A::Initialize()");
  
  if (proxy){
    return proxy->Initialize(hinst, dwVersion);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7A::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance){
  SDL_Log("IDirectInput7A::FindDevice()");
  
  if (proxy){
      return proxy->FindDevice(rguidClass, ptszName, pguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7A::CreateDeviceEx(REFGUID rguid, REFIID riid, LPVOID * ppvObj, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInput7A::CreateDeviceEx()");
  
  if (proxy){
    SDL_Log("IDirectInput7A::CreateDeviceEx() -> IDirectInput8A::CreateDevice()");
    return proxy->CreateDevice(rguid, (LPDIRECTINPUTDEVICE8A *)ppvObj, pUnkOuter);
  }
  
  return E_POINTER;
}