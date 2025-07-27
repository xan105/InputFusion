/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInputW::IDirectInputW() : m_refCount(1) {
  SDL_Log("IDirectInputW");
  proxy = new(std::nothrow) IDirectInput8W();
}

IDirectInputW::~IDirectInputW() {
  SDL_Log("~IDirectInputW");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputW::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputW::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputW))
  {
    *ppvObject = static_cast<IDirectInputW*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputDeviceW::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputW::AddRef() {
  SDL_Log("IDirectInputW::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputW::Release() {
  SDL_Log("IDirectInputW::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputW::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICEW *lplpDirectInputDevice, LPUNKNOWN pUnkOuter){
  SDL_Log("IDirectInputW::CreateDevice()");
  
  IDirectInputDeviceW* pDInputDevice = new(std::nothrow) IDirectInputDeviceW;
  if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
  *lplpDirectInputDevice = static_cast<IDirectInputDeviceW*>(pDInputDevice);

  HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION_300, rguid);
  if (FAILED(hr)) { return hr; }

  return DI_OK;
}

STDMETHODIMP IDirectInputW::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInputW::EnumDevices()");
  
  if (proxy){
      return proxy->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputW::GetDeviceStatus(REFGUID rguidInstance){
  SDL_Log("IDirectInputW::GetDeviceStatus()");
  
  if (proxy){
      return proxy->GetDeviceStatus(rguidInstance);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputW::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInputW::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputW::Initialize(HINSTANCE hinst, DWORD dwVersion){
  SDL_Log("IDirectInputW::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion);
  }
  
  return E_POINTER;
}