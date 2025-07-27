/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInputDevice2W::IDirectInputDevice2W() : m_refCount(1) {
  SDL_Log("IDirectInputDevice2W");
  proxy = new(std::nothrow) IDirectInputDevice8W();
}

IDirectInputDevice2W::~IDirectInputDevice2W() {
  SDL_Log("~IDirectInputDevice2W");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputDevice2W::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputDevice2W::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputDevice2W) ||
     IsEqualGUID(riid, IID_IDirectInputDeviceW)) //Dino Crisis 2 (2000) GOG
  {
    *ppvObject = static_cast<IDirectInputDevice2W*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputDevice2W::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDevice2W::AddRef() {
  SDL_Log("IDirectInputDevice2W::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDevice2W::Release() {
  SDL_Log("IDirectInputDevice2W::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputDevice2W::GetCapabilities(LPDIDEVCAPS lpDIDevCaps){
  SDL_Log("IDirectInputDevice2W::GetCapabilities()");
  
  if (proxy){
      return proxy->GetCapabilities(lpDIDevCaps);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2W::EnumObjects()");
  
  if (proxy){
      return proxy->EnumObjects(lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDevice2W::GetProperty()");
  
  if (proxy){
      return proxy->GetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDevice2W::SetProperty()");
  
  if (proxy){
      return proxy->SetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::Acquire(){
  SDL_Log("IDirectInputDevice2W::Acquire()");
  
  if (proxy){
      return proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::Unacquire(){
  SDL_Log("IDirectInputDevice2W::Unacquire()");
  
  if (proxy){
      return proxy->Unacquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::GetDeviceState(DWORD cbData, LPVOID lpvData){
  SDL_Log("IDirectInputDevice2W::GetDeviceState()");
  
  if (proxy){
      return proxy->GetDeviceState(cbData, lpvData);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2W::GetDeviceData()");
  
  if (proxy){
      return proxy->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::SetDataFormat(LPCDIDATAFORMAT lpdf){
  SDL_Log("IDirectInputDevice2W::SetDataFormat()");
  
  if (proxy){
      return proxy->SetDataFormat(lpdf);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::SetEventNotification(HANDLE hEvent){
  SDL_Log("IDirectInputDevice2W::SetEventNotification()");
  
  if (proxy){
      return proxy->SetEventNotification(hEvent);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::SetCooperativeLevel(HWND hwnd, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2W::SetCooperativeLevel()");
  
  if (proxy){
      return proxy->SetCooperativeLevel(hwnd, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow){
  SDL_Log("IDirectInputDevice2W::GetObjectInfo()");
  
  if (proxy){
      return proxy->GetObjectInfo(pdidoi, dwObj, dwHow);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi){
  SDL_Log("IDirectInputDevice2W::GetDeviceInfo()");
  
  if (proxy){
      return proxy->GetDeviceInfo(pdidi);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2W::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid){
  SDL_Log("IDirectInputDevice2W::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion, rguid);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter){
  SDL_Log("IDirectInputDevice2W::CreateEffect()");
  
  if (proxy){
      return proxy->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType){
  SDL_Log("IDirectInputDevice2W::EnumEffects()");
  
  if (proxy){
      return proxy->EnumEffects(lpCallback, pvRef, dwEffType);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid){
  SDL_Log("IDirectInputDevice2W::GetEffectInfo()");
  
  if (proxy){
      return proxy->GetEffectInfo(pdei, rguid);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::GetForceFeedbackState(LPDWORD pdwOut){
  SDL_Log("IDirectInputDevice2W::GetForceFeedbackState()");
  
  if (proxy){
      return proxy->GetForceFeedbackState(pdwOut);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::SendForceFeedbackCommand(DWORD dwFlags){
  SDL_Log("IDirectInputDevice2W::SendForceFeedbackCommand()");
  
  if (proxy){
      return proxy->SendForceFeedbackCommand(dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl){
  SDL_Log("IDirectInputDevice2W::EnumCreatedEffectObjects()");
  
  if (proxy){
      return proxy->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::Escape(LPDIEFFESCAPE pesc){
  SDL_Log("IDirectInputDevice2W::Escape()");
  
  if (proxy){
      return proxy->Escape(pesc);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::Poll(){
  SDL_Log("IDirectInputDevice2W::Poll()");
  
  if (proxy){
      return proxy->Poll();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2W::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl){
  SDL_Log("IDirectInputDevice2W::SendDeviceData()");
  
  if (proxy){
      return proxy->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
  }
  
  return E_POINTER;
}