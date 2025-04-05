/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "dinput8.h"

IDirectInput8A::IDirectInput8A() : m_refCount(1) {
	SDL_Log("IDirectInput8A");
}

STDMETHODIMP IDirectInput8A::QueryInterface(REFIID riid, void** ppvObject) {
	SDL_Log("IDirectInput8A::QueryInterface()");

	if (ppvObject == nullptr)
		return E_POINTER;

  if(IsEqualGUID(riid, IID_IUnknown) ||
     IsEqualGUID(riid, IID_IDirectInput8A))
  {
    *ppvObject = static_cast<IDirectInput8A*>(this);
    AddRef();
    return S_OK;
  }

  if (IsEqualGUID(riid, IID_IDirectInputJoyConfig8)) {
    SDL_Log("IID_IDirectInput8A::QueryInterface() > IDirectInputJoyConfig");

    IDirectInputJoyConfig8* pDInputJoyConfig = new(std::nothrow) IDirectInputJoyConfig8;
    if (pDInputJoyConfig == nullptr) return E_NOINTERFACE;
    *ppvObject = static_cast<IDirectInputJoyConfig8*>(pDInputJoyConfig);

    return S_OK;
  }
	
	*ppvObject = nullptr;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInput8A::AddRef() {
	SDL_Log("IDirectInput8A::AddRef()");

	return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInput8A::Release() {
	SDL_Log("IDirectInput8A::Release()");

	ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
	if (refCount == 0) {
		delete this;
	}
	return refCount;
}

STDMETHODIMP IDirectInput8A::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A* lplpDirectInputDevice, LPUNKNOWN pUnkOuter) {
	SDL_Log("IDirectInput8A::CreateDevice()");

	if (rguid == GUID_SysKeyboard || rguid == GUID_SysMouse)
		return DIERR_NOINTERFACE;

	IDirectInputDevice8A* pDInputDevice = new(std::nothrow) IDirectInputDevice8A;
	if (pDInputDevice == nullptr) return DIERR_OUTOFMEMORY;
	*lplpDirectInputDevice = static_cast<IDirectInputDevice8A*>(pDInputDevice);

	HRESULT hr = pDInputDevice->Initialize(NULL, DIRECTINPUT_VERSION, rguid);
	if (FAILED(hr)) { return hr; }

	return DI_OK;
}

STDMETHODIMP IDirectInput8A::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags) {
	SDL_Log("IDirectInput8A::EnumDevices()");

	if (dwDevType != DI8DEVCLASS_ALL && dwDevType != DI8DEVCLASS_GAMECTRL)
		return E_NOTIMPL;

	SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
	if (!(Flags & SDL_INIT_GAMEPAD)) {
		return DIERR_NOTINITIALIZED;
	}

	int count;
	SDL_JoystickID* gamepads = SDL_GetGamepads(&count);
	if (gamepads == nullptr) {
		return DIERR_NOTINITIALIZED;
	}

	for (int i = 0; i < count; i++) {

		SDL_JoystickID id = gamepads[i];
		SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
		if (gamepad == nullptr) continue;

		DIDEVICEINSTANCEA lpddi = { 0 };
		lpddi.dwSize = sizeof(DIDEVICEINSTANCEA);

		//add a flag to create a ds4 instead ?
		
		//To ID the gamepad later on when creating a device, I'm encoding the corresponding playerIndex into the fake guid instance
		lpddi.guidInstance = {
			MAKELONG(XBOX360_VID, XBOX360_PID),														// Data1 (VID + PID)
			0x0000,																					// Data2 (reserved)
			0x0000,																					// Data3 (reserved)
			{ 0x00, (BYTE)SDL_GetGamepadPlayerIndex(gamepad), 0x50, 0x4C, 0x41, 0x59, 0x45, 0x52 }	// Data4 (ASCII "PLAYER")
		};
		lpddi.guidProduct = {
			MAKELONG(XBOX360_VID, XBOX360_PID),														// Data1 VID + PID
			0x0000,																					// Data2 (reserved)
			0x0000,																					// Data3 (reserved)
			{ 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 }										// Data4 (ASCII "PIDVID")
		};

		lpddi.dwDevType = (MAKEWORD(DI8DEVTYPE_GAMEPAD, DI8DEVTYPEGAMEPAD_STANDARD) | DIDEVTYPE_HID); //0x00010215
		strcpy_s(lpddi.tszInstanceName, _countof(lpddi.tszInstanceName), XBOX360_INSTANCE_NAMEA);
		strcpy_s(lpddi.tszProductName, _countof(lpddi.tszProductName), XBOX360_PRODUCT_NAMEA);
		lpddi.guidFFDriver = GUID_NULL; 
		lpddi.wUsagePage = 0x01;
		lpddi.wUsage = 0x05;

		bool DIENUM = lpCallback(&lpddi, pvRef);
		if (DIENUM == DIENUM_STOP) {
			break;
		}

	}
	SDL_free(gamepads);

	return DI_OK;
}

STDMETHODIMP IDirectInput8A::GetDeviceStatus(REFGUID rguidInstance) {
	SDL_Log("IDirectInput8A::GetDeviceStatus()");
	return DI_OK;
}


STDMETHODIMP IDirectInput8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
	SDL_Log("IDirectInput8A::RunControlPanel()");

	if (dwFlags != 0) return DIERR_INVALIDPARAM;
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::Initialize(HINSTANCE hinst, DWORD dwVersion) {
	SDL_Log("IDirectInput8A::Initialize()");

	//if (dwVersion < DIRECTINPUT_VERSION) return DIERR_OLDDIRECTINPUTVERSION;
	SDL_Log("DInput version: %u", dwVersion);
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance) {
	SDL_Log("IDirectInput8A::FindDevice()");
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags) {
	SDL_Log("IDirectInput8A::EnumDevicesBySemantics()");
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData) {
	SDL_Log("IDirectInput8A::ConfigureDevices()");
	//IDirectInput8::ConfigureDevices is deprecated. This method always fails on Windows Vista and later versions of Windows.
	return E_NOTIMPL;
}
