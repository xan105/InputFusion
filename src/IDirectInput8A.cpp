/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "dinput8.h"

IDirectInput8A::IDirectInput8A() : m_refCount(1) {
	std::cout << "IDirectInput8A" << std::endl;
}

STDMETHODIMP IDirectInput8A::QueryInterface(REFIID riid, void** ppvObject) {

	std::cout << "IDirectInput8A::QueryInterface()" << std::endl;

	if (ppvObject == nullptr)
		return E_POINTER;

	if (riid == IID_IUnknown || riid == IID_IDirectInput8A) {
		*ppvObject = static_cast<IDirectInput8A*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObject = nullptr;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInput8A::AddRef() {

	std::cout << "IDirectInput8A::AddRef()" << std::endl;

	return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInput8A::Release() {

	std::cout << "IDirectInput8A::Release()" << std::endl;

	ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
	if (refCount == 0) {
		delete this;
	}
	return refCount;
}

STDMETHODIMP IDirectInput8A::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A* lplpDirectInputDevice, LPUNKNOWN pUnkOuter) {

	std::cout << "IDirectInput8A::CreateDevice()" << std::endl;

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

	std::cout << "IDirectInput8A::EnumDevices()" << std::endl;

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

	std::cout << "IDirectInput8A::GetDeviceStatus()" << std::endl;
	return DI_OK;
}


STDMETHODIMP IDirectInput8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {

	std::cout << "IDirectInput8A::RunControlPanel()" << std::endl;

	if (dwFlags != 0) return DIERR_INVALIDPARAM;
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::Initialize(HINSTANCE hinst, DWORD dwVersion) {

	std::cout << "IDirectInput8A::Initialize()" << std::endl;

	if (dwVersion < DIRECTINPUT_VERSION) return DIERR_OLDDIRECTINPUTVERSION;
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::FindDevice(REFGUID rguidClass, LPCTSTR ptszName, LPGUID pguidInstance) {

	std::cout << "IDirectInput8A::FindDevice()" << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::EnumDevicesBySemantics(LPCTSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags) {

	std::cout << "IDirectInput8A::EnumDevicesBySemantics()" << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInput8A::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData) {
	std::cout << "IDirectInput8A::ConfigureDevices()" << std::endl;
	return E_NOTIMPL;
}
