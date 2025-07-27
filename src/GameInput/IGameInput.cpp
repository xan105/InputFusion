/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInput::IGameInput() : m_refCount(1) {
  SDL_Log("IGameInput");
}

STDMETHODIMP IGameInput::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IGameInput::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInput))
  {
    *ppvObject = static_cast<IGameInput*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IGameInput::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInput::AddRef() {
  SDL_Log("IGameInput::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInput::Release() {
  SDL_Log("IGameInput::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IGameInput::CreateDispatcher(IGameInputDispatcher** dispatcher){
  SDL_Log("IGameInput::CreateDispatcher()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::FindDeviceFromId(const APP_LOCAL_DEVICE_ID* value, IGameInputDevice** device){
  SDL_Log("IGameInput::FindDeviceFromId()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::FindDeviceFromPlatformString(LPCWSTR value, IGameInputDevice** device){
  SDL_Log("IGameInput::FindDeviceFromPlatformString()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::GetCurrentReading(GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading){
  SDL_Log("IGameInput::GetCurrentReading()");
  return E_NOTIMPL;
}

STDMETHODIMP_(uint64_t) IGameInput::GetCurrentTimestamp(){
  SDL_Log("IGameInput::GetCurrentTimestamp()");
  return 0;
}

STDMETHODIMP IGameInput::GetNextReading(IGameInputReading* referenceReading, GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading){
  SDL_Log("IGameInput::GetNextReading()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::GetPreviousReading(IGameInputReading* referenceReading, GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading){
  SDL_Log("IGameInput::GetPreviousReading()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterDeviceCallback(IGameInputDevice* device, GameInputKind inputKind, GameInputDeviceStatus statusFilter, GameInputEnumerationKind enumerationKind, void* context, GameInputDevice_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_Log("IGameInput::RegisterDeviceCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterKeyboardLayoutCallback(IGameInputDevice* device, void* context, GameInputKeyboardLayout_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_Log("IGameInput::RegisterKeyboardLayoutCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterReadingCallback(IGameInputDevice* device, GameInputKind inputKind, void* context, GameInputReading_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_Log("IGameInput::RegisterReadingCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterSystemButtonCallback(IGameInputDevice* device, GameInputSystemButtons buttonFilter, void* context, GameInputSystemButton_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_Log("IGameInput::RegisterSystemButtonCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP_(void) IGameInput::SetFocusPolicy(GameInputFocusPolicy policy){
  SDL_Log("IGameInput::SetFocusPolicy()");
  return;
}

STDMETHODIMP_(void) IGameInput::StopCallback(GameInputCallbackToken callbackToken){
  SDL_Log("IGameInput::StopCallback()");
  return;
}

STDMETHODIMP_(bool) IGameInput::UnregisterCallback(GameInputCallbackToken callbackToken){
  SDL_Log("IGameInput::UnregisterCallback()");
  return true;
}