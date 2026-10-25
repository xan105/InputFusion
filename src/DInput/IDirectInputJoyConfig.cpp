/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"
#include "../util.h"

IDirectInputJoyConfig::IDirectInputJoyConfig() : m_refCount(1) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig");
  proxy = new(std::nothrow) IDirectInputJoyConfig8();
}

IDirectInputJoyConfig::~IDirectInputJoyConfig() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "~IDirectInputJoyConfig");
  if (proxy) { proxy->Release(); }
}

STDMETHODIMP IDirectInputJoyConfig::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputJoyConfig))
  {
    *ppvObject = static_cast<IDirectInputJoyConfig*>(this);
    AddRef();
    return S_OK;
  }

  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::QueryInterface() > Unknow REFIID: %s", util::GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputJoyConfig::AddRef() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputJoyConfig::Release() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputJoyConfig::Acquire(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::Acquire()");
  
  if(proxy){
    return proxy->Acquire();
  }
  
  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::Unacquire(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::Unacquire()");
  
  if (proxy) {
      return proxy->Unacquire();
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetCooperativeLevel(HWND  unnamedParam1, DWORD unnamedParam2){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::SetCooperativeLevel()");
  
  if (proxy) {
      return proxy->SetCooperativeLevel(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SendNotify(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::SendNotify()");
  
  if (proxy) {
      return proxy->SendNotify();
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::EnumTypes(LPDIJOYTYPECALLBACK unnamedParam1, LPVOID unnamedParam2){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::EnumTypes()");
  
  if (proxy) {
      return proxy->EnumTypes(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::GetTypeInfo(LPCWSTR unnamedParam1, LPDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::GetTypeInfo()");
  
  if (proxy) {
      return proxy->GetTypeInfo(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetTypeInfo(LPCWSTR unnamedParam1, LPCDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3, LPWSTR unnamedParam4){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::SetTypeInfo()");
  
  if (proxy) {
      return proxy->SetTypeInfo(unnamedParam1, unnamedParam2, unnamedParam3, unnamedParam4);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::DeleteType(LPCWSTR unnamedParam1){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::DeleteType()");
  
  if (proxy) {
      return proxy->DeleteType(unnamedParam1);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::GetConfig(UINT unnamedParam1, LPDIJOYCONFIG unnamedParam2, DWORD unnamedParam3){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::GetConfig()");
  
  if (proxy) {
      return proxy->GetConfig(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetConfig(UINT unnamedParam1, LPCDIJOYCONFIG unnamedParam2, DWORD unnamedParam3){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::SetConfig()");
  
  if (proxy) {
      return proxy->SetConfig(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::DeleteConfig(UINT unnamedParam1){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::DeleteConfig()");
  
  if (proxy) {
      return proxy->DeleteConfig(unnamedParam1);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::GetUserValues(LPDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::GetUserValues()");
  
  if (proxy) {
      return proxy->GetUserValues(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::SetUserValues(LPCDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::SetUserValues()");
  
  if (proxy) {
      return proxy->SetUserValues(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::AddNewHardware(HWND unnamedParam1, REFGUID unnamedParam2){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::AddNewHardware()");
  
  if (proxy) {
      return proxy->AddNewHardware(unnamedParam1, unnamedParam2);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::OpenTypeKey(LPCWSTR unnamedParam1, DWORD unnamedParam2, PHKEY unnamedParam3){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::OpenTypeKey()");
  
  if (proxy) {
      return proxy->OpenTypeKey(unnamedParam1, unnamedParam2, unnamedParam3);
  }

  return E_POINTER;
}

STDMETHODIMP IDirectInputJoyConfig::OpenConfigKey(DWORD unnamedParam1, PHKEY unnamedParam2){

    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::OpenConfigKey()");
    
    if (proxy){
        SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IDirectInputJoyConfig::OpenConfigKey() -> IDirectInputJoyConfig8::OpenAppStatusKey()");
        return proxy->OpenAppStatusKey(unnamedParam2);
    }
 
    return E_POINTER;
}
