/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"

IDirectInputDeviceW::IDirectInputDeviceW() : m_refCount(1) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW");
  proxy = new(std::nothrow) IDirectInputDevice8W();
}

IDirectInputDeviceW::~IDirectInputDeviceW() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "~IDirectInputDeviceW");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputDeviceW::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::QueryInterface()");

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
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDeviceW::Release() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputDeviceW::GetCapabilities(LPDIDEVCAPS lpDIDevCaps){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::GetCapabilities()");
  
  if (proxy){
      return proxy->GetCapabilities(lpDIDevCaps);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::EnumObjects()");
  
  if (proxy){
      return proxy->EnumObjects(lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::GetProperty()");
  
  if (proxy){
      return proxy->GetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::SetProperty()");
  
  if (proxy){
      return proxy->SetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::Acquire(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::Acquire()");
  
  if (proxy){
      return proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::Unacquire(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::Unacquire()");
  
  if (proxy){
      return proxy->Unacquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetDeviceState(DWORD cbData, LPVOID lpvData){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::GetDeviceState()");
  
  if (proxy){
      return proxy->GetDeviceState(cbData, lpvData);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::GetDeviceData()");
  
  if (proxy){
      return proxy->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetDataFormat(LPCDIDATAFORMAT lpdf){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::SetDataFormat()");
  
  if (proxy){
      return proxy->SetDataFormat(lpdf);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetEventNotification(HANDLE hEvent){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::SetEventNotification()");
  
  if (proxy){
      return proxy->SetEventNotification(hEvent);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::SetCooperativeLevel(HWND hwnd, DWORD dwFlags){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::SetCooperativeLevel()");
  
  if (proxy){
      return proxy->SetCooperativeLevel(hwnd, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::GetObjectInfo()");
  
  if (proxy){
      return proxy->GetObjectInfo(pdidoi, dwObj, dwHow);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::GetDeviceInfo()");
  
  if (proxy){
      return proxy->GetDeviceInfo(pdidi);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceW::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputDeviceW::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion, rguid);
  }
  
  return E_POINTER;
}