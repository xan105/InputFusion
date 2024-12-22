/*
Copyright (c) Anthony Beaumont
This source code is licensed under the MIT License
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "dinput8.h"
#include <unordered_map>

/*const std::unordered_map<SDL_GamepadButton, DWORD> BUTTONS = {
	{SDL_GAMEPAD_BUTTON_SOUTH, 0},
	{SDL_GAMEPAD_BUTTON_EAST, 1},
	{SDL_GAMEPAD_BUTTON_WEST, 2},
	{SDL_GAMEPAD_BUTTON_NORTH, 3},
	{SDL_GAMEPAD_BUTTON_BACK, 6},
	{SDL_GAMEPAD_BUTTON_START, 7},
	{SDL_GAMEPAD_BUTTON_LEFT_STICK, 8},
	{SDL_GAMEPAD_BUTTON_RIGHT_STICK, 9},
	{SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, 4},
	{SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, 5},
	{SDL_GAMEPAD_BUTTON_TOUCHPAD, 6}
};*/


IDirectInputDevice8W::IDirectInputDevice8W() : m_refCount(1) {
	std::cout << "IDirectInputDevice8W" << std::endl;
	this->playerIndex = -1;
}

STDMETHODIMP IDirectInputDevice8W::QueryInterface(REFIID riid, void** ppvObject) {

	std::cout << "IDirectInputDevice8W::QueryInterface()" << std::endl;

	if (ppvObject == nullptr)
		return E_POINTER;

	if (riid == IID_IUnknown || riid == IID_IDirectInputDevice8W) {
		*ppvObject = static_cast<IDirectInputDevice8W*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObject = nullptr;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDevice8W::AddRef() {

	std::cout << "IDirectInputDevice8W::AddRef()" << std::endl;

	return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDevice8W::Release() {

	std::cout << "IDirectInputDevice8W::Release()" << std::endl;

	ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
	if (refCount == 0) {
		delete this;
	}
	return refCount;
}


STDMETHODIMP IDirectInputDevice8W::GetCapabilities(LPDIDEVCAPS lpDIDevCaps) {
	std::cout << "IDirectInputDevice8W::GetCapabilities()" << std::endl;

	if (lpDIDevCaps == nullptr) return E_POINTER;
	if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;

	if (lpDIDevCaps->dwSize == sizeof(DIDEVCAPS)) {
		ZeroMemory(lpDIDevCaps, sizeof(DIDEVCAPS));
		lpDIDevCaps->dwSize = sizeof(DIDEVCAPS);
	}
	else if (lpDIDevCaps->dwSize == sizeof(DIDEVCAPS_DX3)) {
		ZeroMemory(lpDIDevCaps, sizeof(DIDEVCAPS_DX3));
		lpDIDevCaps->dwSize = sizeof(DIDEVCAPS_DX3);
	}
	else {
		return DIERR_INVALIDPARAM;
	}

	lpDIDevCaps->dwFlags = DIDC_ATTACHED | DIDC_EMULATED;
	lpDIDevCaps->dwDevType = (MAKEWORD(DI8DEVTYPE_GAMEPAD, DI8DEVTYPEGAMEPAD_STANDARD) | DIDEVTYPE_HID);
	lpDIDevCaps->dwAxes = 5;
	lpDIDevCaps->dwButtons = 10;
	lpDIDevCaps->dwPOVs = 1;

	if (lpDIDevCaps->dwSize == sizeof(DIDEVCAPS)) {
		lpDIDevCaps->dwFFSamplePeriod = 0;
		lpDIDevCaps->dwFFMinTimeResolution = 0;
		lpDIDevCaps->dwFirmwareRevision = 0;
		lpDIDevCaps->dwHardwareRevision = 0;
		lpDIDevCaps->dwFFDriverVersion = 0;
	}

	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::EnumObjects()" << std::endl;

	DIDEVICEOBJECTINSTANCEW lpddoi = { 0 };
	lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEW);

	bool DIENUM = lpCallback(&lpddoi, pvRef);
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph) {
	std::cout << "IDirectInputDevice8W::GetProperty()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph) {
	std::cout << "IDirectInputDevice8W::SetProperty()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Acquire() {
	std::cout << "IDirectInputDevice8W::Acquire()" << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::Unacquire() {
	std::cout << "IDirectInputDevice8W::Unacquire()" << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceState(DWORD cbData, LPVOID lpvData) {
	std::cout << "IDirectInputDevice8W::GetDeviceState()" << std::endl;
	return E_NOTIMPL;

	//IDirectInputDevice8::SetDataFormat
	//c_dfDIJoystick -> DIJOYSTATE
	//c_dfDIJoystick2 -> DIJOYSTATE2



	/*for (const auto& sdl_axis : AXIS) {

		int value = SDL_GetGamepadAxis(gamepad, sdl_axis);

		if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
			state.lX = value;
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
			state.lY = value
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
			state.lRx = value;
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
			state.lRy = value;
		}
		//Xbox controller over DInput: 
		//The left and right trigger buttons will act as a single button, not independently.
		//Triggers are combined, one trigger to a positive direction and the other to a negative direction.
		else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
			state.lZ = std::clamp(-value, -32768, 0);
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
			state.lZ = std::clamp(value, 0, 32767);
		}
	}*/




}

STDMETHODIMP IDirectInputDevice8W::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::GetDeviceData()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SetDataFormat(LPCDIDATAFORMAT lpdf) {
	std::cout << "IDirectInputDevice8W::SetDataFormat()" << std::endl;
	//if (IsEqualGUID(*lpdf->rgodf->pguid, c_dfDIJoystick)
	
	if (lpdf == nullptr) return DIERR_INVALIDPARAM;
	if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SetEventNotification(HANDLE hEvent) {
	std::cout << "IDirectInputDevice8W::SetEventNotification()" << std::endl;
	//SDL_UpdateGamepads();
	//SetEvent(&hEvent);
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SetCooperativeLevel(HWND hwnd, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::SetCooperativeLevel()" << std::endl;
	
	if (hwnd == nullptr) return DIERR_INVALIDPARAM;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow) {
	std::cout << "IDirectInputDevice8W::GetObjectInfo()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi) {
	std::cout << "IDirectInputDevice8W::GetDeviceInfo()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::RunControlPanel()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid) {
	std::cout << "IDirectInputDevice8W::Initialize()" << std::endl;

	if (this->playerIndex >= 0) return S_FALSE; //device already initialized

	if (rguid.Data1 != MAKELONG(XBOX360_VID, XBOX360_PID) ||
		std::string(reinterpret_cast<const char*>(rguid.Data4) + 2, 6) != "PLAYER") return DIERR_DEVICENOTREG;

	this->playerIndex = rguid.Data4[1];
	if (this->playerIndex < 0) return DIERR_DEVICENOTREG;

	std::cout << "IDirectInputDevice8W::Initialize() > Set player to " << this->playerIndex << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter) {
	std::cout << "IDirectInputDevice8W::CreateEffect()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType) {
	std::cout << "IDirectInputDevice8W::EnumEffects()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid) {
	std::cout << "IDirectInputDevice8W::GetEffectInfo()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetForceFeedbackState(LPDWORD pdwOut) {
	std::cout << "IDirectInputDevice8W::GetForceFeedbackState()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SendForceFeedbackCommand(DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::SendForceFeedbackCommand()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl) {
	std::cout << "IDirectInputDevice8W::EnumCreatedEffectObjects()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Escape(LPDIEFFESCAPE pesc) {
	std::cout << "IDirectInputDevice8W::Escape()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::Poll() {
	std::cout << "IDirectInputDevice8W::Poll()" << std::endl;
	//SDL_UpdateGamepads();
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8W::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl) {
	std::cout << "IDirectInputDevice8W::SendDeviceData()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::EnumEffectsInFile()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPCDIFILEEFFECT rgDiFileEft, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::WriteEffectToFile()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::BuildActionMap(LPDIACTIONFORMATW lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::BuildActionMap()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::SetActionMap(LPCDIACTIONFORMATW lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8W::SetActionMap()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8W::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW lpdiDevImageInfoHeader) {
	std::cout << "IDirectInputDevice8W::GetImageInfo()" << std::endl;
	return E_NOTIMPL;
}