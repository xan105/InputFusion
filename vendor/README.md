- [Microsoft detours](https://github.com/microsoft/Detours)
- [SDL3](https://github.com/libsdl-org/SDL)
- [SimpleINI](https://github.com/brofield/simpleini)
- [LuaJIT](https://luajit.org/)

SDL3
====

## Compile SDL3 as a static lib

1. Open `SDL3/VisualC/SDL.sln` in Visual Studio.
2. Select `SDL3` and right-click `Properties`
3. Change the following:

  + Configuration: `All/All`
    - `General > Configuration type`: Change to `Static library (.lib)`
    - `C/C++ > Preprocessor > Preprocessor Definitions`: Remove `DLL_EXPORT`
    
  + Configuration: `Debug/All`
    - `C/C++ > Code Generation > Runtime Library`: Change to `/MDd`
    
  + Configuration: `Release/All`
    - `C/C++ > Code Generation > Runtime Library`: Change to `/MD`
    
4. Build `SDL3` for all desired targets

## Visual Studio

VC++ Directories
  - Include: `..\vendor\SDL3\include`
  - Lib dir: `..\vendor\SDL3\VisualC\$(Platform)\$(Configuration)`
  
Linker
  - SDL3.lib
  - winmm.lib
  - setupapi.lib
  - version.lib
  - Imm32.lib

LuaJIT
======

## Compile LuaJIT as a static lib

I've made some scripts to automate building LuaJIT

- `luajit/pull.cmd` to git clone LuaJIT in `luajit/luajit`
- `luajit/build.cmd` to build LuaJIT as static lib => `luajit/build/$(Platform)/$(Configuration)`

ref: http://luajit.org/install.html

> LuaJIT uses rolling releases. There are no release tarballs available for download. 
> LuaJIT is only made available as source code from a git repository. 
> To access this repository, you need to install the git command for your operating system.

## Visual Studio

VC++ Directories
  - Include: `..\vendor\luajit\luajit\src`
  - Lib dir: `..\vendor\luajit\build\$(Platform)\$(Configuration)`
  
Linker
  - lua51.lib