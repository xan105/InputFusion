/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInputForceFeedbackEffect::IGameInputForceFeedbackEffect() : m_refCount(1) {
  SDL_Log("IGameInputForceFeedbackEffect");
}

STDMETHODIMP IGameInputForceFeedbackEffect::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IGameInputForceFeedbackEffect::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInputForceFeedbackEffect))
  {
    *ppvObject = static_cast<IGameInputForceFeedbackEffect*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IGameInputForceFeedbackEffect::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInputForceFeedbackEffect::AddRef() {
  SDL_Log("IGameInputForceFeedbackEffect::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInputForceFeedbackEffect::Release() {
  SDL_Log("IGameInputForceFeedbackEffect::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::GetDevice(IGameInputDevice** device){
  SDL_Log("IGameInputForceFeedbackEffect::GetDevice()");
  return;
}

STDMETHODIMP_(float) IGameInputForceFeedbackEffect::GetGain(){
  SDL_Log("IGameInputForceFeedbackEffect::GetGain()");
  return 0.0f;
}

STDMETHODIMP_(uint32_t) IGameInputForceFeedbackEffect::GetMotorIndex(){
  SDL_Log("IGameInputForceFeedbackEffect::GetMotorIndex()");
  return 0;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::GetParams(GameInputForceFeedbackParams* params){
  SDL_Log("IGameInputForceFeedbackEffect::GetParams()");
  return;
}

STDMETHODIMP_(GameInputFeedbackEffectState) IGameInputForceFeedbackEffect::GetState(){
  SDL_Log("IGameInputForceFeedbackEffect::GetState()");
  GameInputFeedbackEffectState state = {};
  return state;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::SetGain(float gain){
  SDL_Log("IGameInputForceFeedbackEffect::SetGain()");
  return;
}

STDMETHODIMP_(bool) IGameInputForceFeedbackEffect::SetParams(const GameInputForceFeedbackParams* params){
  SDL_Log("IGameInputForceFeedbackEffect::SetParams()");
  return true;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::SetState(GameInputFeedbackEffectState state){
  SDL_Log("IGameInputForceFeedbackEffect::SetState()");
  return;
}