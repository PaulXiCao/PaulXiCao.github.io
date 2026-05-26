---
title: "First Experiments with C++26 Contracts (and a GCC Crash)"
categories: [cpp, gcc, contracts, libstdc++]
tags: [c++, c++26, contracts, gcc, libstdc++]
---

I have recently been working on a hazard pointer implementation for `libstdc++` - more posts on that are coming soon - and decided to experiment with the upcoming C++26 contracts support along the way.

Unfortunately, my very first attempt immediately triggered a GCC crash.

The issue appears when attaching a `post()` condition to a defaulted in-class constructor:

```cpp
class Foo {
    Foo() post(true) = default;
};
```

GCC crashes during compilation instead of producing a diagnostic.

Since compiler crashes are always worth reporting, I filed a bug report on the GCC Bugzilla tracker as well: [bugreport](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=125403).

It is still early days for contracts support, so rough edges are expected, but hitting an ICE on the very first experiment raises concerns.
