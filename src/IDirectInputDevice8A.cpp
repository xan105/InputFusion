/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#include "pch.h"
#include "dinput8.h"
#include <unordered_map>
#include <algorithm>

const std::unordered_map<SDL_GamepadButton, DWORD> BUTTONS = {
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
};

const std::vector<SDL_GamepadAxis> AXIS = {
	SDL_GAMEPAD_AXIS_LEFTX,
	SDL_GAMEPAD_AXIS_LEFTY,
	SDL_GAMEPAD_AXIS_RIGHTX,
	SDL_GAMEPAD_AXIS_RIGHTY,
	SDL_GAMEPAD_AXIS_LEFT_TRIGGER,
	SDL_GAMEPAD_AXIS_RIGHT_TRIGGER
};

IDirectInputDevice8A::IDirectInputDevice8A() : m_refCount(1) {
	std::cout << "IDirectInputDevice8A" << std::endl;
	this->playerIndex = -1;
}

STDMETHODIMP IDirectInputDevice8A::QueryInterface(REFIID riid, void** ppvObject) {

	std::cout << "IDirectInputDevice8A::QueryInterface()" << std::endl;

	if (ppvObject == nullptr)
		return E_POINTER;

	if (riid == IID_IUnknown || riid == IID_IDirectInputDevice8A) {
		*ppvObject = static_cast<IDirectInputDevice8A*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObject = nullptr;
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) IDirectInputDevice8A::AddRef() {

	std::cout << "IDirectInputDevice8A::AddRef()" << std::endl;

	return InterlockedIncrement(reinterpret_cast<long*>(&m_refCount));
}

STDMETHODIMP_(ULONG) IDirectInputDevice8A::Release() {

	std::cout << "IDirectInputDevice8A::Release()" << std::endl;

	ULONG refCount = InterlockedDecrement(reinterpret_cast<long*>(&m_refCount));
	if (refCount == 0) {
		delete this;
	}
	return refCount;
}

STDMETHODIMP IDirectInputDevice8A::GetCapabilities(LPDIDEVCAPS lpDIDevCaps) {
	std::cout << "IDirectInputDevice8A::GetCapabilities()" << std::endl;

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

STDMETHODIMP IDirectInputDevice8A::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::EnumObjects()" << std::endl;

	DIDEVICEOBJECTINSTANCEA lpddoi = { 0 };
	lpddoi.dwSize = sizeof(DIDEVICEOBJECTINSTANCEA);

	bool DIENUM = lpCallback(&lpddoi, pvRef);
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph) {
	std::cout << "IDirectInputDevice8A::GetProperty()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph) {
	std::cout << "IDirectInputDevice8A::SetProperty()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::Acquire() {
	std::cout << "IDirectInputDevice8A::Acquire()" << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::Unacquire() {
	std::cout << "IDirectInputDevice8A::Unacquire()" << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetDeviceState(DWORD cbData, LPVOID lpvData) {
	std::cout << "IDirectInputDevice8A::GetDeviceState()" << std::endl;

	//IDirectInputDevice8::SetDataFormat
	//c_dfDIJoystick -> DIJOYSTATE
	//c_dfDIJoystick2 -> DIJOYSTATE2


	if (lpvData == nullptr) return DIERR_INVALIDPARAM;
	if (cbData != sizeof(DIJOYSTATE) && cbData != sizeof(DIJOYSTATE2)) return DIERR_INVALIDPARAM;
	if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;

	SDL_InitFlags Flags = SDL_WasInit(SDL_INIT_GAMEPAD);
	if (!(Flags & SDL_INIT_GAMEPAD)) {
		return E_PENDING;
	}

	SDL_Gamepad* gamepad = SDL_GetGamepadFromPlayerIndex(this->playerIndex);
	if (gamepad == nullptr) {
		return DIERR_NOTACQUIRED;
	}

	ZeroMemory(lpvData, cbData);

	for (const auto& [sdl_button, index] : BUTTONS) {
		if (SDL_GetGamepadButton(gamepad, sdl_button)) {
			if (cbData == sizeof(DIJOYSTATE)) {
				((DIJOYSTATE*)lpvData)->rgbButtons[index] = 0x80;
			}
			else if (cbData == sizeof(DIJOYSTATE2)) {
				((DIJOYSTATE2*)lpvData)->rgbButtons[index] = 0x80;
			}
		}
	}

	DWORD pov = 0xFFFF; //Centered (no direction pressed)
	bool up = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
	bool down = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
	bool left = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
	bool right = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);

	if (up && down || left && right) {
		pov = 0xFFFF; // Opposing directions are cancelled out
	}
	else if (up && left) {
		pov = 0x0007;
	}
	else if (up && right) {
		pov = 0x0001;
	}
	else if (down && left) {
		pov = 0x0005;
	}
	else if (down && right) {
		pov = 0x0003;
	}
	else if (up) {
		pov = 0x0000;
	}
	else if (down) {
		pov = 0x0004;
	}
	else if (left) {
		pov = 0x0006;
	}
	else if (right) {
		pov = 0x0002;
	}

	if (cbData == sizeof(DIJOYSTATE)) {
		((DIJOYSTATE*)lpvData)->rgdwPOV[0] = pov;
	}
	else if (cbData == sizeof(DIJOYSTATE2)) {
		((DIJOYSTATE2*)lpvData)->rgdwPOV[0] = pov;
	}

	for (const auto& sdl_axis : AXIS) {
		int value = SDL_GetGamepadAxis(gamepad, sdl_axis);
		if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTX) {
			if (cbData == sizeof(DIJOYSTATE)) {
				((DIJOYSTATE*)lpvData)->lX = value;
			}
			else if (cbData == sizeof(DIJOYSTATE2)) {
				((DIJOYSTATE2*)lpvData)->lX = value;
			}
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFTY) {
			if (cbData == sizeof(DIJOYSTATE)) {
				((DIJOYSTATE*)lpvData)->lY = value;
			}
			else if (cbData == sizeof(DIJOYSTATE2)) {
				((DIJOYSTATE2*)lpvData)->lY = value;
			}
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTX) {
			if (cbData == sizeof(DIJOYSTATE)) {
				((DIJOYSTATE*)lpvData)->lRx = value;
			}
			else if (cbData == sizeof(DIJOYSTATE2)) {
				((DIJOYSTATE2*)lpvData)->lRx = value;
			}
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHTY) {
			if (cbData == sizeof(DIJOYSTATE)) {
				((DIJOYSTATE*)lpvData)->lRy = value;
			}
			else if (cbData == sizeof(DIJOYSTATE2)) {
				((DIJOYSTATE2*)lpvData)->lRy = value;
			}
		}
		// Combine left trigger and right trigger as one axis for DInput Xbox controllers
		else if (sdl_axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER) {
			if (cbData == sizeof(DIJOYSTATE)) {
				((DIJOYSTATE*)lpvData)->lZ = std::clamp(-value, -32768, 0);
			}
			else if (cbData == sizeof(DIJOYSTATE2)) {
				((DIJOYSTATE2*)lpvData)->lZ = std::clamp(-value, -32768, 0);
			}
		}
		else if (sdl_axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) {
			if (cbData == sizeof(DIJOYSTATE)) {
				((DIJOYSTATE*)lpvData)->lZ = std::clamp(value, 0, 32767);
			}
			else if (cbData == sizeof(DIJOYSTATE2)) {
				((DIJOYSTATE2*)lpvData)->lZ = std::clamp(value, 0, 32767);
			}
		}
	}

	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::GetDeviceData()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::SetDataFormat(LPCDIDATAFORMAT lpdf) {
	std::cout << "IDirectInputDevice8A::SetDataFormat()" << std::endl;

	//if (IsEqualGUID(*lpdf->rgodf->pguid, c_dfDIJoystick)

	if (lpdf == nullptr) return DIERR_INVALIDPARAM;
	if (this->playerIndex < 0) return DIERR_NOTINITIALIZED;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::SetEventNotification(HANDLE hEvent) {
	std::cout << "IDirectInputDevice8A::SetEventNotification()" << std::endl;

	//SDL_UpdateGamepads();
	//SetEvent(&hEvent);
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::SetCooperativeLevel(HWND hwnd, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::SetCooperativeLevel()" << std::endl;

	if (hwnd == nullptr) return DIERR_INVALIDPARAM;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow) {
	std::cout << "IDirectInputDevice8A::GetObjectInfo()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi) {
	std::cout << "IDirectInputDevice8A::GetDeviceInfo()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::RunControlPanel()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid) {
	std::cout << "IDirectInputDevice8A::Initialize()" << std::endl;

	if (this->playerIndex >= 0) return S_FALSE; //device already initialized

	if (rguid.Data1 != MAKELONG(XBOX360_VID, XBOX360_PID) ||
		std::string(reinterpret_cast<const char*>(rguid.Data4)+2, 6) != "PLAYER") return DIERR_DEVICENOTREG;

	this->playerIndex = rguid.Data4[1];
	if (this->playerIndex < 0) return DIERR_DEVICENOTREG;

	std::cout << "IDirectInputDevice8A::Initialize() > Set player to " << this->playerIndex << std::endl;
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter) {
	std::cout << "IDirectInputDevice8A::CreateEffect()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType) {
	std::cout << "IDirectInputDevice8A::EnumEffects()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid) {
	std::cout << "IDirectInputDevice8A::GetEffectInfo()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::GetForceFeedbackState(LPDWORD pdwOut) {
	std::cout << "IDirectInputDevice8A::GetForceFeedbackState()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::SendForceFeedbackCommand(DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::SendForceFeedbackCommand()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl) {
	std::cout << "IDirectInputDevice8A::EnumCreatedEffectObjects()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::Escape(LPDIEFFESCAPE pesc) {
	std::cout << "IDirectInputDevice8A::Escape()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::Poll() {
	std::cout << "IDirectInputDevice8A::Poll()" << std::endl;
	SDL_UpdateGamepads();
	return DI_OK;
}

STDMETHODIMP IDirectInputDevice8A::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl) {
	std::cout << "IDirectInputDevice8A::SendDeviceData()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::EnumEffectsInFile()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPCDIFILEEFFECT rgDiFileEft, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::WriteEffectToFile()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::BuildActionMap(LPDIACTIONFORMATA lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::BuildActionMap()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::SetActionMap(LPCDIACTIONFORMATA lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags) {
	std::cout << "IDirectInputDevice8A::SetActionMap()" << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP IDirectInputDevice8A::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA lpdiDevImageInfoHeader) {
	std::cout << "IDirectInputDevice8A::GetImageInfo()" << std::endl;
	return E_NOTIMPL;
}