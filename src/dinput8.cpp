/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "dinput8.h"

#ifdef DINPUT8_EXPORTS
extern "C" {
#endif
  HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter){
    SDL_Log("DirectInput8Create()");

    if(hinst == nullptr || ppvOut == nullptr)
      return DIERR_INVALIDPARAM;
    
    if (dwVersion < DIRECTINPUT_VERSION)
      return DIERR_OLDDIRECTINPUTVERSION;

    if (riidltf == IID_IDirectInput8A) {
      IDirectInput8A* pDInput = new(std::nothrow) IDirectInput8A;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInput8A*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, DIRECTINPUT_VERSION);
      if (FAILED(hr)) { return hr; }

      return DI_OK;
    }
    else if (riidltf == IID_IDirectInput8W) {
      IDirectInput8W* pDInput = new(std::nothrow) IDirectInput8W;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInput8W*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, DIRECTINPUT_VERSION);
      if (FAILED(hr)) { return hr; }

      return DI_OK;
    }
    
    return DIERR_INVALIDPARAM;
  }

  HRESULT WINAPI DllCanUnloadNow(){
    return S_OK;
  }
  
  HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv){
    return CLASS_E_CLASSNOTAVAILABLE;
  }
  
  HRESULT WINAPI DllRegisterServer(){
    return NULL;
  }
  
  HRESULT WINAPI DllUnregisterServer(){
    return S_OK;
  }
#ifdef DINPUT8_EXPORTS
}
#endif