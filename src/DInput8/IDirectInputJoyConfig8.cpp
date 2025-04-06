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

STDMETHODIMP IDirectInputJoyConfig8::GetConfig(UINT unnamedParam1, LPDIJOYCONFIG unnamedParam2, DWORD unnamedParam3){
  SDL_Log("IDirectInputJoyConfig8::GetConfig()");
  
  //https://learn.microsoft.com/en-us/windows/win32/api/dinputd/nf-dinputd-idirectinputjoyconfig8-getconfig
  /*
	DIJC_GUIDINSTANCE
	Indicates that the instance GUID for the joystick is being requested. 
	An application can pass the instance GUID to IDirectInput::CreateDevice to obtain an IDirectInputDevice interface to the joystick.
  */

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
