/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInput7W::IDirectInput7W() : m_refCount(1) {
  SDL_Log("IDirectInput7W");
  proxy = new(std::nothrow) IDirectInput8W();
}

IDirectInput7W::~IDirectInput7W() {
  SDL_Log("~IDirectInput7W");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInput7W::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInput7W::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInput7W))
  {
    *ppvObject = static_cast<IDirectInput7W*>(this);
    AddRef();
    return S_OK;
  }
  
  if (IsEqualGUID(riid, IID_IDirectInputJoyConfig)) { //Resident Evil (1996) GOG
    SDL_Log("IDirectInput7W::QueryInterface() > IDirectInputJoyConfig");

    IDirectInputJoyConfig* pDInputJoyConfig = new(std::nothrow) IDirectInputJoyConfig;
    if (pDInputJoyConfig == nullptr) return E_NOINTERFACE;
    *ppvObject = static_cast<IDirectInputJoyConfig*>(pDInputJoyConfig);

    return S_OK;
  }

  SDL_Log("IDirectInput7W::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInput7W::AddRef() {
  SDL_Log("IDirectInput7W::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInput7W::Release() {
  SDL_Log("IDirectInput7W::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInput7W::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE7W *lplpDirectInputDevice, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInput7W::CreateDevice()");
  
  IDirectInputDevice7W* pDInputDevice = new(std::nothrow) IDirectInputDevice7W;
  if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
  *lplpDirectInputDevice = static_cast<IDirectInputDevice7W*>(pDInputDevice);

  HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION_700, rguid);
  if (FAILED(hr)) { return hr; }

  return DI_OK;
}

STDMETHODIMP IDirectInput7W::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInput7W::EnumDevices()");
  
  if (proxy){
      return proxy->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7W::GetDeviceStatus(REFGUID rguidInstance){
  SDL_Log("IDirectInput7W::GetDeviceStatus()");
  
  if (proxy){
      return proxy->GetDeviceStatus(rguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7W::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInput7W::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7W::Initialize(HINSTANCE hinst, DWORD dwVersion){
  SDL_Log("IDirectInput7W::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7W::FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance){
  SDL_Log("IDirectInput7W::FindDevice()");
  
  if (proxy){
      return proxy->FindDevice(rguidClass, ptszName, pguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInput7W::CreateDeviceEx(REFGUID rguid, REFIID riid, LPVOID * ppvObj, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInput7W::CreateDeviceEx()");
  
  if (proxy){
    SDL_Log("IDirectInput7W::CreateDeviceEx() -> IDirectInput8W::CreateDevice()");
    return proxy->CreateDevice(rguid, (LPDIRECTINPUTDEVICE8W *)ppvObj, pUnkOuter);
  }
  
  return E_POINTER;
}