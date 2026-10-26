@echo off

REM x86 static build
cd "%~dp0"
call build_env_x86.bat
cd "%~dp0luajit\src"
REM debug build
call msvcbuild.bat debug static
if not exist ..\..\build\Win32\Debug (
    mkdir ..\..\build\Win32\Debug
)
move /Y luajit.exe ..\..\build\Win32\Debug
move /Y luajit.exp ..\..\build\Win32\Debug
move /Y luajit.lib ..\..\build\Win32\Debug
move /Y luajit.pdb ..\..\build\Win32\Debug
move /Y lua51.lib ..\..\build\Win32\Debug
move /Y vc140.pdb ..\..\build\Win32\Debug
move /Y buildvm.exp ..\..\build\Win32\Debug
move /Y buildvm.lib ..\..\build\Win32\Debug
move /Y luajit_relver.txt ..\..\build\Win32\Debug
move /Y minilua.exp ..\..\build\Win32\Debug
move /Y minilua.lib ..\..\build\Win32\Debug
REM release build
call msvcbuild.bat static
if not exist ..\..\build\Win32\Release (
    mkdir ..\..\build\Win32\Release
)
move /Y luajit.exe ..\..\build\Win32\Release
move /Y luajit.exp ..\..\build\Win32\Release
move /Y luajit.lib ..\..\build\Win32\Release
move /Y luajit.pdb ..\..\build\Win32\Release
move /Y lua51.lib ..\..\build\Win32\Release
move /Y vc140.pdb ..\..\build\Win32\Release
move /Y buildvm.exp ..\..\build\Win32\Release
move /Y buildvm.lib ..\..\build\Win32\Release
move /Y luajit_relver.txt ..\..\build\Win32\Release
move /Y minilua.exp ..\..\build\Win32\Release
move /Y minilua.lib ..\..\build\Win32\Release

REM x64 static build
cd "%~dp0"
call build_env_x64.bat
cd "%~dp0luajit\src"
REM debug build
call msvcbuild.bat debug static
if not exist ..\..\build\x64\Debug (
    mkdir ..\..\build\x64\Debug
)
move /Y luajit.exe ..\..\build\x64\Debug
move /Y luajit.exp ..\..\build\x64\Debug
move /Y luajit.lib ..\..\build\x64\Debug
move /Y luajit.pdb ..\..\build\x64\Debug
move /Y lua51.lib ..\..\build\x64\Debug
move /Y vc140.pdb ..\..\build\x64\Debug
move /Y buildvm.exp ..\..\build\x64\Debug
move /Y buildvm.lib ..\..\build\x64\Debug
move /Y luajit_relver.txt ..\..\build\x64\Debug
move /Y minilua.exp ..\..\build\x64\Debug
move /Y minilua.lib ..\..\build\x64\Debug
REM release build
call msvcbuild.bat static
if not exist ..\..\build\x64\Release (
    mkdir ..\..\build\x64\Release
)
move /Y luajit.exe ..\..\build\x64\Release
move /Y luajit.exp ..\..\build\x64\Release
move /Y luajit.lib ..\..\build\x64\Release
move /Y luajit.pdb ..\..\build\x64\Release
move /Y lua51.lib ..\..\build\x64\Release
move /Y vc140.pdb ..\..\build\x64\Release
move /Y buildvm.exp ..\..\build\x64\Release
move /Y buildvm.lib ..\..\build\x64\Release
move /Y luajit_relver.txt ..\..\build\x64\Release
move /Y minilua.exp ..\..\build\x64\Release
move /Y minilua.lib ..\..\build\x64\Release