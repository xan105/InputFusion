/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"

IDirectInputA::IDirectInputA() : m_refCount(1) {
  SDL_Log("IDirectInputA");
  proxy = new(std::nothrow) IDirectInput8A();
}

IDirectInputA::~IDirectInputA() {
  SDL_Log("~IDirectInputA");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputA::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputA::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputA))
  {
    *ppvObject = static_cast<IDirectInputA*>(this);
    AddRef();
    return S_OK;
  }

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputA::AddRef() {
  SDL_Log("IDirectInputA::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputA::Release() {
  SDL_Log("IDirectInputA::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputA::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICEA *lplpDirectInputDevice, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInputA::CreateDevice()");
  
  if (proxy){
    proxy->CreateDevice(rguid, (LPDIRECTINPUTDEVICE8A *)lplpDirectInputDevice, pUnkOuter);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputA::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInputA::EnumDevices()");
  
  if (proxy){
    proxy->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputA::GetDeviceStatus(REFGUID rguidInstance){
  SDL_Log("IDirectInputA::GetDeviceStatus()");
  
  if (proxy){
    proxy->GetDeviceStatus(rguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputA::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInputA::RunControlPanel()");
  
  if (proxy){
    proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputA::Initialize(HINSTANCE hinst, DWORD dwVersion){
  SDL_Log("IDirectInputA::Initialize()");
  
  if (proxy){
    proxy->Initialize(hinst, dwVersion);
  }
  
  return E_POINTER;
}