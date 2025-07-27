/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInputReading::IGameInputReading() : m_refCount(1) {
  SDL_Log("IGameInputReading");
}

STDMETHODIMP IGameInputReading::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IGameInputReading::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInputReading))
  {
    *ppvObject = static_cast<IGameInputReading*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IGameInputReading::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInputReading::AddRef() {
  SDL_Log("IGameInputReading::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInputReading::Release() {
  SDL_Log("IGameInputReading::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP_(bool) IGameInputReading::GetArcadeStickState(GameInputArcadeStickState* state){
  SDL_Log("IGameInputReading::GetArcadeStickState()");
  return true;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerAxisCount(){
  SDL_Log("IGameInputReading::GetControllerAxisCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerAxisState(uint32_t stateArrayCount, float* stateArray){
  SDL_Log("IGameInputReading::GetControllerAxisState()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerButtonCount(){
  SDL_Log("IGameInputReading::GetControllerButtonCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerButtonState(uint32_t stateArrayCount, bool* stateArray){
  SDL_Log("IGameInputReading::GetControllerButtonState()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerSwitchCount(){
  SDL_Log("IGameInputReading::GetControllerSwitchCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetControllerSwitchState(uint32_t stateArrayCount, GameInputSwitchPosition* stateArray){
  SDL_Log("IGameInputReading::GetControllerSwitchState()");
  return 0;
}

STDMETHODIMP_(void) IGameInputReading::GetDevice(IGameInputDevice** device){
  SDL_Log("IGameInputReading::GetDevice()");
  return;
}

STDMETHODIMP_(bool) IGameInputReading::GetFlightStickState(GameInputFlightStickState* state){
  SDL_Log("IGameInputReading::GetFlightStickState()");
  return true;
}

STDMETHODIMP_(bool) IGameInputReading::GetGamepadState(GameInputGamepadState* state){
  SDL_Log("IGameInputReading::GetGamepadState()");
  return true;
}

STDMETHODIMP_(GameInputKind) IGameInputReading::GetInputKind(){
  SDL_Log("IGameInputReading::GetInputKind()");
  return GameInputKindUnknown;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetKeyCount(){
  SDL_Log("IGameInputReading::GetKeyCount()");
  return 0;
}

STDMETHODIMP_(uint32_t) IGameInputReading::GetKeyState(uint32_t stateArrayCount, GameInputKeyState* stateArray){
  SDL_Log("IGameInputReading::GetKeyState()");
  return 0;
}

STDMETHODIMP_(bool) IGameInputReading::GetMouseState(GameInputMouseState* state){
  SDL_Log("IGameInputReading::GetMouseState()");
  return true;
}

STDMETHODIMP_(bool) IGameInputReading::GetRacingWheelState(GameInputRacingWheelState* state){
  SDL_Log("IGameInputReading::GetRacingWheelState()");
  return true;
}

STDMETHODIMP_(uint64_t) IGameInputReading::GetTimestamp(){
  SDL_Log("IGameInputReading::GetTimestamp()");
  return 0;
}

STDMETHODIMP_(bool) IGameInputReading::GetUiNavigationState(GameInputUiNavigationState* state){
  SDL_Log("IGameInputReading::GetUiNavigationState()");
  return true;
}