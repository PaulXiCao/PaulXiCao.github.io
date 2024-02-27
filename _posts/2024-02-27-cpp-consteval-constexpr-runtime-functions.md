---
title: "C++: `consteval` vs `constexpr` vs runtime functions"
categories:
  - C++
tags:
  - consteval
  - constexpr
---

There is [this in-depth blog post](https://biowpn.github.io/bioweapon/2024/02/17/constexpr-consteval-function.html)
going over `consteval, constexpr`, and runtime functions.

`consteval` functions are evaluated only at compile-time.

`constexpr` functions can be evaluated at compile- or runtime.

Main point of the post: They can all call each other as long as the taken code path (branch) fits the call type.

Side notes to keep in mind:
- The C++23 feature [`if consteval`](https://en.cppreference.com/w/cpp/language/if) can be especially useful for `constexpr`
  functions.
- `constexpr` functions can be implemented s.t. they are only callable either at compile-time XOR runtime.
  This is considered bad practice.
- C++23 Foot Gun: A templated `constexpr` function can be implicitly converted to a `consteval` function (_Escalation_).
