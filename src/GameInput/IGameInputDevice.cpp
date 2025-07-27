/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInputDevice::IGameInputDevice() : m_refCount(1) {
  SDL_Log("IGameInputDevice");
}

STDMETHODIMP IGameInputDevice::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IGameInputDevice::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInputDevice))
  {
    *ppvObject = static_cast<IGameInputDevice*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IGameInputDevice::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInputDevice::AddRef() {
  SDL_Log("IGameInputDevice::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInputDevice::Release() {
  SDL_Log("IGameInputDevice::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP IGameInputDevice::CreateForceFeedbackEffect(uint32_t motorIndex, const GameInputForceFeedbackParams* params, IGameInputForceFeedbackEffect** effect) {
  SDL_Log("IGameInputDevice::CreateForceFeedbackEffect()");
  return E_NOTIMPL;
}

STDMETHODIMP IGameInputDevice::GetDeviceInfo(const GameInputDeviceInfo** info) {
  SDL_Log("IGameInputDevice::GetDeviceInfo()");
  return E_NOTIMPL;
}

STDMETHODIMP_(GameInputDeviceStatus) IGameInputDevice::GetDeviceStatus() {
  SDL_Log("IGameInputDevice::GetDeviceStatus()");
  GameInputDeviceStatus status = {};
  return status;
}

STDMETHODIMP_(bool) IGameInputDevice::IsForceFeedbackMotorPoweredOn(uint32_t motorIndex) {
  SDL_Log("IGameInputDevice::IsForceFeedbackMotorPoweredOn()");
  return true;
}

STDMETHODIMP_(void) IGameInputDevice::SetForceFeedbackMotorGain(uint32_t motorIndex, float masterGain) {
  SDL_Log("IGameInputDevice::SetForceFeedbackMotorGain()");
  return;
}

STDMETHODIMP_(void) IGameInputDevice::SetRumbleState(const GameInputRumbleParams* params) {
  SDL_Log("IGameInputDevice::SetRumbleState()");
  return;
}