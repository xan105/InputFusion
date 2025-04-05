/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"

IDirectInputJoyConfig::IDirectInputJoyConfig() : m_refCount(1) {
  SDL_Log("IDirectInputJoyConfig");
  proxy = new(std::nothrow) IDirectInputJoyConfig8();
}

IDirectInputJoyConfig::~IDirectInputJoyConfig() {
  SDL_Log("~IDirectInputJoyConfig");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputJoyConfig::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputJoyConfig::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputJoyConfig))
  {
    *ppvObject = static_cast<IDirectInputJoyConfig*>(this);
    AddRef();
    return S_OK;
  }

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputJoyConfig::AddRef() {
  SDL_Log("IDirectInputJoyConfig::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputJoyConfig::Release() {
  SDL_Log("IDirectInputJoyConfig::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputJoyConfig::Acquire(){
  SDL_Log("IDirectInputJoyConfig::Acquire()");
  
  if(proxy){
    return proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::Unacquire(){
  SDL_Log("IDirectInputJoyConfig::Unacquire()");
  
  if (proxy) {
      return proxy->Unacquire();
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetCooperativeLevel(HWND  unnamedParam1, DWORD unnamedParam2){
  SDL_Log("IDirectInputJoyConfig::SetCooperativeLevel()");
  
  if (proxy) {
      return proxy->SetCooperativeLevel(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SendNotify(){
  SDL_Log("IDirectInputJoyConfig::SendNotify()");
  
  if (proxy) {
      return proxy->SendNotify();
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::EnumTypes(LPDIJOYTYPECALLBACK unnamedParam1, LPVOID unnamedParam2){
  SDL_Log("IDirectInputJoyConfig::EnumTypes()");
  
  if (proxy) {
      return proxy->EnumTypes(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::GetTypeInfo(LPCWSTR unnamedParam1, LPDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3){
  SDL_Log("IDirectInputJoyConfig::GetTypeInfo()");
  
  if (proxy) {
      return proxy->GetTypeInfo(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetTypeInfo(LPCWSTR unnamedParam1, LPCDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3, LPWSTR unnamedParam4){
  SDL_Log("IDirectInputJoyConfig::SetTypeInfo()");
  
  if (proxy) {
      return proxy->SetTypeInfo(unnamedParam1, unnamedParam2, unnamedParam3, unnamedParam4);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::DeleteType(LPCWSTR unnamedParam1){
  SDL_Log("IDirectInputJoyConfig::DeleteType()");
  
  if (proxy) {
      return proxy->DeleteType(unnamedParam1);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::GetConfig(UINT unnamedParam1, LPDIJOYCONFIG unnamedParam2, DWORD unnamedParam3){
  SDL_Log("IDirectInputJoyConfig::GetConfig()");
  
  if (proxy) {
      return proxy->GetConfig(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetConfig(UINT unnamedParam1, LPCDIJOYCONFIG unnamedParam2, DWORD unnamedParam3){
  SDL_Log("IDirectInputJoyConfig::SetConfig()");
  
  if (proxy) {
      return proxy->SetConfig(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::DeleteConfig(UINT unnamedParam1){
  SDL_Log("IDirectInputJoyConfig::DeleteConfig()");
  
  if (proxy) {
      return proxy->DeleteConfig(unnamedParam1);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::GetUserValues(LPDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2){
  SDL_Log("IDirectInputJoyConfig::GetUserValues()");
  
  if (proxy) {
      return proxy->GetUserValues(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetUserValues(LPCDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2){
  SDL_Log("IDirectInputJoyConfig::SetUserValues()");
  
  if (proxy) {
      return proxy->SetUserValues(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::AddNewHardware(HWND unnamedParam1, REFGUID unnamedParam2){
  SDL_Log("IDirectInputJoyConfig::AddNewHardware()");
  
  if (proxy) {
      return proxy->AddNewHardware(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::OpenTypeKey(LPCWSTR unnamedParam1, DWORD unnamedParam2, PHKEY unnamedParam3){
  SDL_Log("IDirectInputJoyConfig::OpenTypeKey()");
  
  if (proxy) {
      return proxy->OpenTypeKey(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::OpenConfigKey(DWORD unnamedParam1, PHKEY unnamedParam2){

    SDL_Log("IDirectInputJoyConfig::OpenConfigKey()");
    
    if (proxy){
        SDL_Log("IDirectInputJoyConfig::OpenConfigKey() -> IDirectInputJoyConfig8::OpenAppStatusKey()");
        proxy->OpenAppStatusKey(unnamedParam2);
    }
 
    return E_POINTER;
}
