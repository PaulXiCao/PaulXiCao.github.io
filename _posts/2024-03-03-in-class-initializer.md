---
title: "C++: in-class-initializer"
categories:
  - C++
tags:
  - c++11
  - initialization
---

C++ is infamous for its extremely complex initialization.
Since C++11 one can initialize class members directly at point of declaration.
The official name is [_default member initializer_](https://en.cppreference.com/w/cpp/language/data_members#Member_initialization) but more commonly known as _in-class-initializer_.
This is advised by the [CppCoreGuidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c45-dont-define-a-default-constructor-that-only-initializes-data-members-use-default-member-initializers-instead).

Example:
```C++
class BadExample {
    string s;
public:
    BadExample() : s{"default"} { }
};

class GoodExample {
    string s {"default"};
public:
    // use compiler-generated default constructor
};
```
