```bat
@echo off
cd /d "%~dp0"

rem Visual Studio base paths
set VS_Base_Path=C:\Program Files (x86)
set VS2026_Base_Path=C:\Program Files

rem Check Visual Studio versions (newest first)
if exist "%VS2026_Base_Path%\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat" goto vs2026
if exist "%VS2026_Base_Path%\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars32.bat" goto vs2026_bt

if exist "%VS_Base_Path%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" goto vs2022
if exist "%VS_Base_Path%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat" goto vs2022_bt
if exist "%VS_Base_Path%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" goto vs2019
if exist "%VS_Base_Path%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat" goto vs2017
if exist "%VS_Base_Path%\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars32.bat" goto vs14
if exist "%VS_Base_Path%\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars32.bat" goto vs2019_bt
if exist "%VS_Base_Path%\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars32.bat" goto vs2017_bt

:vs2026
call "%VS2026_Base_Path%\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:vs2026_bt
call "%VS2026_Base_Path%\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:vs2022
call "%VS_Base_Path%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:vs2019
call "%VS_Base_Path%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:vs2017
call "%VS_Base_Path%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:vs14
call "%VS_Base_Path%\Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars32.bat"
goto batend

:vs2022_bt
call "%VS_Base_Path%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:vs2019_bt
call "%VS_Base_Path%\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:vs2017_bt
call "%VS_Base_Path%\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
goto batend

:batend
