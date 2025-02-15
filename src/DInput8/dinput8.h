/*
Copyright (c) Anthony Beaumont
This source code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE Version 3
found in the LICENSE file in the root directory of this source tree.
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <SDL3/SDL.h>
#include <unknwn.h> 
#include <string>
#include <atomic>

#define DIRECTINPUT_VERSION 0x0800

DEFINE_GUID(IID_IDirectInput8A, 0xBF798030, 0x483A, 0x4DA2, 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00);
DEFINE_GUID(IID_IDirectInput8W, 0xBF798031, 0x483A, 0x4DA2, 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00);
DEFINE_GUID(IID_IDirectInputDevice8A, 0x54D41080, 0xDC15, 0x4833, 0xA4, 0x1B, 0x74, 0x8F, 0x73, 0xA3, 0x81, 0x79);
DEFINE_GUID(IID_IDirectInputDevice8W, 0x54D41081, 0xDC15, 0x4833, 0xA4, 0x1B, 0x74, 0x8F, 0x73, 0xA3, 0x81, 0x79);

DEFINE_GUID(GUID_SysKeyboard, 0x6b9b1bdf, 0x846b, 0x11d0, 0x97, 0x0b, 0x00, 0xa0, 0xc9, 0x1f, 0x5d, 0x6c);
DEFINE_GUID(GUID_SysMouse, 0x6b9b1bf0, 0x846b, 0x11d0, 0x97, 0x0b, 0x00, 0xa0, 0xc9, 0x1f, 0x5d, 0x6c);


#define DI_OK				S_OK
#define DIERR_INVALIDPARAM	E_INVALIDARG
#define DIERR_OUTOFMEMORY	E_OUTOFMEMORY
#define DIERR_DEVICENOTREG  REGDB_E_CLASSNOTREG
#define DIERR_NOINTERFACE   E_NOINTERFACE

#define DIERR_OLDDIRECTINPUTVERSION		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_OLD_WIN_VERSION)
#define DIERR_NOTINITIALIZED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_NOT_READY)
#define DIERR_INPUTLOST                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_READ_FAULT)
#define DIERR_ACQUIRED                  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_BUSY)
#define DIERR_NOTACQUIRED               MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ERROR_INVALID_ACCESS)


#define DIENUM_STOP			0
#define DIENUM_CONTINUE		1

#define DI8DEVCLASS_ALL			0
#define DI8DEVCLASS_DEVICE		1
#define DI8DEVCLASS_POINTER		2
#define DI8DEVCLASS_KEYBOARD	3
#define DI8DEVCLASS_GAMECTRL	4

#define DI8DEVTYPE_DEVICE           0x11
#define DI8DEVTYPE_MOUSE            0x12
#define DI8DEVTYPE_KEYBOARD         0x13
#define DI8DEVTYPE_JOYSTICK         0x14
#define DI8DEVTYPE_GAMEPAD          0x15
#define DI8DEVTYPE_DRIVING          0x16
#define DI8DEVTYPE_FLIGHT           0x17
#define DI8DEVTYPE_1STPERSON        0x18
#define DI8DEVTYPE_DEVICECTRL       0x19
#define DI8DEVTYPE_SCREENPOINTER    0x1A
#define DI8DEVTYPE_REMOTE           0x1B
#define DI8DEVTYPE_SUPPLEMENTAL     0x1C
#define DIDEVTYPE_HID               0x00010000

#define DI8DEVTYPEGAMEPAD_STANDARD  2

#define DIDC_ATTACHED           0x00000001
#define DIDC_EMULATED           0x00000004

#define XBOX360_VID	0x045E
#define XBOX360_PID	0x028E
#define XBOX360_INSTANCE_NAMEW (L"XBOX 360 For Windows (Controller)")
#define XBOX360_PRODUCT_NAMEW (L"XBOX 360 For Windows (Controller)")
#define XBOX360_INSTANCE_NAMEA ("XBOX 360 For Windows (Controller)")
#define XBOX360_PRODUCT_NAMEA ("XBOX 360 For Windows (Controller)")




typedef struct DIJOYSTATE {
    LONG lX;
    LONG lY;
    LONG lZ;
    LONG lRx;
    LONG lRy;
    LONG lRz;
    LONG rglSlider[2];
    DWORD rgdwPOV[4];
    BYTE rgbButtons[32];
} DIJOYSTATE, * LPDIJOYSTATE;

typedef struct DIJOYSTATE2 {
    LONG lX;
    LONG lY;
    LONG lZ;
    LONG lRx;
    LONG lRy;
    LONG lRz;
    LONG rglSlider[2];
    DWORD rgdwPOV[4];
    BYTE rgbButtons[128];
    LONG lVX;
    LONG lVY;
    LONG lVZ;
    LONG lVRx;
    LONG lVRy;
    LONG lVRz;
    LONG rglVSlider[2];
    LONG lAX;
    LONG lAY;
    LONG lAZ;
    LONG lARx;
    LONG lARy;
    LONG lARz;
    LONG rglASlider[2];
    LONG lFX;
    LONG lFY;
    LONG lFZ;
    LONG lFRx;
    LONG lFRy;
    LONG lFRz;
    LONG rglFSlider[2];
} DIJOYSTATE2, * LPDIJOYSTATE2;








typedef DWORD D3DCOLOR;

typedef struct _DICOLORSET {
    DWORD dwSize;
    D3DCOLOR cTextFore;
    D3DCOLOR cTextHighlight;
    D3DCOLOR cCalloutLine;
    D3DCOLOR cCalloutHighlight;
    D3DCOLOR cBorder;
    D3DCOLOR cControlFill;
    D3DCOLOR cHighlightFill;
    D3DCOLOR cAreaFill;
} DICOLORSET, * LPDICOLORSET;
typedef const DICOLORSET* LPCDICOLORSET;

typedef struct _DIACTIONA {
    UINT_PTR    uAppData;
    DWORD       dwSemantic;
    OPTIONAL    DWORD       dwFlags;
    OPTIONAL    union {
        LPCSTR      lptszActionName;
        UINT        uResIdString;
    };
    OPTIONAL    GUID        guidInstance;
    OPTIONAL    DWORD       dwObjID;
    OPTIONAL    DWORD       dwHow;
} DIACTIONA, * LPDIACTIONA;
typedef struct _DIACTIONW {
    UINT_PTR    uAppData;
    DWORD       dwSemantic;
    OPTIONAL    DWORD       dwFlags;
    OPTIONAL    union {
        LPCWSTR     lptszActionName;
        UINT        uResIdString;
    };
    OPTIONAL    GUID        guidInstance;
    OPTIONAL    DWORD       dwObjID;
    OPTIONAL    DWORD       dwHow;
} DIACTIONW, * LPDIACTIONW;

typedef struct _DIACTIONFORMATA {
    DWORD       dwSize;
    DWORD       dwActionSize;
    DWORD       dwDataSize;
    DWORD       dwNumActions;
    LPDIACTIONA rgoAction;
    GUID        guidActionMap;
    DWORD       dwGenre;
    DWORD       dwBufferSize;
    OPTIONAL    LONG        lAxisMin;
    OPTIONAL    LONG        lAxisMax;
    OPTIONAL    HINSTANCE   hInstString;
    FILETIME    ftTimeStamp;
    DWORD       dwCRC;
    CHAR        tszActionMap[MAX_PATH];
} DIACTIONFORMATA, * LPDIACTIONFORMATA;
typedef const DIACTIONFORMATA* LPCDIACTIONFORMATA;

typedef struct _DIACTIONFORMATW {
    DWORD       dwSize;
    DWORD       dwActionSize;
    DWORD       dwDataSize;
    DWORD       dwNumActions;
    LPDIACTIONW rgoAction;
    GUID        guidActionMap;
    DWORD       dwGenre;
    DWORD       dwBufferSize;
    OPTIONAL    LONG        lAxisMin;
    OPTIONAL    LONG        lAxisMax;
    OPTIONAL    HINSTANCE   hInstString;
    FILETIME    ftTimeStamp;
    DWORD       dwCRC;
    WCHAR       tszActionMap[MAX_PATH];
} DIACTIONFORMATW, * LPDIACTIONFORMATW;
typedef const DIACTIONFORMATW* LPCDIACTIONFORMATW;

typedef struct _DIDEVICEINSTANCEA {
    DWORD   dwSize;
    GUID    guidInstance;
    GUID    guidProduct;
    DWORD   dwDevType;
    CHAR    tszInstanceName[MAX_PATH];
    CHAR    tszProductName[MAX_PATH];
    GUID    guidFFDriver;
    WORD    wUsagePage;
    WORD    wUsage;
} DIDEVICEINSTANCEA, * LPDIDEVICEINSTANCEA;

typedef struct _DIDEVICEINSTANCEW {
    DWORD   dwSize;
    GUID    guidInstance;
    GUID    guidProduct;
    DWORD   dwDevType;
    WCHAR   tszInstanceName[MAX_PATH];
    WCHAR   tszProductName[MAX_PATH];
    GUID    guidFFDriver;
    WORD    wUsagePage;
    WORD    wUsage;
} DIDEVICEINSTANCEW, * LPDIDEVICEINSTANCEW;

typedef struct _DICONFIGUREDEVICESPARAMSA {
    DWORD             dwSize;
    DWORD             dwcUsers;
    LPSTR             lptszUserNames;
    DWORD             dwcFormats;
    LPDIACTIONFORMATA lprgFormats;
    HWND              hwnd;
    DICOLORSET        dics;
    IUnknown FAR* lpUnkDDSTarget;
} DICONFIGUREDEVICESPARAMSA, * LPDICONFIGUREDEVICESPARAMSA;
typedef struct _DICONFIGUREDEVICESPARAMSW {
    DWORD             dwSize;
    DWORD             dwcUsers;
    LPWSTR            lptszUserNames;
    DWORD             dwcFormats;
    LPDIACTIONFORMATW lprgFormats;
    HWND              hwnd;
    DICOLORSET        dics;
    IUnknown FAR* lpUnkDDSTarget;
} DICONFIGUREDEVICESPARAMSW, * LPDICONFIGUREDEVICESPARAMSW;

typedef struct DIDEVCAPS {
    DWORD   dwSize;
    DWORD   dwFlags;
    DWORD   dwDevType;
    DWORD   dwAxes;
    DWORD   dwButtons;
    DWORD   dwPOVs;
    DWORD   dwFFSamplePeriod;
    DWORD   dwFFMinTimeResolution;
    DWORD   dwFirmwareRevision;
    DWORD   dwHardwareRevision;
    DWORD   dwFFDriverVersion;
} DIDEVCAPS, * LPDIDEVCAPS;

typedef struct DIDEVCAPS_DX3 {
    DWORD   dwSize;
    DWORD   dwFlags;
    DWORD   dwDevType;
    DWORD   dwAxes;
    DWORD   dwButtons;
    DWORD   dwPOVs;
} DIDEVCAPS_DX3, * LPDIDEVCAPS_DX3;

typedef struct DIDEVICEOBJECTINSTANCEA {
    DWORD   dwSize;
    GUID    guidType;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
    CHAR    tszName[MAX_PATH];
    DWORD   dwFFMaxForce;
    DWORD   dwFFForceResolution;
    WORD    wCollectionNumber;
    WORD    wDesignatorIndex;
    WORD    wUsagePage;
    WORD    wUsage;
    DWORD   dwDimension;
    WORD    wExponent;
    WORD    wReportId;
} DIDEVICEOBJECTINSTANCEA, * LPDIDEVICEOBJECTINSTANCEA;

typedef struct DIDEVICEOBJECTINSTANCEW {
    DWORD   dwSize;
    GUID    guidType;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
    WCHAR   tszName[MAX_PATH];
    DWORD   dwFFMaxForce;
    DWORD   dwFFForceResolution;
    WORD    wCollectionNumber;
    WORD    wDesignatorIndex;
    WORD    wUsagePage;
    WORD    wUsage;
    DWORD   dwDimension;
    WORD    wExponent;
    WORD    wReportId;
} DIDEVICEOBJECTINSTANCEW, * LPDIDEVICEOBJECTINSTANCEW;

typedef struct DIPROPHEADER {
    DWORD   dwSize;
    DWORD   dwHeaderSize;
    DWORD   dwObj;
    DWORD   dwHow;
} DIPROPHEADER, *LPDIPROPHEADER;
typedef const DIPROPHEADER* LPCDIPROPHEADER;

typedef struct DIDEVICEOBJECTDATA {
    DWORD       dwOfs;
    DWORD       dwData;
    DWORD       dwTimeStamp;
    DWORD       dwSequence;
    UINT_PTR    uAppData;
} DIDEVICEOBJECTDATA, * LPDIDEVICEOBJECTDATA;
typedef const DIDEVICEOBJECTDATA* LPCDIDEVICEOBJECTDATA;

typedef struct _DIOBJECTDATAFORMAT {
    const GUID* pguid;
    DWORD   dwOfs;
    DWORD   dwType;
    DWORD   dwFlags;
} DIOBJECTDATAFORMAT, * LPDIOBJECTDATAFORMAT;

typedef struct _DIDATAFORMAT {
    DWORD   dwSize;
    DWORD   dwObjSize;
    DWORD   dwFlags;
    DWORD   dwDataSize;
    DWORD   dwNumObjs;
    LPDIOBJECTDATAFORMAT rgodf;
} DIDATAFORMAT, * LPDIDATAFORMAT;
typedef const DIDATAFORMAT* LPCDIDATAFORMAT;

typedef struct DIENVELOPE {
    DWORD dwSize;
    DWORD dwAttackLevel;
    DWORD dwAttackTime;
    DWORD dwFadeLevel;
    DWORD dwFadeTime;
} DIENVELOPE, * LPDIENVELOPE;
typedef const DIENVELOPE* LPCDIENVELOPE;

typedef struct DIEFFECT {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwDuration;
    DWORD dwSamplePeriod;
    DWORD dwGain;
    DWORD dwTriggerButton;
    DWORD dwTriggerRepeatInterval;
    DWORD cAxes;
    LPDWORD rgdwAxes;
    LPLONG rglDirection;
    LPDIENVELOPE lpEnvelope;
    DWORD cbTypeSpecificParams;
    LPVOID lpvTypeSpecificParams;
    DWORD  dwStartDelay;
} DIEFFECT, * LPDIEFFECT;
typedef const DIEFFECT* LPCDIEFFECT;

typedef struct DIEFFESCAPE {
    DWORD   dwSize;
    DWORD   dwCommand;
    LPVOID  lpvInBuffer;
    DWORD   cbInBuffer;
    LPVOID  lpvOutBuffer;
    DWORD   cbOutBuffer;
} DIEFFESCAPE, * LPDIEFFESCAPE;

typedef struct DIEFFECTINFOA {
    DWORD   dwSize;
    GUID    guid;
    DWORD   dwEffType;
    DWORD   dwStaticParams;
    DWORD   dwDynamicParams;
    CHAR    tszName[MAX_PATH];
} DIEFFECTINFOA, * LPDIEFFECTINFOA;
typedef struct DIEFFECTINFOW {
    DWORD   dwSize;
    GUID    guid;
    DWORD   dwEffType;
    DWORD   dwStaticParams;
    DWORD   dwDynamicParams;
    WCHAR   tszName[MAX_PATH];
} DIEFFECTINFOW, * LPDIEFFECTINFOW;

typedef struct DIFILEEFFECT {
    DWORD       dwSize;
    GUID        GuidEffect;
    LPCDIEFFECT lpDiEffect;
    CHAR        szFriendlyName[MAX_PATH];
}DIFILEEFFECT, * LPDIFILEEFFECT;
typedef const DIFILEEFFECT* LPCDIFILEEFFECT;

typedef struct _DIDEVICEIMAGEINFOA {
    CHAR        tszImagePath[MAX_PATH];
    DWORD       dwFlags;
    DWORD       dwViewID;
    RECT        rcOverlay;
    DWORD       dwObjID;
    DWORD       dwcValidPts;
    POINT       rgptCalloutLine[5];
    RECT        rcCalloutRect;
    DWORD       dwTextAlign;
} DIDEVICEIMAGEINFOA, * LPDIDEVICEIMAGEINFOA;

typedef struct _DIDEVICEIMAGEINFOW {
    WCHAR       tszImagePath[MAX_PATH];
    DWORD       dwFlags;
    DWORD       dwViewID;
    RECT        rcOverlay;
    DWORD       dwObjID;
    DWORD       dwcValidPts;
    POINT       rgptCalloutLine[5];
    RECT        rcCalloutRect;
    DWORD       dwTextAlign;
} DIDEVICEIMAGEINFOW, * LPDIDEVICEIMAGEINFOW;

typedef struct _DIDEVICEIMAGEINFOHEADERA {
    DWORD       dwSize;
    DWORD       dwSizeImageInfo;
    DWORD       dwcViews;
    DWORD       dwcButtons;
    DWORD       dwcAxes;
    DWORD       dwcPOVs;
    DWORD       dwBufferSize;
    DWORD       dwBufferUsed;
    LPDIDEVICEIMAGEINFOA lprgImageInfoArray;
} DIDEVICEIMAGEINFOHEADERA, * LPDIDEVICEIMAGEINFOHEADERA;

typedef struct _DIDEVICEIMAGEINFOHEADERW {
    DWORD       dwSize;
    DWORD       dwSizeImageInfo;
    DWORD       dwcViews;
    DWORD       dwcButtons;
    DWORD       dwcAxes;
    DWORD       dwcPOVs;
    DWORD       dwBufferSize;
    DWORD       dwBufferUsed;
    LPDIDEVICEIMAGEINFOW lprgImageInfoArray;
} DIDEVICEIMAGEINFOHEADERW, * LPDIDEVICEIMAGEINFOHEADERW;

class IDirectInputEffect : public IUnknown
{
public:
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwFlags, REFGUID rguid);
    virtual STDMETHODIMP GetEffectGuid(LPGUID rguid);
    virtual STDMETHODIMP GetParameters(LPDIEFFECT pDieffect, DWORD dwFlags);
    virtual STDMETHODIMP SetParameters(LPCDIEFFECT pDieffect, DWORD dwFlags);
    virtual STDMETHODIMP Start(DWORD dwIterations, DWORD dwFlags);
    virtual STDMETHODIMP Stop();
    virtual STDMETHODIMP GetEffectStatus(LPDWORD pdwStatus);
    virtual STDMETHODIMP Download();
    virtual STDMETHODIMP Unload();
    virtual STDMETHODIMP Escape(LPDIEFFESCAPE pDieffEscape);
};
typedef class IDirectInputEffect* LPDIRECTINPUTEFFECT;

typedef BOOL(FAR PASCAL* LPDICONFIGUREDEVICESCALLBACK)(IUnknown FAR*, LPVOID);
typedef BOOL(FAR PASCAL* LPDIENUMDEVICESCALLBACKA)(DIDEVICEINSTANCEA*, LPVOID);
typedef BOOL(FAR PASCAL* LPDIENUMDEVICESCALLBACKW)(DIDEVICEINSTANCEW*, LPVOID);




typedef const DIDEVICEOBJECTINSTANCEA* LPCDIDEVICEOBJECTINSTANCEA;
typedef const DIDEVICEOBJECTINSTANCEW* LPCDIDEVICEOBJECTINSTANCEW;

typedef BOOL(FAR PASCAL* LPDIENUMDEVICEOBJECTSCALLBACKA)(LPCDIDEVICEOBJECTINSTANCEA, LPVOID);
typedef BOOL(FAR PASCAL* LPDIENUMDEVICEOBJECTSCALLBACKW)(LPCDIDEVICEOBJECTINSTANCEW, LPVOID);

typedef const DIEFFECTINFOA* LPCDIEFFECTINFOA;
typedef const DIEFFECTINFOW* LPCDIEFFECTINFOW;

typedef BOOL(FAR PASCAL* LPDIENUMEFFECTSCALLBACKA)(LPCDIEFFECTINFOA, LPVOID);
typedef BOOL(FAR PASCAL* LPDIENUMEFFECTSCALLBACKW)(LPCDIEFFECTINFOW, LPVOID);

typedef BOOL(FAR PASCAL* LPDIENUMCREATEDEFFECTOBJECTSCALLBACK)(LPDIRECTINPUTEFFECT, LPVOID);

typedef BOOL(FAR PASCAL* LPDIENUMEFFECTSINFILECALLBACK)(LPCDIFILEEFFECT, LPVOID);

class IDirectInputDevice8A : public IUnknown {
private:
    std::atomic<long> m_refCount;
    int playerIndex;

public:

    IDirectInputDevice8A();
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
    virtual STDMETHODIMP CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter);
    virtual STDMETHODIMP EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType);
    virtual STDMETHODIMP GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid);
    virtual STDMETHODIMP GetForceFeedbackState(LPDWORD pdwOut);
    virtual STDMETHODIMP SendForceFeedbackCommand(DWORD dwFlags);
    virtual STDMETHODIMP EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    virtual STDMETHODIMP Escape(LPDIEFFESCAPE pesc);
    virtual STDMETHODIMP Poll();
    virtual STDMETHODIMP SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
    virtual STDMETHODIMP EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPCDIFILEEFFECT rgDiFileEft, DWORD dwFlags);
    virtual STDMETHODIMP BuildActionMap(LPDIACTIONFORMATA lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags);
    virtual STDMETHODIMP SetActionMap(LPCDIACTIONFORMATA lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags);
    virtual STDMETHODIMP GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA lpdiDevImageInfoHeader);
};
typedef class IDirectInputDevice8A* LPDIRECTINPUTDEVICE8A;

class IDirectInputDevice8W : public IUnknown {
private:
    std::atomic<long> m_refCount;
    int playerIndex;

public:

    IDirectInputDevice8W();
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
    virtual STDMETHODIMP CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter);
    virtual STDMETHODIMP EnumEffects(LPDIENUMEFFECTSCALLBACKW lpCallback, LPVOID pvRef, DWORD dwEffType);
    virtual STDMETHODIMP GetEffectInfo(LPDIEFFECTINFOW pdei, REFGUID rguid);
    virtual STDMETHODIMP GetForceFeedbackState(LPDWORD pdwOut);
    virtual STDMETHODIMP SendForceFeedbackCommand(DWORD dwFlags);
    virtual STDMETHODIMP EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    virtual STDMETHODIMP Escape(LPDIEFFESCAPE pesc);
    virtual STDMETHODIMP Poll();
    virtual STDMETHODIMP SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
    virtual STDMETHODIMP EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPCDIFILEEFFECT rgDiFileEft, DWORD dwFlags);
    virtual STDMETHODIMP BuildActionMap(LPDIACTIONFORMATW lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags);
    virtual STDMETHODIMP SetActionMap(LPCDIACTIONFORMATW lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags);
    virtual STDMETHODIMP GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW lpdiDevImageInfoHeader);
};
typedef class IDirectInputDevice8W* LPDIRECTINPUTDEVICE8W;

typedef BOOL(FAR PASCAL* LPDIENUMDEVICESBYSEMANTICSCBA)(DIDEVICEINSTANCEA*, LPDIRECTINPUTDEVICE8A, DWORD, DWORD, LPVOID);
typedef BOOL(FAR PASCAL* LPDIENUMDEVICESBYSEMANTICSCBW)(DIDEVICEINSTANCEW*, LPDIRECTINPUTDEVICE8W, DWORD, DWORD, LPVOID);

class IDirectInput8A : public IUnknown {
private:
    std::atomic<long> m_refCount;

public:

    IDirectInput8A();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
    virtual STDMETHODIMP FindDevice(REFGUID rguidClass, LPCTSTR ptszName, LPGUID pguidInstance);
    virtual STDMETHODIMP EnumDevicesBySemantics(LPCTSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData);
};

class IDirectInput8W : public IUnknown {
private:
    std::atomic<long> m_refCount;

public:

    IDirectInput8W();
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
    virtual STDMETHODIMP_(ULONG) AddRef() override;
    virtual STDMETHODIMP_(ULONG) Release() override;
    virtual STDMETHODIMP CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
    virtual STDMETHODIMP EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP GetDeviceStatus(REFGUID rguidInstance);
    virtual STDMETHODIMP RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    virtual STDMETHODIMP Initialize(HINSTANCE hinst, DWORD dwVersion);
    virtual STDMETHODIMP FindDevice(REFGUID rguidClass, LPCTSTR ptszName, LPGUID pguidInstance);
    virtual STDMETHODIMP EnumDevicesBySemantics(LPCTSTR ptszUserName, LPDIACTIONFORMATW lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBW lpCallback, LPVOID pvRef, DWORD dwFlags);
    virtual STDMETHODIMP ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData);
};


typedef HRESULT(WINAPI* DirectInput8Create_t)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);

#ifdef DINPUT8_EXPORTS
extern "C" {
#endif
  HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
  HRESULT WINAPI DllCanUnloadNow();
  HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv);
  HRESULT WINAPI DllRegisterServer();
  HRESULT WINAPI DllUnregisterServer();
#ifdef DINPUT8_EXPORTS
}
#endif