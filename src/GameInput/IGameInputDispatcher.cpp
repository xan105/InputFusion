/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "GameInput.h"
#include "../util.h"

IGameInputDispatcher::IGameInputDispatcher() : m_refCount(1) {
  SDL_Log("IGameInputDispatcher");
}

STDMETHODIMP IGameInputDispatcher::QueryInterface(REFIID riid, void** ppvObject) {
  SDL_Log("IGameInputDispatcher::QueryInterface()");

  if (ppvObject == nullptr)
    return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IGameInputDispatcher))
  {
    *ppvObject = static_cast<IGameInputDispatcher*>(this);
    AddRef();
    return S_OK;
  }

  SDL_Log("IGameInputDispatcher::QueryInterface() > Unknow REFIID: %s", GUIDToString(riid).c_str());
  
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IGameInputDispatcher::AddRef() {
  SDL_Log("IGameInputDispatcher::AddRef()");

  return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IGameInputDispatcher::Release() {
  SDL_Log("IGameInputDispatcher::Release()");

  ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
  if (refCount == 0) {
    delete this;
  }
  return refCount;
}

STDMETHODIMP_(bool) IGameInputDispatcher::Dispatch(uint64_t quotaInMicroseconds){
  SDL_Log("IGameInputDispatcher::Dispatch()");
  return true;
}

STDMETHODIMP IGameInputDispatcher::OpenWaitHandle(HANDLE* waitHandle){
  SDL_Log("IGameInputDispatcher::OpenWaitHandle()");
  return E_NOTIMPL;
}