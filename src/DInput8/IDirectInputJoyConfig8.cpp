/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput8.h"
#include "../util.h"

IDirectInputJoyConfig8::IDirectInputJoyConfig8() : m_refCount(1) {
  SDL_Log("IDirectInputJoyConfig8");
}

STDMETHODIMP IDirectInputJoyConfig8::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IDirectInputJoyConfig8::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInputJoyConfig8))
  {
    *ppvObject = static_cast<IDirectInputJoyConfig8*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IDirectInputJoyConfig8::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());

  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputJoyConfig8::AddRef() {
  SDL_Log("IDirectInputJoyConfig8::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputJoyConfig8::Release() {
  SDL_Log("IDirectInputJoyConfig8::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IDirectInputJoyConfig8::Acquire(){
  SDL_Log("IDirectInputJoyConfig8::Acquire()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::Unacquire(){
  SDL_Log("IDirectInputJoyConfig8::Unacquire()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::SetCooperativeLevel(HWND  unnamedParam1, DWORD unnamedParam2){
  SDL_Log("IDirectInputJoyConfig8::SetCooperativeLevel()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::SendNotify(){
  SDL_Log("IDirectInputJoyConfig8::SendNotify()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::EnumTypes(LPDIJOYTYPECALLBACK unnamedParam1, LPVOID unnamedParam2){
  SDL_Log("IDirectInputJoyConfig8::EnumTypes()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::GetTypeInfo(LPCWSTR unnamedParam1, LPDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3){
  SDL_Log("IDirectInputJoyConfig8::GetTypeInfo()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::SetTypeInfo(LPCWSTR unnamedParam1, LPCDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3, LPWSTR unnamedParam4){
  SDL_Log("IDirectInputJoyConfig8::SetTypeInfo()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::DeleteType(LPCWSTR unnamedParam1){
  SDL_Log("IDirectInputJoyConfig8::DeleteType()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::GetConfig(UINT uid, LPDIJOYCONFIG pjc, DWORD flags){
  SDL_Log("IDirectInputJoyConfig8::GetConfig() for joystick %u", uid);
  
  if (uid > DIRECTINPUT_USER_MAX_COUNT - 1) {
    SDL_Log("IDirectInputJoyConfig8::GetConfig() > DIERR_NOMOREITEMS");
    return DIERR_NOMOREITEMS;
  }
  if (pjc == nullptr) return E_POINTER;
  
  if (flags && DIJC_GUIDINSTANCE) {
    SDL_Log("IDirectInputJoyConfig8::GetConfig() > DIJC_GUIDINSTANCE");
    
        //To ID the gamepad later on when creating a device, I'm encoding the corresponding playerIndex into the fake guid instance
        pjc->guidInstance = {
        MAKELONG(XBOX360_VID, XBOX360_PID),														    // Data1 (VID + PID)
        0x0000,																					                  // Data2 (reserved)
        0x0000,																					                  // Data3 (reserved)
        { 0x00, (BYTE)uid, 0x50, 0x4C, 0x41, 0x59, 0x45, 0x52 }	          // Data4 (ASCII "PLAYER")
      };
  }
  
  if (flags && DIJC_REGHWCONFIGTYPE) {
    SDL_Log("IDirectInputJoyConfig8::GetConfig() > DIJC_REGHWCONFIGTYPE");
  }
  
  if (flags && DIJC_GAIN) {
    SDL_Log("IDirectInputJoyConfig8::GetConfig() > DIJC_GAIN");
  }
  
  if (flags && DIJC_CALLOUT) {
    SDL_Log("IDirectInputJoyConfig8::GetConfig() > DIJC_CALLOUT");
  }
  
  SDL_Log("IDirectInputJoyConfig8::GetConfig() > DI_OK");
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::SetConfig(UINT unnamedParam1, LPCDIJOYCONFIG unnamedParam2, DWORD unnamedParam3){
  SDL_Log("IDirectInputJoyConfig8::SetConfig()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::DeleteConfig(UINT unnamedParam1){
  SDL_Log("IDirectInputJoyConfig8::DeleteConfig()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::GetUserValues(LPDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2){
  SDL_Log("IDirectInputJoyConfig8::GetUserValues()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::SetUserValues(LPCDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2){
  SDL_Log("IDirectInputJoyConfig8::SetUserValues()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::AddNewHardware(HWND unnamedParam1, REFGUID unnamedParam2){
  SDL_Log("IDirectInputJoyConfig8::AddNewHardware()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::OpenTypeKey(LPCWSTR unnamedParam1, DWORD unnamedParam2, PHKEY unnamedParam3){
  SDL_Log("IDirectInputJoyConfig8::OpenTypeKey()");
  
  return DI_OK;
}

STDMETHODIMP IDirectInputJoyConfig8::OpenAppStatusKey(PHKEY unnamedParam1){
  SDL_Log("IDirectInputJoyConfig8::OpenAppStatusKey()");
  
  return DI_OK;
}
