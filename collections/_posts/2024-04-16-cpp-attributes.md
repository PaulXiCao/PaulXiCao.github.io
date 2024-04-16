---
title: 'C++ Attributes'
categories:
- programming
tags:
- c++
- attribute
- empty base optimization
---

C++ code can contain [attributes](https://en.cppreference.com/w/cpp/language/attributes) (e.g. `[[deprecated]]`).
These serve different purposes, ranging from silencing compiler warnings to changing optimization strategies.
This [C++ Weekly episode](https://www.youtube.com/watch?v=BpulWncdn9Y) gives a summary of all attributes upto C++23.

## Overview

- C++11: `[[noreturn]]`, `[[carries_dependency]]`
- C++14: `[[deprecated(<msg>)]]`
- C++17: `[[fallthrough]]`, `[[nodiscard]]`, `[[maybe_unused]]`
- C++20: `[[likely]]`, `[[unlikely]]`, `[[no_unique_address]]`
- C++23: `[[assume(<expr>)]]`

## `[[noreturn]]`

[`[[noreturn]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/noreturn)

Indicating that a function does not return control flow to calling function.
Possibly handy for handling error cases which `throw`.

Example
```cpp
// void func();           // yields compiler warning: "control reaches end of non-void function [-Wreturn-type]"
[[noreturn]] void func(); // compiles fine

int getValue(int i) {
  if (i<2) return i;

  func();
}
```

## `[[carries_dependency]]`

This is a niche and poorly understood feature according to Jason.
Let me just refer you to [cppreference](https://en.cppreference.com/w/cpp/language/attributes/carries_dependency).

## `[[deprecated(<msg>)]]`

[`[[deprecated(<msg>)]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/deprecated)

One can mark different objects in code as deprecated, e.g. classes, functions, namespaces.
This will yield compiler warnings if they are used nonetheless.

They can contain optionally contain a message, i.e. `[[deprecated]]`, and `[[deprecated("some message")]]` are valid.

Example
```cpp
[[deprecated("upgrade to `foo2`")]] void foo();
```

## `[[fallthrough]]`

[`[[fallthrough]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/fallthrough)

Allow statements in `switch` cases to fall through to the next case.

```cpp
void foo();

int bar(int i) {
  switch (i) {
    case 0:  // no fallthrough needed
    case 1:
      return 42;
    case 2:
      foo();
      [[fallthrough]];  // compiler warning if left out
    case 3:
      return 7;
    default:
      return -1;
  }
}
```

## `[[nodiscard]]`

[`[[nodiscard]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/nodiscard)

Indicating that a function's return value should not be neglected.

Example
```cpp
int foo() {return 42;}
[[nodiscard]] int bar() {return 7;}

int main() {
  foo();            // return value can be dropped
  int i = bar();    // return value must be used
}
```

## `[[maybe_unused]]`

[`[[maybe_unused]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/maybe_unused)

Suppress compiler warnings on unused entities, e.g. classes, functions.

Example
```cpp
int main(){
  [[maybe_unused]] int i;
}
```

## `[[likely]]`, `[[unlikely]]`

[`[[likely]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/likely)

Hints to compiler which code path to optimize for.
A summary of [this talk](https://youtu.be/RjPK3HKcouA?si=K62DDzCoyuHUmR2L) is that one should best avoid theses attributes as compilers optimize better in most cases without them.

## `[[no_unique_address]]`

[`[[no_unique_address]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/no_unique_address)

Allow overlapping of data members for memory optimization.

Example
```cpp
struct Empty {};

struct Foo {
  Empty e;
  int i;
};

struct Bar {
  [[no_unique_address]] Empty e;
  int i;
};

int main() {
  static_assert(sizeof(Foo)  > sizeof(int));
  static_assert(sizeof(Bar) == sizeof(int));
}
```

To understand the above example one needs to know that every class has at least size 1, e.g. `sizeof(Empty) == 1`.
One usually works around this by using the _Empty Base Optimization (EBO)_ technique, e.g.
```cpp
struct Empty {};

struct Foo {
  Empty e;
  int i;
};

struct Bar : Empty // <== EBO
{
  int i;
};

int main() {
  static_assert(sizeof(Foo)  > sizeof(int));
  static_assert(sizeof(Bar) == sizeof(int));
}
```
This is used for to implement [compressed pairs](https://devblogs.microsoft.com/oldnewthing/20230801-00/?p=108509) which is similar to `std::pair` but if one element is empty then the compressed pair should have the same size as the other element.

## `[[assume(<expr>)]]`

[`[[assume(<expr>)]]` on cppreference](https://en.cppreference.com/w/cpp/language/attributes/assume)

Telling the compiler that a specific assumption holds to enable further optimizations.
Do use with care as the same code will be executed even when the assumption does not hold, e.g. it is not part of the functions API but only visible in the implementation!

Example:
```cpp
int divideByFour_noAssume(int i) {
  return i / 4;
}

int divideByFour_assume(int i) {
  [[assume(i > 0)]];
  return i / 4;
}
```
[godbolt link](https://godbolt.org/z/qPj7dnscT) showing that the assumption produces simpler assembly code.
