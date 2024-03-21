---
title: 'Talk: Forwarding References'
categories:
- C++ Talks
tags:
- c++
- CppCon
- forwarding reference
- perfect forwarding
- perfect returning
- generic programming
---

Talk: _CppCon23: Back to Basics: Forwarding References - How to Forward Parameters in Modern C++ - Mateusz Pusz 2023_

Resources:
- [video on YouTube](https://youtu.be/0GXnfi9RAlU?si=_THsfxtB7iJQFr2Y)
- [slides](https://github.com/CppCon/CppCon2023/blob/c2a62d20cc34ad5ef88893b03fdcd30e5aa13cd9/Presentations/B2B_Forwarding_References.pdf)
- [Forwarding References on cppreference](https://en.cppreference.com/w/cpp/language/reference#Forwarding_references)

A well-rounded overview about the topic of Forwarding References.

## Introduction

In the context of generic programming one may write wrapper-like functions for general arguments `T` and forwards them to other functions.
Depending on how the argument was supplied a different underlying function should be called (e.g. call-by-value, call-by-reference, passing rvalues).

## Example: Wrapper using forwarding references

```cpp
#include <utility> // forward, move

// string_views only used to have readable asserts in main
#include <string_view>
using SV = std::string_view;

constexpr void do_something() {}

constexpr SV f(      int& ) { return "int&";       }
constexpr SV f(const int& ) { return "const int&"; }
constexpr SV f(      int&&) { return "int&&";      }

constexpr SV badWrapper(      int&  v) { do_something(); return f(          v ); }
constexpr SV badWrapper(const int&  v) { do_something(); return f(          v ); }
constexpr SV badWrapper(      int&& v) { do_something(); return f(std::move(v)); }

template <typename T>
constexpr SV goodWrapper(T&& v) { do_something(); return f(std::forward<T>(v)); }

int main() {
  int mutInt;
  const int constInt = 42;

  static_assert("int&"       == badWrapper(mutInt  ));
  static_assert("const int&" == badWrapper(constInt));
  static_assert("int&&"      == badWrapper(42      ));

  static_assert("int&"       == goodWrapper(mutInt  ));
  static_assert("const int&" == goodWrapper(constInt));
  static_assert("int&&"      == goodWrapper(42      ));

  return 0;
}
```

As explained above we are interested in writing wrapper-like functions (i.e. `badWrapper()`, and `goodWrapper()` wrap `f()`).
There are multiple points to make here:

- The `badWrapper()` is inferior as it introduces lots of code duplication.
  Especially for multiple arguments (many combinations to handle).
- The `badWrapper(int&&)` forwards its argument correctly via `std::move()` as `int&& v` is a named object (alas no temporary) and thus, needs moving.
- The `goodWrapper()` needs two points to function properly:
  - The _forwarding reference_ `T&& v` "preserves the value category of the function argument".
    High talk for matching `int&`, `const int&`, and even `int&&` in our example above.
  - The argument must be forwarded via `std::forward<T>()` which pass its argument as is or in the case of rvalues it will call `std::move()` (aka converting the named argument back to an rvalue).
- Both wrapper implementations will yield the same binary.
  Thus, this is no performance optimization but rather about DRY (don't repeat yourself) and maintainability.

## Perfect returning

A similar problem occurs when trying to wrap functions returning different types.
This is called _perfect returning_ and the following example shows how to implement such a wrapper.

```cpp
#include <type_traits>  // is_same_v
#include <utility>      // forward

      int& f(      int&  x){ return x; }
const int& f(const int&  x){ return x; }
      int  f(      int&& x){ return x; }

template <typename T>
decltype(auto) wrapper(T&& x) { return f(std::forward<T>(x)); }

int main() {
  int mutInt;
  const int constInt = 42;

  static_assert(std::is_same_v<      int&, decltype(wrapper(mutInt  ))>);
  static_assert(std::is_same_v<const int&, decltype(wrapper(constInt))>);
  static_assert(std::is_same_v<      int , decltype(wrapper(42      ))>);

  return 0;
}
```

The return type `decltype(auto)` is needed to return correctly, i.e. temporaries by value and references by reference.

## Summary and guidelines

- Use `const MyType&` if read-access is all that is needed.
- For sinks consider `MyType` first.
  If the additional move needs to be optimized away, then consider `const MyType&`, `MyType&&`.
  (sink: consumes given object.)
- Use `T&&` to forward.
  Remember this is not a rvalue reference.
- Use `decltype(auto)` to perfectly return another functions invocation.
- Use `auto&&` to accept forwarding references.
  Also useful within range-for loops.
