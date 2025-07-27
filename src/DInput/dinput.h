/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#define WIN32_LEAN_AND_MEAN
#include "../DInput8/dinput8.h"

typedef enum DirectInputVersions: DWORD {
    DIRECTINPUT_VERSION_300 = 0x0300,
    DIRECTINPUT_VERSION_500 = 0x0500,
    DIRECTINPUT_VERSION_50A = 0x050A,
    DIRECTINPUT_VERSION_5B2 = 0x05B2,
    DIRECTINPUT_VERSION_602 = 0x0602,
    DIRECTINPUT_VERSION_61A = 0x061A,
    DIRECTINPUT_VERSION_700 = 0x0700,
    DIRECTINPUT_VERSION_800 = 0x0800
} DirectInputVersions;

DEFINE_GUID(IID_IDirectInputA,        0x89521360,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputW,        0x89521361,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInput2A,       0x5944E662,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInput2W,       0x5944E663,0xAA8A,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInput7A,       0x9A4CB684,0x236D,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInput7W,       0x9A4CB685,0x236D,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInputDeviceA,  0x5944E680,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDeviceW,  0x5944E681,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDevice2A, 0x5944E682,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDevice2W, 0x5944E683,0xC92E,0x11CF,0xBF,0xC7,0x44,0x45,0x53,0x54,0x00,0x00);
DEFINE_GUID(IID_IDirectInputDevice7A, 0x57D7C6BC,0x2356,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInputDevice7W, 0x57D7C6BD,0x2356,0x11D3,0x8E,0x9D,0x00,0xC0,0x4F,0x68,0x44,0xAE);
DEFINE_GUID(IID_IDirectInputEffect,   0xE7E1F7C0,0x88D2,0x11D0,0x9A,0xD0,0x00,0xA0,0xC9,0xA0,0x6E,0x35);
DEFINE_GUID(IID_IDirectInputJoyConfig, 0x1DE12AB1, 0xC9F5, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

class IDirectInputDeviceA : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInputDevice8A* proxy;
    
public:

    IDirectInputDeviceA();
    ~IDirectInputDeviceA();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP GetCapabilities(LPDIDEVCAPS lpDIDevCaps);
    virtual STDMETHODIMP EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    virtual STDMETHODIMP SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    virtual STDMETHODIMP Acquire();
    virtual STDMETHODIMP Unacquire();
    virtual STDMETHODIMP GetDeviceState(DWORD cbData, LPVOID lpvData);
    virtual STDMETHODIMP GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    virtual STDMETHODIMP SetDataFormat(LPCDIDATAFORMAT lpdf);
    virtual STDMETHODIMP SetEventNotification(HANDLE hEvent);
    virtual STDMETHODIMP SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
    virtual STDMETHODIMP GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow);
    virtual STDMETHODIMP GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
};
typedef class IDirectInputDeviceA* LPDIRECTINPUTDEVICEA;

class IDirectInputDeviceW : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInputDevice8W* proxy;
    
public:

    IDirectInputDeviceW();
    ~IDirectInputDeviceW();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP GetCapabilities(LPDIDEVCAPS lpDIDevCaps);
    virtual STDMETHODIMP EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    virtual STDMETHODIMP SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    virtual STDMETHODIMP Acquire();
    virtual STDMETHODIMP Unacquire();
    virtual STDMETHODIMP GetDeviceState(DWORD cbData, LPVOID lpvData);
    virtual STDMETHODIMP GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    virtual STDMETHODIMP SetDataFormat(LPCDIDATAFORMAT lpdf);
    virtual STDMETHODIMP SetEventNotification(HANDLE hEvent);
    virtual STDMETHODIMP SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
    virtual STDMETHODIMP GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow);
    virtual STDMETHODIMP GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
};
typedef class IDirectInputDeviceW* LPDIRECTINPUTDEVICEW;

class IDirectInputDevice2A : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInputDevice8A* proxy;
    
public:

    IDirectInputDevice2A();
    ~IDirectInputDevice2A();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP GetCapabilities(LPDIDEVCAPS lpDIDevCaps);
    virtual STDMETHODIMP EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    virtual STDMETHODIMP SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    virtual STDMETHODIMP Acquire();
    virtual STDMETHODIMP Unacquire();
    virtual STDMETHODIMP GetDeviceState(DWORD cbData, LPVOID lpvData);
    virtual STDMETHODIMP GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    virtual STDMETHODIMP SetDataFormat(LPCDIDATAFORMAT lpdf);
    virtual STDMETHODIMP SetEventNotification(HANDLE hEvent);
    virtual STDMETHODIMP SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
    virtual STDMETHODIMP GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow);
    virtual STDMETHODIMP GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
    virtual STDMETHODIMP CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter);
    virtual STDMETHODIMP EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType);
    virtual STDMETHODIMP GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid);
    virtual STDMETHODIMP GetForceFeedbackState(LPDWORD pdwOut);
    virtual STDMETHODIMP SendForceFeedbackCommand(DWORD dwFlags);
    virtual STDMETHODIMP EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    virtual STDMETHODIMP Escape(LPDIEFFESCAPE pesc);
    virtual STDMETHODIMP Poll();
    virtual STDMETHODIMP SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
};
typedef class IDirectInputDevice2A* LPDIRECTINPUTDEVICE2A;

class IDirectInputDevice2W : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInputDevice8W* proxy;
    
public:

    IDirectInputDevice2W();
    ~IDirectInputDevice2W();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP GetCapabilities(LPDIDEVCAPS lpDIDevCaps);
    virtual STDMETHODIMP EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    virtual STDMETHODIMP SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    virtual STDMETHODIMP Acquire();
    virtual STDMETHODIMP Unacquire();
    virtual STDMETHODIMP GetDeviceState(DWORD cbData, LPVOID lpvData);
    virtual STDMETHODIMP GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    virtual STDMETHODIMP SetDataFormat(LPCDIDATAFORMAT lpdf);
    virtual STDMETHODIMP SetEventNotification(HANDLE hEvent);
    virtual STDMETHODIMP SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
    virtual STDMETHODIMP GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow);
    virtual STDMETHODIMP GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
    virtual STDMETHODIMP CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter);
    virtual STDMETHODIMP EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType);
    virtual STDMETHODIMP GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid);
    virtual STDMETHODIMP GetForceFeedbackState(LPDWORD pdwOut);
    virtual STDMETHODIMP SendForceFeedbackCommand(DWORD dwFlags);
    virtual STDMETHODIMP EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    virtual STDMETHODIMP Escape(LPDIEFFESCAPE pesc);
    virtual STDMETHODIMP Poll();
    virtual STDMETHODIMP SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
};
typedef class IDirectInputDevice2W* LPDIRECTINPUTDEVICE2W;

class IDirectInputDevice7A : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInputDevice8A* proxy;
    
public:

    IDirectInputDevice7A();
    ~IDirectInputDevice7A();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP GetCapabilities(LPDIDEVCAPS lpDIDevCaps);
    virtual STDMETHODIMP EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    virtual STDMETHODIMP SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    virtual STDMETHODIMP Acquire();
    virtual STDMETHODIMP Unacquire();
    virtual STDMETHODIMP GetDeviceState(DWORD cbData, LPVOID lpvData);
    virtual STDMETHODIMP GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    virtual STDMETHODIMP SetDataFormat(LPCDIDATAFORMAT lpdf);
    virtual STDMETHODIMP SetEventNotification(HANDLE hEvent);
    virtual STDMETHODIMP SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
    virtual STDMETHODIMP GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow);
    virtual STDMETHODIMP GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
    virtual STDMETHODIMP CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter);
    virtual STDMETHODIMP EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType);
    virtual STDMETHODIMP GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid);
    virtual STDMETHODIMP GetForceFeedbackState(LPDWORD pdwOut);
    virtual STDMETHODIMP SendForceFeedbackCommand(DWORD dwFlags);
    virtual STDMETHODIMP EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    virtual STDMETHODIMP Escape(LPDIEFFESCAPE pesc);
    virtual STDMETHODIMP Poll();
    virtual STDMETHODIMP SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
    virtual STDMETHODIMP EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags);
};
typedef class IDirectInputDevice7A* LPDIRECTINPUTDEVICE7A;

class IDirectInputDevice7W : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInputDevice8W* proxy;
    
public:

    IDirectInputDevice7W();
    ~IDirectInputDevice7W();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP GetCapabilities(LPDIDEVCAPS lpDIDevCaps);
    virtual STDMETHODIMP EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    virtual STDMETHODIMP SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    virtual STDMETHODIMP Acquire();
    virtual STDMETHODIMP Unacquire();
    virtual STDMETHODIMP GetDeviceState(DWORD cbData, LPVOID lpvData);
    virtual STDMETHODIMP GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    virtual STDMETHODIMP SetDataFormat(LPCDIDATAFORMAT lpdf);
    virtual STDMETHODIMP SetEventNotification(HANDLE hEvent);
    virtual STDMETHODIMP SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
    virtual STDMETHODIMP GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW pdidoi, DWORD dwObj, DWORD dwHow);
    virtual STDMETHODIMP GetDeviceInfo(LPDIDEVICEINSTANCEW pdidi);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
    virtual STDMETHODIMP CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT * ppdeff, LPUNKNOWN punkOuter);
    virtual STDMETHODIMP EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType);
    virtual STDMETHODIMP GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid);
    virtual STDMETHODIMP GetForceFeedbackState(LPDWORD pdwOut);
    virtual STDMETHODIMP SendForceFeedbackCommand(DWORD dwFlags);
    virtual STDMETHODIMP EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    virtual STDMETHODIMP Escape(LPDIEFFESCAPE pesc);
    virtual STDMETHODIMP Poll();
    virtual STDMETHODIMP SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
    virtual STDMETHODIMP EnumEffectsInFile(LPCWSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP WriteEffectToFile(LPCWSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags);
};
typedef class IDirectInputDevice7W* LPDIRECTINPUTDEVICE7W;

class IDirectInputA : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInput8A* proxy;
    
public:

    IDirectInputA();
    ~IDirectInputA();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICEA *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
};

class IDirectInputW : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInput8W* proxy;
    
public:

    IDirectInputW();
    ~IDirectInputW();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICEW *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
};

class IDirectInput2A : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInput8A* proxy;
    
public:

    IDirectInput2A();
    ~IDirectInput2A();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE2A *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
    virtual STDMETHODIMP FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance);
};

class IDirectInput2W : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInput8W* proxy;
    
public:

    IDirectInput2W();
    ~IDirectInput2W();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE2W *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
    virtual STDMETHODIMP FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance);
};

class IDirectInput7A : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInput8A* proxy;
    
public:

    IDirectInput7A();
    ~IDirectInput7A();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE7A *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
    virtual STDMETHODIMP FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance);
    virtual STDMETHODIMP CreateDeviceEx(REFGUID rguid, REFIID riid, LPVOID* ppvObj, LPUNKNOWN pUnkOuter);
};

class IDirectInput7W : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInput8W* proxy;
    
public:

    IDirectInput7W();
    ~IDirectInput7W();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE7W *lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
    virtual STDMETHODIMP FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance);
    virtual STDMETHODIMP CreateDeviceEx(REFGUID rguid, REFIID riid, LPVOID* ppvObj, LPUNKNOWN pUnkOuter);
};

class IDirectInputJoyConfig : public IUnknown {
private:
    std::atomic<long> m_refCount;
    IDirectInputJoyConfig8* proxy;
    
public:

    IDirectInputJoyConfig();
    ~IDirectInputJoyConfig();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP Acquire();
    virtual STDMETHODIMP Unacquire();
    virtual STDMETHODIMP SetCooperativeLevel(HWND unnamedParam1, DWORD unnamedParam2);
    virtual STDMETHODIMP SendNotify();
    virtual STDMETHODIMP EnumTypes(LPDIJOYTYPECALLBACK unnamedParam1, LPVOID unnamedParam2);
    virtual STDMETHODIMP GetTypeInfo(LPCWSTR unnamedParam1, LPDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3);
    virtual STDMETHODIMP SetTypeInfo(LPCWSTR unnamedParam1, LPCDIJOYTYPEINFO unnamedParam2, DWORD unnamedParam3, LPWSTR unnamedParam4);
    virtual STDMETHODIMP DeleteType(LPCWSTR unnamedParam1);
    virtual STDMETHODIMP GetConfig(UINT unnamedParam1, LPDIJOYCONFIG unnamedParam2, DWORD unnamedParam3);
    virtual STDMETHODIMP SetConfig(UINT unnamedParam1, LPCDIJOYCONFIG unnamedParam2, DWORD unnamedParam3);
    virtual STDMETHODIMP DeleteConfig(UINT unnamedParam1);
    virtual STDMETHODIMP GetUserValues(LPDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2);
    virtual STDMETHODIMP SetUserValues(LPCDIJOYUSERVALUES unnamedParam1, DWORD unnamedParam2);
    virtual STDMETHODIMP AddNewHardware(HWND unnamedParam1, REFGUID unnamedParam2);
    virtual STDMETHODIMP OpenTypeKey(LPCWSTR unnamedParam1, DWORD unnamedParam2, PHKEY unnamedParam3);
    virtual STDMETHODIMP OpenConfigKey(DWORD unnamedParam1, PHKEY unnamedParam2);
};


#ifdef DINPUT_EXPORTS
extern "C" {
#endif
    HRESULT WINAPI DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppvOut, LPUNKNOWN punkOuter);
    HRESULT WINAPI DirectInputCreateW(HINSTANCE hinst, DWORD dwVersion, LPVOID* ppvOut, LPUNKNOWN punkOuter);
    HRESULT WINAPI DirectInputCreateEx(HINSTANCE hinst, DWORD dwVersion, REFIID riid, LPVOID* ppvOut, LPUNKNOWN punkOuter);
#ifdef DINPUT_EXPORTS
}
#endif