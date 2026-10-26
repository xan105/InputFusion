@echo off
setlocal

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"

if not exist "%VSWHERE%" (
    echo ERROR: vswhere.exe was not found.
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_PATH=%%i"

if not defined VS_PATH (
    echo ERROR: Visual Studio C++ tools were not found.
    exit /b 1
)

if not exist "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" (
    echo ERROR: vcvarsall.bat was not found at "%VS_PATH%".
    exit /b 1
)

endlocal & set "VS_PATH=%VS_PATH%"
call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x86
if errorlevel 1 exit /b 1

where cl.exe
if errorlevel 1 exit /b 1
