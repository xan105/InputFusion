#include "pch.h"
#include "dinput8.h"

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
	return E_NOTIMPL;
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
	return E_NOTIMPL;

	//IDirectInputDevice8::SetDataFormat
	//c_dfDIJoystick -> DIJOYSTATE
	//c_dfDIJoystick2 -> DIJOYSTATE2
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
	//SDL_UpdateGamepads();
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