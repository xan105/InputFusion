/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInputDevice7W::IDirectInputDevice7W() : m_refCount(1) {
  SDL_Log("IDirectInputDevice7W");
  proxy = new(std::nothrow) IDirectInputDevice8W();
}

IDirectInputDevice7W::~IDirectInputDevice7W() {
  SDL_Log("~IDirectInputDevice7W");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputDevice7W::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputDevice7W::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputDevice7W) ||
     IsEqualGUID(riid, IID_IDirectInputDevice2W) || //Dino Crisis 2 (2000) GOG
     IsEqualGUID(riid, IID_IDirectInputDeviceW))
  {
    *ppvObject = static_cast<IDirectInputDevice7W*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputDevice7W::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDevice7W::AddRef() {
  SDL_Log("IDirectInputDevice7W::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDevice7W::Release() {
  SDL_Log("IDirectInputDevice7W::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputDevice7W::GetCapabilities(LPDIDEVCAPS lpDIDevCaps){
  SDL_Log("IDirectInputDevice7W::GetCapabilities()");
  
  if (proxy){
      return proxy->GetCapabilities(lpDIDevCaps);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags){
  SDL_Log("IDirectInputDevice7W::EnumObjects()");
  
  if (proxy){
      return proxy->EnumObjects(lpCallback, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDevice7W::GetProperty()");
  
  if (proxy){
      return proxy->GetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph){
  SDL_Log("IDirectInputDevice7W::SetProperty()");
  
  if (proxy){
      return proxy->SetProperty(rguidProp, pdiph);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::Acquire(){
  SDL_Log("IDirectInputDevice7W::Acquire()");
  
  if (proxy){
      return proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::Unacquire(){
  SDL_Log("IDirectInputDevice7W::Unacquire()");
  
  if (proxy){
      return proxy->Unacquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::GetDeviceState(DWORD cbData, LPVOID lpvData){
  SDL_Log("IDirectInputDevice7W::GetDeviceState()");
  
  if (proxy){
      return proxy->GetDeviceState(cbData, lpvData);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags){
  SDL_Log("IDirectInputDevice7W::GetDeviceData()");
  
  if (proxy){
      return proxy->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::SetDataFormat(LPCDIDATAFORMAT lpdf){
  SDL_Log("IDirectInputDevice7W::SetDataFormat()");
  
  if (proxy){
      return proxy->SetDataFormat(lpdf);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::SetEventNotification(HANDLE hEvent){
  SDL_Log("IDirectInputDevice7W::SetEventNotification()");
  
  if (proxy){
      return proxy->SetEventNotification(hEvent);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::SetCooperativeLevel(HWND hwnd, DWORD dwFlags){
  SDL_Log("IDirectInputDevice7W::SetCooperativeLevel()");
  
  if (proxy){
      return proxy->SetCooperativeLevel(hwnd, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow){
  SDL_Log("IDirectInputDevice7W::GetObjectInfo()");
  
  if (proxy){
      return proxy->GetObjectInfo(pdidoi, dwObj, dwHow);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi){
  SDL_Log("IDirectInputDevice7W::GetDeviceInfo()");
  
  if (proxy){
      return proxy->GetDeviceInfo(pdidi);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::RunControlPanel(HWND hwndOwner, DWORD dwFlags){
  SDL_Log("IDirectInputDevice7W::RunControlPanel()");
  
  if (proxy){
      return proxy->RunControlPanel(hwndOwner, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid){
  SDL_Log("IDirectInputDevice7W::Initialize()");
  
  if (proxy){
      return proxy->Initialize(hinst, dwVersion, rguid);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter){
  SDL_Log("IDirectInputDevice7W::CreateEffect()");
  
  if (proxy){
      return proxy->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType){
  SDL_Log("IDirectInputDevice7W::EnumEffects()");
  
  if (proxy){
      return proxy->EnumEffects(lpCallback, pvRef, dwEffType);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid){
  SDL_Log("IDirectInputDevice7W::GetEffectInfo()");
  
  if (proxy){
      return proxy->GetEffectInfo(pdei, rguid);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::GetForceFeedbackState(LPDWORD pdwOut){
  SDL_Log("IDirectInputDevice7W::GetForceFeedbackState()");
  
  if (proxy){
      return proxy->GetForceFeedbackState(pdwOut);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::SendForceFeedbackCommand(DWORD dwFlags){
  SDL_Log("IDirectInputDevice7W::SendForceFeedbackCommand()");
  
  if (proxy){
      return proxy->SendForceFeedbackCommand(dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl){
  SDL_Log("IDirectInputDevice7W::EnumCreatedEffectObjects()");
  
  if (proxy){
      return proxy->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::Escape(LPDIEFFESCAPE pesc){
  SDL_Log("IDirectInputDevice7W::Escape()");
  
  if (proxy){
      return proxy->Escape(pesc);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::Poll(){
  SDL_Log("IDirectInputDevice7W::Poll()");
  
  if (proxy){
      return proxy->Poll();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl){
  SDL_Log("IDirectInputDevice7W::SendDeviceData()");
  
  if (proxy){
      return proxy->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::EnumEffectsInFile(LPCWSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
  SDL_Log("IDirectInputDevice7W::EnumEffectsInFile()");
  
  if (proxy){
      return proxy->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputDevice7W::WriteEffectToFile(LPCWSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
  SDL_Log("IDirectInputDevice7W::WriteEffectToFile()");
  
  if (proxy){
      return proxy->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
  }
  
  return E_POINTER;
}