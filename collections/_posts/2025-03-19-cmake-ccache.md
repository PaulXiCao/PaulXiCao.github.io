---
title: "Using Ccache with CMake"
categories:
- cmake
tags:
- ccache
---

CMake supports Ccache out-of-the-box via the [CMAKE_<LANG>_COMPILER_LAUNCHER](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_LAUNCHER.html) property.

Example usage via commandline
```bash
cmake -B build -D CMAKE_C_COMPILER_LAUNCHER=ccache -D CMAKE_CXX_COMPILER_LAUNCHER=ccache
```
or within CMakePreset
```json
// file: CMakeUserPresets.json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "myPreset",
      "hidden": false,
      "cacheVariables": {
        "CMAKE_C_COMPILER_LAUNCHER": "ccache",
        "CMAKE_CXX_COMPILER_LAUNCHER": "ccache"
      }
    }
  ]
}
```