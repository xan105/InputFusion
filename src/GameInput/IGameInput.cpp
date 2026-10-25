/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInput::IGameInput() : m_refCount(1) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput");
}

STDMETHODIMP IGameInput::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInput))
  {
    *ppvObject = static_cast<IGameInput*>(this);
    AddRef();
    return S_OK;
  }

  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::QueryInterface() > Unknow REFIID: %s", util::GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInput::AddRef() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInput::Release() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IGameInput::CreateDispatcher(IGameInputDispatcher** dispatcher){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::CreateDispatcher()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::FindDeviceFromId(const APP_LOCAL_DEVICE_ID* value, IGameInputDevice** device){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::FindDeviceFromId()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::FindDeviceFromPlatformString(LPCWSTR value, IGameInputDevice** device){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::FindDeviceFromPlatformString()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::GetCurrentReading(GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::GetCurrentReading()");
  return E_NOTIMPL;
}

STDMETHODIMP_(uint64_t) IGameInput::GetCurrentTimestamp(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::GetCurrentTimestamp()");
  return 0;
}

STDMETHODIMP IGameInput::GetNextReading(IGameInputReading* referenceReading, GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::GetNextReading()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::GetPreviousReading(IGameInputReading* referenceReading, GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::GetPreviousReading()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterDeviceCallback(IGameInputDevice* device, GameInputKind inputKind, GameInputDeviceStatus statusFilter, GameInputEnumerationKind enumerationKind, void* context, GameInputDevice_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::RegisterDeviceCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterKeyboardLayoutCallback(IGameInputDevice* device, void* context, GameInputKeyboardLayout_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::RegisterKeyboardLayoutCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterReadingCallback(IGameInputDevice* device, GameInputKind inputKind, void* context, GameInputReading_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::RegisterReadingCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInput::RegisterSystemButtonCallback(IGameInputDevice* device, GameInputSystemButtons buttonFilter, void* context, GameInputSystemButton_Callback callbackFunc, GameInputCallbackToken* callbackToken){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::RegisterSystemButtonCallback()");
  return E_NOTIMPL;
}

STDMETHODIMP_(void) IGameInput::SetFocusPolicy(GameInputFocusPolicy policy){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::SetFocusPolicy()");
  return;
}

STDMETHODIMP_(void) IGameInput::StopCallback(GameInputCallbackToken callbackToken){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::StopCallback()");
  return;
}

STDMETHODIMP_(bool) IGameInput::UnregisterCallback(GameInputCallbackToken callbackToken){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInput::UnregisterCallback()");
  return true;
}