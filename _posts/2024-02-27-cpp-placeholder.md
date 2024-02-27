---
title: "C++: Placeholder `_`"
categories:
  - C++
tags:
  - c++26
  - placeholder
---

C++ is on its way to get a placeholder variable `_` similar to other languages, e.g., Python, rust.

This is useful in multiple scenarios:
- Variables used just for their side effects do not require a unique name anymore, e.g. locks, and other RAII objects.
```c++
std::lock_guard someUniqueName(mutex); // old: needed to find unique name
std::lock_guard _(mutex); // new: no name needed anymore
```
- When unpacking objects via structured binding some parts might not be of interest.
    This can now be represented directly in code.
```c++
[[maybe_unused]] auto [x, y, zUnused] = f(); // old
auto [x, y, _] = f(); // new
```
- Supposedly useful for pattern matching.

Implementation by gcc14 and clang18 (both unreleased WIP versions).

Resources:
- Short and precise explanation by [Jason Turner's cppweekly on youtube](https://www.youtube.com/watch?v=OZ1gNuF60BU).
- Original paper: [_Placeholder variables with no name_](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2169r4.pdf)
- cppreference has no separate page as of this writing but the feature is listed on its [C++26 overview page](https://en.cppreference.com/w/cpp/26).
