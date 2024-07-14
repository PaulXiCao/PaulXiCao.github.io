---
title: 'C++ dual build: Including headers or importing modules'
categories:
- C++
tags:
- modules
- CMake
- C++20
---

This blog post will discuss enabling a dual build configuration for C++20 modules: Either use old-school header includes (x)or import modules instead.

C++20 modules is a feature rich topic and there are many online resources on how to write them from scratch, e.g.
- [Rainer Grimm's modernescpp.com](https://www.modernescpp.com/index.php/cpp20-modules/)
- [MicroSoft's documentation](https://learn.microsoft.com/en-us/cpp/cpp/modules-cpp?view=msvc-170)
- [clang's documentation](https://clang.llvm.org/docs/StandardCPlusPlusModules.html)
- [cppreference on modules](https://en.cppreference.com/w/cpp/language/modules)
- [my own post on a minimal introduction to modules (wip)]({% link _posts/old/2024-01-10-turbo-introduction-of-c20-modules.html %})

There is a [reddit discussion](https://www.reddit.com/r/cpp/comments/1e37dd6/dual_build_mode_header_includes_or_module_imports/) about this dual build idea.

# Motivation

Using modules provides certain benefits to the old header include method (see the above linked pages for in-detail discussions of benefits).
For a working legacy code base one of the most promising features is a potential reduction in compilation time.

Switching to modules-only implementation might not be possible for a corporation as many still need to support older environments.
A typical scenario might be that the internal developers can make use of up-to-date build tools but the CI still needs to run an older configuration as some customers still rely on it.

To be precise, the developers might use the current versions of CMake, ninja, clang whereas an older version of gcc (no module support) still needs to compile the code base.

# The {fmt} way

While searching the internet for ideas I came across this post by Daniela E on [reddit](https://www.reddit.com/r/cpp/comments/1busseu/comment/kxvfayf).
She explains that she took the header-only `fmt` library and created a wrapper module which includes the headers.
Thus, one can consume the library either via header includes or module import.
She employed some non-trivial CMake functions and preprocessor macros (fmt can be found [here](https://github.com/fmtlib/fmt)).
Unfortunately, I did not find any further information on that which is why I am writing this post.

As a side note: The fmt library is header-only.
Writing a wrapper module is far easier than for a library with translation units (cpp files).
More on that later.

# Implementation overview

Enabling the dual build mode is not straight-forward which is why I have created a [sample repository](https://github.com/PaulXiCao/cpp-module-dual-build) for testing and explanation purposes.

In folder 01 I start off with a very simple version and with each new folder I add some additional functionality.
For people familiar with CMake, preprocessor, and modules it might be enough to look into the last folder (currently 06 as of this writing).
Take note of the README files in each folder (describing background information and build instructions).

I will try to explain the important parts but reading the code will probably still be necessary (I might just be too deep into the topic at the moment to know what needs explaining ;) ).

## The big picture

We start off with a given library (hpp/cpp files) and plan to enable importing that library via a module but with minimal changes to the original files.
The module will include the library's hpp _and_ cpp files and export only the API entities.
For that we will need to introduce some macros that split the files into separate sections (e.g. std includes, library includes, external includes, non-include content).

On the CMake side of the business we introduce an option (e.g. `-D ENABLE_MODULES=ON/OFF`) and employ `target_sources(.. FILE_SET CXX_MODULES ..)` to work with modules.

# The deep dive: Folder 06

Lets take a look the code in the [folder 06](https://github.com/PaulXiCao/cpp-module-dual-build/tree/master/06_multipleLibraries).

It contains two separate libraries (e.g. in folders `Lib{1,2}/`) and one `main.cpp` executable.
The `main.cpp` either imports modules or does header includes depending on the CMake option `ENABLE_MODULES`.

The libraries themselves contain the original hpp/cpp files (e.g. `Lib1/Lib1_header*`) as well as 2 files needed to build the wrapper module for each library (e.g. `Lib1/Lib1_module*`).

The complex parts are which separation to introduce in the original files (using macros) and where to include them into the named module.

Lets look at `Lib1/Lib1_header1.hpp` for example.

## Normal header include build

At first lets make sure that the normal header include still works fine.
Thus we define the macros s.t. `#pragma once` is used, all headers get included (e.g. `<string>`), and `MODULE_EXPORT` expands to an empty value.

## The `MODULE_EXPORT` macro

The API entities (e.g. `struct my_struct`) needs to be `export`ed within the wrapper module.
We achieve this by prepending the struct with the `MODULE_EXPORT` macro which expands to `export` in the module build type.

## Conditional `#pragma once`

The `#pragma once` include guard cannot be used in the module build because we need to include the file twice in the primary module interface unit (e.g. `Lib1/Lib1_module.cpp`).

## Separating includes

The includes are grouped into 3 different sections, i.e. `USE_INCLUDES_FROM_STD`, `USE_INCLUDES_FROM_THIS_LIBRARY`, `USE_INCLUDES_FROM_OTHER_LIBRARIES`.
This is necessary because the includes need to be pasted into different sections of the module.

(We could make our lives easier by either `import std;` in the module or hard-coding them in the module.
But I wanted to see how precise I could make this.)

### `USE_INCLUDES_FROM_STD`

The standard headers belong to the global module purview and need to be imported in the global module fragment (e.g. between the `module;` and `export module MyModule;` lines).
Thus, this can be achieved by extracting only the standard headers by only defining this macro.

### `USE_INCLUDES_FROM_THIS_LIBRARY`

This separates includes from this library to others (either standard or external).
As we plan to write a wrapper of this library which includes all headers via hard-coded includes we do not need to include them again.
Thus, this macro is currently never set in module build.

### `USE_INCLUDES_FROM_OTHER_LIBRARIES`

(WIP)

External libraries can either be header included or imported.
This is currently not well implemented.
(Includes should probably appear in global module fragment.)

There is also additional complexity to find out if the external module should be imported in the module impl file which depends if it was already imported in the primary module file.

### `USE_NON_INCLUDES_CONTENT`

This section is for all the "real" entities of the library, e.g. declarations and definitions.

## Primary module interface

The primary module interface (e.g. `Lib1/Lib1_module.cpp`) is the "main" module file.
It is responsible for exporting the API entities.
This should resemble a combination of the library's headers.

The standard headers get included in the global module fragment (e.g. between the `module;` and `export module lib1;` lines).

In the module fragment part (e.g. after `export module lib1;`) we want the declarations and definitions of all headers as well as specifying which entities get `export`ed.

## Module implementation unit

The module implementation unit (e.g. `Lib/Lib1_module_impl.cpp`) is the file used for the implementations.
This should resemble a combination of the library's cpp files.

The include logic is the same as for the primary module interface, but now applied to the library's cpp files instead of the headers.

## CMakeLists

### The Library file

The library CMakeLists (e.g. `Lib{1,2}/CMakeLists.txt`) are very similar.

In the module build all original library files (e.g. `Lib1/Lib1_header*`) are treated as header files. 
This is done as they are all `include`d in some module file.
Furthermore, the `CXX_MODULES` file-set type is used for the primary module interface (the one doing the `export`).

In the header build a typical setup is used.

### The top-level file

The top-level `./CMakeLists.txt` sets up a standard project.

In the header build the necessary macros are set s.t. the headers do not reflect any module specifics (e.g. empty `MODULE_EXPORT=`).
Furthermore, module scanning is turned off (for performance reasons as we do not use them).

# Conclusion / Outlook

I found this a worthwhile deep-dive into the C++ module world.
I learned quite a lot :)

Ideas for further investigation:
- How to treat external libraries automatically (instead of checking a library specific macro multiple times)?
- Are there any issues with shared/static libraries?
- How should one treat the case when only part of the library has been modularized?
- Typical unity-build issues (e.g. static functions with the same name/arguments in different cpp files)?
- Discuss expected compile-time benefits/regressions
  - benefit: `import`ing should be faster than `include`ing headers multiple times
  - regression: modularizing a library under development might be result in long compilation times of the module implementation unit `*module_impl.cpp`)?
- Investigate wrongful `include`ing of headers when the module was build instead (should not be possible for correctly set include directories)?