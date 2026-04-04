- [Microsoft detours](https://github.com/microsoft/Detours)
- [SDL3](https://github.com/libsdl-org/SDL)

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