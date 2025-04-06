/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput.h"

#ifdef DINPUT_EXPORTS
extern "C" {
#endif

HRESULT WINAPI DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    SDL_Log("DirectInputCreateA()");

    return DirectInputCreateEx(hinst, dwVersion, IID_IDirectInput7A, ppvOut, punkOuter);
}

HRESULT WINAPI DirectInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    SDL_Log("DirectInputCreateW()");
  
    return DirectInputCreateEx(hinst, dwVersion, IID_IDirectInput7W, ppvOut, punkOuter);
}

HRESULT WINAPI DirectInputCreateEx(HINSTANCE hinst, DWORD dwVersion, REFIID riid, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    SDL_Log("DirectInputCreateEx()");

    if(hinst == nullptr || ppvOut == nullptr)
      return DIERR_INVALIDPARAM;

    if (IsEqualGUID(riid, IID_IDirectInputA))
    {
      IDirectInputA* pDInput = new(std::nothrow) IDirectInputA;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInputA*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, dwVersion);
      if (FAILED(hr)) { return hr; }

      return DI_OK;
    }
    else if (IsEqualGUID(riid, IID_IDirectInputW))
    {
      IDirectInputW* pDInput = new(std::nothrow) IDirectInputW;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInputW*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, dwVersion);
      if (FAILED(hr)) { return hr; }

      return DI_OK;
    }
    else if (IsEqualGUID(riid, IID_IDirectInput2A))
    {
      IDirectInput2A* pDInput = new(std::nothrow) IDirectInput2A;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInput2A*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, dwVersion);
      if (FAILED(hr)) { return hr; }

      return DI_OK;
    }
    else if (IsEqualGUID(riid, IID_IDirectInput2W))
    {
      IDirectInput2W* pDInput = new(std::nothrow) IDirectInput2W;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInput2W*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, dwVersion);
      if (FAILED(hr)) { return hr; }

      return DI_OK;
    }
    else if (IsEqualGUID(riid, IID_IDirectInput7A))
    {
      IDirectInput7A* pDInput = new(std::nothrow) IDirectInput7A;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInput7A*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, dwVersion);
      if (FAILED(hr)) { return hr; }

      SDL_Log("DI_OK");

      return DI_OK;
    } 
    else if (IsEqualGUID(riid, IID_IDirectInput7W))
    {
      IDirectInput7W* pDInput = new(std::nothrow) IDirectInput7W;
      if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
      *ppvOut = static_cast<IDirectInput7W*>(pDInput);

      HRESULT hr = pDInput->Initialize(NULL, dwVersion);
      if (FAILED(hr)) { return hr; }

      return DI_OK;
    }

    return DIERR_INVALIDPARAM;
  }
#ifdef DINPUT_EXPORTS
}
#endif