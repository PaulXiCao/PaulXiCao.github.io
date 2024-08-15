---
title: 'My first patch to `libc++`'
categories:
- C++
tags:
- llvm
- libc++
---

I listened to an older cppcast episode and it mentioned that the C++17 mathematical special functions are not yet included in llvm's standard library libc++.
_How hard could it be?_

Follow me down the rabbit hole how I implemented `std::hermite` for libc++.

Some important pages for reference:
- Documentation, building instructions, testing instructions, etc. can be found at https://libcxx.llvm.org/.
- Development and issue tracking happens at https://github.com/llvm/llvm-project.
- The merge request: https://github.com/llvm/llvm-project/pull/89982.

Getting the development environment up and running:
1. Fork the project at github, e.g. https://github.com/PaulXiCao/llvm-project/
2. Checkout
```bash
git clone git@github.com:PaulXiCao/llvm-project.git
cd llvm-project
```
3. Create `runtimes/CMakeUserPresets.json`:
```
{
    "version": 3,
    "configurePresets": [
        {
            "name": "gcc",
            "description": "gcc, mold, ccache",
            "hidden": false,
            "generator": "Ninja",
            "cacheVariables": {
                "CMAKE_CXX_COMPILER_LAUNCHER": "ccache",
                "CMAKE_EXE_LINKER_FLAGS": "-fuse-ld=mold",
                "CMAKE_SHARED_LINKER_FLAGS": "-fuse-ld=mold",
                "LLVM_ENABLE_RUNTIMES": "libcxx;libcxxabi;libunwind"
            }
        },
        {
            "name": "systemClang",
            "description": "system-wide clang, mold, ccache",
            "inherits": "gcc",
            "cacheVariables": {
                "CMAKE_C_COMPILER": "clang",
                "CMAKE_CXX_COMPILER": "clang++"
            }
        }
    ]
}
```
4. Initiate CMake: `$ cmake -B build -S runtimes --preset systemClang`
5. Build: `$ ninja -C build cxx-test-depends`
6. Run tests: `$ libcxx/utils/libcxx-lit build/ -sv libcxx/test/std/numerics/c.math/cmath.pass.cpp`

Figuring out the build process (especially the test program) took quite some time.

Then came the review process.
It span across 4 months and roughly 150 comments and 80 commits.
Summary of feedback
- Use unconditional includes. E.g. not
```c++
#if _LIBCPP_STD_VER >= 17
#  include <type_traits>
#endif 
```
- Use granularized headers.
- Hide functions in headers from ABI. E.g. `_LIBCPP_HIDE_FROM_ABI _Real __hermite(..) {..}`
- Qualify all function calls. E.g. `std::__hermite();` instead of `__hermite();`
- Test files should specify their valid standards. E.g. `// UNSUPPORTED: c++03, c++11, c++14`
- (Add a status page for mathematical special function implementation.)
- Refactor tests to fit surrounding code.
- Add functions to header synopsis for documentation purposes.
- And some more discussions (e.g. NaN handling, accuracy counted in ULPs)..

This was quite some work but I learned a lot! 
The reviewers were more than welcoming.
I enjoyed it quite a lot :)