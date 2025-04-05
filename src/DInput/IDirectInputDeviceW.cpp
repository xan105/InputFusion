/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"

IDirectInputDeviceW::IDirectInputDeviceW() : m_refCount(1) {
  SDL_Log("IDirectInputDeviceW");
  proxy = new(std::nothrow) IDirectInputDevice8W();
}

IDirectInputDeviceW::~IDirectInputDeviceW() {
  SDL_Log("~IDirectInputDeviceW");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputDeviceW::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputDeviceW::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputDeviceW))
  {
    *ppvObject = static_cast<IDirectInputDeviceW*>(this);
    AddRef();
    return S_OK;
  }
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDeviceW::AddRef() {
  SDL_Log("IDirectInputDeviceW::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDeviceW::Release() {
  SDL_Log("IDirectInputDeviceW::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputDeviceW::GetCapabilities(LPDIDEVCAPS lpDIDevCaps){
  SDL_Log("IDirectInputDeviceW::GetCapabilities()");
  
  if (proxy){
    proxy->GetCapabilities(lpDIDevCaps);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceW::EnumObjects()");
  
  if (proxy){
    proxy->EnumObjects(lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDeviceW::GetProperty()");
  
  if (proxy){
    proxy->GetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDeviceW::SetProperty()");
  
  if (proxy){
    proxy->SetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::Acquire(){
  SDL_Log("IDirectInputDeviceW::Acquire()");
  
  if (proxy){
    proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::Unacquire(){
  SDL_Log("IDirectInputDeviceW::Unacquire()");
  
  if (proxy){
    proxy->Unacquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetDeviceState(DWORD cbData, LPVOID lpvData){
  SDL_Log("IDirectInputDeviceW::GetDeviceState()");
  
  if (proxy){
    proxy->GetDeviceState(cbData, lpvData);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceW::GetDeviceData()");
  
  if (proxy){
    proxy->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetDataFormat(LPCDIDATAFORMAT lpdf){
  SDL_Log("IDirectInputDeviceW::SetDataFormat()");
  
  if (proxy){
    proxy->SetDataFormat(lpdf);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetEventNotification(HANDLE hEvent){
  SDL_Log("IDirectInputDeviceW::SetEventNotification()");
  
  if (proxy){
    proxy->SetEventNotification(hEvent);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetCooperativeLevel(HWND hwnd, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceW::SetCooperativeLevel()");
  
  if (proxy){
    proxy->SetCooperativeLevel(hwnd, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow){
  SDL_Log("IDirectInputDeviceW::GetObjectInfo()");
  
  if (proxy){
    proxy->GetObjectInfo(pdidoi, dwObj, dwHow);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi){
  SDL_Log("IDirectInputDeviceW::GetDeviceInfo()");
  
  if (proxy){
    proxy->GetDeviceInfo(pdidi);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceW::RunControlPanel()");
  
  if (proxy){
    proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid){
  SDL_Log("IDirectInputDeviceW::Initialize()");
  
  if (proxy){
    proxy->Initialize(hinst, dwVersion, rguid);
  }
  
  return E_POINTER;
}