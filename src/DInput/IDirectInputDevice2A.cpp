/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInputDevice2A::IDirectInputDevice2A() : m_refCount(1) {
  SDL_Log("IDirectInputDevice2A");
  proxy = new(std::nothrow) IDirectInputDevice8A();
}

IDirectInputDevice2A::~IDirectInputDevice2A() {
  SDL_Log("~IDirectInputDevice2A");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputDevice2A::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputDevice2A::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputDevice2A) ||
     IsEqualGUID(riid, IID_IDirectInputDeviceA)) //Dino Crisis 2 (2000) GOG
  {
    *ppvObject = static_cast<IDirectInputDevice2A*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputDevice2A::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDevice2A::AddRef() {
  SDL_Log("IDirectInputDevice2A::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDevice2A::Release() {
  SDL_Log("IDirectInputDevice2A::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputDevice2A::GetCapabilities(LPDIDEVCAPS lpDIDevCaps){
  SDL_Log("IDirectInputDevice2A::GetCapabilities()");
  
  if (proxy){
      return proxy->GetCapabilities(lpDIDevCaps);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2A::EnumObjects()");
  
  if (proxy){
      return proxy->EnumObjects(lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDevice2A::GetProperty()");
  
  if (proxy){
      return proxy->GetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDevice2A::SetProperty()");
  
  if (proxy){
      return proxy->SetProperty(rguidProp, pdiph);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::Acquire(){
  SDL_Log("IDirectInputDevice2A::Acquire()");
  
  if (proxy){
      return proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::Unacquire(){
  SDL_Log("IDirectInputDevice2A::Unacquire()");
  
  if (proxy){
      return proxy->Unacquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::GetDeviceState(DWORD cbData, LPVOID lpvData){
  SDL_Log("IDirectInputDevice2A::GetDeviceState()");
  
  if (proxy){
      return proxy->GetDeviceState(cbData, lpvData);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2A::GetDeviceData()");
  
  if (proxy){
      return proxy->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::SetDataFormat(LPCDIDATAFORMAT lpdf){
  SDL_Log("IDirectInputDevice2A::SetDataFormat()");
  
  if (proxy){
      return proxy->SetDataFormat(lpdf);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::SetEventNotification(HANDLE hEvent){
  SDL_Log("IDirectInputDevice2A::SetEventNotification()");
  
  if (proxy){
      return proxy->SetEventNotification(hEvent);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::SetCooperativeLevel(HWND hwnd, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2A::SetCooperativeLevel()");
  
  if (proxy){
      return proxy->SetCooperativeLevel(hwnd, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow){
  SDL_Log("IDirectInputDevice2A::GetObjectInfo()");
  
  if (proxy){
      return proxy->GetObjectInfo(pdidoi, dwObj, dwHow);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi){
  SDL_Log("IDirectInputDevice2A::GetDeviceInfo()");
  
  if (proxy){
      return proxy->GetDeviceInfo(pdidi);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInputDevice2A::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid){
  SDL_Log("IDirectInputDevice2A::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion, rguid);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter){
  SDL_Log("IDirectInputDevice2A::CreateEffect()");
  
  if (proxy){
      return proxy->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType){
  SDL_Log("IDirectInputDevice2A::EnumEffects()");
  
  if (proxy){
      return proxy->EnumEffects(lpCallback, pvRef, dwEffType);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid){
  SDL_Log("IDirectInputDevice2A::GetEffectInfo()");
  
  if (proxy){
      return proxy->GetEffectInfo(pdei, rguid);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::GetForceFeedbackState(LPDWORD pdwOut){
  SDL_Log("IDirectInputDevice2A::GetForceFeedbackState()");
  
  if (proxy){
      return proxy->GetForceFeedbackState(pdwOut);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::SendForceFeedbackCommand(DWORD dwFlags){
  SDL_Log("IDirectInputDevice2A::SendForceFeedbackCommand()");
  
  if (proxy){
      return proxy->SendForceFeedbackCommand(dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl){
  SDL_Log("IDirectInputDevice2A::EnumCreatedEffectObjects()");
  
  if (proxy){
      return proxy->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::Escape(LPDIEFFESCAPE pesc){
  SDL_Log("IDirectInputDevice2A::Escape()");
  
  if (proxy){
      return proxy->Escape(pesc);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::Poll(){
  SDL_Log("IDirectInputDevice2A::Poll()");
  
  if (proxy){
      return proxy->Poll();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice2A::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl){
  SDL_Log("IDirectInputDevice2A::SendDeviceData()");
  
  if (proxy){
      return proxy->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
  }
  
  return E_POINTER;
}