/*
Copyright (c) Anthony Beaumont
This source code is licensed under the MIT License
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "dinput8.h"

//extern "C" {

	HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter){

		std::cout << "DirectInput8Create()" << std::endl;
		
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

			std::cout << "DirectInput8Create() > IID_IDirectInput8W" << std::endl;

			IDirectInput8W* pDInput = new(std::nothrow) IDirectInput8W;
			if (pDInput == nullptr) return DIERR_OUTOFMEMORY;
			*ppvOut = static_cast<IDirectInput8W*>(pDInput);

			HRESULT hr = pDInput->Initialize(NULL, DIRECTINPUT_VERSION);
			if (FAILED(hr)) { return hr; }

			return DI_OK;
		}
		
		return DIERR_INVALIDPARAM;
	}

//}