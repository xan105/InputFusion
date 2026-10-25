/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInputReading::IGameInputReading() : m_refCount(1) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading");
}

STDMETHODIMP IGameInputReading::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInputReading))
  {
    *ppvObject = static_cast<IGameInputReading*>(this);
    AddRef();
    return S_OK;
  }

  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::QueryInterface() > Unknow REFIID: %s", util::GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInputReading::AddRef() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInputReading::Release() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP_(bool) IGameInputReading::GetArcadeStickState(GameInputArcadeStickState* state){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetArcadeStickState()");
  return true;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerAxisCount(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetControllerAxisCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerAxisState(uint32_t stateArrayCount, float* stateArray){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetControllerAxisState()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerButtonCount(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetControllerButtonCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerButtonState(uint32_t stateArrayCount, bool* stateArray){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetControllerButtonState()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerSwitchCount(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetControllerSwitchCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerSwitchState(uint32_t stateArrayCount, GameInputSwitchPosition* stateArray){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetControllerSwitchState()");
  return 0;
}

STDMETHODIMP_(void) IGameInputReading::GetDevice(IGameInputDevice** device){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetDevice()");
  return;
}

STDMETHODIMP_(bool) IGameInputReading::GetFlightStickState(GameInputFlightStickState* state){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetFlightStickState()");
  return true;
}

STDMETHODIMP_(bool) IGameInputReading::GetGamepadState(GameInputGamepadState* state){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetGamepadState()");
  return true;
}

STDMETHODIMP_(GameInputKind) IGameInputReading::GetInputKind(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetInputKind()");
  return GameInputKindUnknown;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetKeyCount(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetKeyCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetKeyState(uint32_t stateArrayCount, GameInputKeyState* stateArray){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetKeyState()");
  return 0;
}

STDMETHODIMP_(bool) IGameInputReading::GetMouseState(GameInputMouseState* state){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetMouseState()");
  return true;
}

STDMETHODIMP_(bool) IGameInputReading::GetRacingWheelState(GameInputRacingWheelState* state){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetRacingWheelState()");
  return true;
}

STDMETHODIMP_(uint64_t) IGameInputReading::GetTimestamp(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetTimestamp()");
  return 0;
}

STDMETHODIMP_(bool) IGameInputReading::GetUiNavigationState(GameInputUiNavigationState* state){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputReading::GetUiNavigationState()");
  return true;
}