/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInputDeviceA::IDirectInputDeviceA() : m_refCount(1) {
  SDL_Log("IDirectInputDeviceA");
  proxy = new(std::nothrow) IDirectInputDevice8A();
}

IDirectInputDeviceA::~IDirectInputDeviceA() {
  SDL_Log("~IDirectInputDeviceA");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputDeviceA::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputDeviceA::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputDeviceA))
  {
    *ppvObject = static_cast<IDirectInputDeviceA*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputDeviceA::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDeviceA::AddRef() {
  SDL_Log("IDirectInputDeviceA::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDeviceA::Release() {
  SDL_Log("IDirectInputDeviceA::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputDeviceA::GetCapabilities(LPDIDEVCAPS lpDIDevCaps){
  SDL_Log("IDirectInputDeviceA::GetCapabilities()");
  
  if (proxy){
      return proxy->GetCapabilities(lpDIDevCaps);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceA::EnumObjects()");
  
  if (proxy){
      return proxy->EnumObjects(lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDeviceA::GetProperty()");
  
  if (proxy){
      return proxy->GetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDeviceA::SetProperty()");
  
  if (proxy){
      return proxy->SetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::Acquire(){
  SDL_Log("IDirectInputDeviceA::Acquire()");
  
  if (proxy){
      return proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::Unacquire(){
  SDL_Log("IDirectInputDeviceA::Unacquire()");
  
  if (proxy){
      return proxy->Unacquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::GetDeviceState(DWORD cbData, LPVOID lpvData){
  SDL_Log("IDirectInputDeviceA::GetDeviceState()");
  
  if (proxy){
      return proxy->GetDeviceState(cbData, lpvData);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceA::GetDeviceData()");
  
  if (proxy){
      return proxy->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::SetDataFormat(LPCDIDATAFORMAT lpdf){
  SDL_Log("IDirectInputDeviceA::SetDataFormat()");
  
  if (proxy){
      return proxy->SetDataFormat(lpdf);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::SetEventNotification(HANDLE hEvent){
  SDL_Log("IDirectInputDeviceA::SetEventNotification()");
  
  if (proxy){
      return proxy->SetEventNotification(hEvent);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::SetCooperativeLevel(HWND hwnd, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceA::SetCooperativeLevel()");
  
  if (proxy){
      return proxy->SetCooperativeLevel(hwnd, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow){
  SDL_Log("IDirectInputDeviceA::GetObjectInfo()");
  
  if (proxy){
      return proxy->GetObjectInfo(pdidoi, dwObj, dwHow);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi){
  SDL_Log("IDirectInputDeviceA::GetDeviceInfo()");
  
  if (proxy){
      return proxy->GetDeviceInfo(pdidi);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInputDeviceA::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDeviceA::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid){
  SDL_Log("IDirectInputDeviceA::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion, rguid);
  }
  
  return E_POINTER;
}