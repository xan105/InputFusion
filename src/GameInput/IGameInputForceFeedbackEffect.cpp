/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInputForceFeedbackEffect::IGameInputForceFeedbackEffect() : m_refCount(1) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect");
}

STDMETHODIMP IGameInputForceFeedbackEffect::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInputForceFeedbackEffect))
  {
    *ppvObject = static_cast<IGameInputForceFeedbackEffect*>(this);
    AddRef();
    return S_OK;
  }

  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::QueryInterface() > Unknow REFIID: %s", util::GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInputForceFeedbackEffect::AddRef() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInputForceFeedbackEffect::Release() {
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::GetDevice(IGameInputDevice** device){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::GetDevice()");
  return;
}

STDMETHODIMP_(float) IGameInputForceFeedbackEffect::GetGain(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::GetGain()");
  return 0.0f;
}

STDMETHODIMP_(uint32_t) IGameInputForceFeedbackEffect::GetMotorIndex(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::GetMotorIndex()");
  return 0;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::GetParams(GameInputForceFeedbackParams* params){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::GetParams()");
  return;
}

STDMETHODIMP_(GameInputFeedbackEffectState) IGameInputForceFeedbackEffect::GetState(){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::GetState()");
  GameInputFeedbackEffectState state = {};
  return state;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::SetGain(float gain){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::SetGain()");
  return;
}

STDMETHODIMP_(bool) IGameInputForceFeedbackEffect::SetParams(const GameInputForceFeedbackParams* params){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::SetParams()");
  return true;
}

STDMETHODIMP_(void) IGameInputForceFeedbackEffect::SetState(GameInputFeedbackEffectState state){
  SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "IGameInputForceFeedbackEffect::SetState()");
  return;
}