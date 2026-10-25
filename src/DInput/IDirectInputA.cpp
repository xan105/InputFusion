/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"

IDirectInputA::IDirectInputA() : m_refCount(1) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA");
  proxy = new(std::nothrow) IDirectInput8A();
}

IDirectInputA::~IDirectInputA() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "~IDirectInputA");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputA::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::QueryInterface()");

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
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputA::Release() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputA::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICEA *lplpDirectInputDevice, LPUNKNOWN pUnkOuter){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::CreateDevice()");
  
  IDirectInputDeviceA* pDInputDevice = new(std::nothrow) IDirectInputDeviceA;
  if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
  *lplpDirectInputDevice = static_cast<IDirectInputDeviceA*>(pDInputDevice);

  HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION_300, rguid);
  if (FAILED(hr)) { return hr; }

  return DI_OK;
}

STDMETHODIMP IDirectInputA::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::EnumDevices()");
  
  if (proxy){
      return proxy->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputA::GetDeviceStatus(REFGUID rguidInstance){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::GetDeviceStatus()");
  
  if (proxy){
      return proxy->GetDeviceStatus(rguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputA::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputA::Initialize(HINSTANCE hinst, DWORD dwVersion){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputA::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion);
  }
  
  return E_POINTER;
}