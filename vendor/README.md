
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

I've made some helper scripts to automate building LuaJIT

- `LuaJIT/pull.cmd` to git clone/pull LuaJIT in `LuaJIT/luajit`.
- `LuaJIT/build.cmd` to open a "Visual Studio Command Prompt" and start LuaJIT's build script.
  > The build output is in `LuaJIT/build/$(Platform)/$(Configuration)`

ref: http://luajit.org/install.html

> LuaJIT uses rolling releases. There are no release tarballs available for download. 
> LuaJIT is only made available as source code from a git repository. 
> To access this repository, you need to install the git command for your operating system.

## Static Library with MSVC /MD and /MDd

Build LuaJIT as a static library (lua51.lib) that can be linked into a DLL compiled with:
- **Release:** /MD
- **Debug**:   /MDd

This avoids mixing incompatible C runtime (CRT) configurations, which can cause linker warnings such as 
`LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs; use /NODEFAULTLIB:library`

Check out or use the diff patch file `LuaJIT/msvcbuild-md-runtime.patch` to update LuaJIT build script `LuaJIT/luajit/src/msvcbuild.bat`.

NB: Using the helper script `LuaJIT/pull.cmd` does it for you.

## Visual Studio

VC++ Directories
  - Include: `..\vendor\LuaJIT\luajit\src`
  - Lib dir: `..\vendor\LuaJIT\build\$(Platform)\$(Configuration)`
  
Linker
  - lua51.lib
  
SimpleINI
=========

## Visual Studio

`C++ > Preprocessor definitions`: Add `NOMINMAX` to resolve the conflicting macro.

Microsoft detours
=================

```cpp
#include "path/to/vendor/Detour/detours.h"
#if defined(_X86_)
    #pragma comment(lib, "path/to/vendor/Detour/detours.x86.lib")
#elif defined(_AMD64_)
    #pragma comment(lib, "path/to/vendor/Detour/detours.x64.lib")
#endif
```