---
title: 'C++ CRTP vs concepts'
categories:
- programming
tags:
- c++
- CRTP
- concept
---

[CRTP](https://en.cppreference.com/w/cpp/language/crtp) is infamous for its complexity.
You can use C++20 concepts to reimplement the feature.

Source: [joelfilho.com](https://joelfilho.com/blog/2021/emulating_crtp_with_cpp_concepts/)


## CRTP Example

CRTP enables compile-time polymorphism as compared to runtime polymorphism via `virtual`.

```cpp
#include <iostream>

// base class for runtime polymorphism
class RuntimeExample {
 public:
  virtual void doWork() { std::cout << "RuntimeExample::doWork()\n"; }
};

// derived class for runtime polymorphism
class SpecificRuntimeExample : public RuntimeExample {
 public:
  void doWork() override { std::cout << "SpecificRuntimeExample::doWork()\n"; }
};

// base class for compile-time polymorphism
template <class actual_type>
class CRTPExample {
 public:
  void doWork() { static_cast<actual_type *>(this)->doWork_impl(); }

 private:
  void doWork_impl() { std::cout << "CRTPExample::doWork_impl()\n"; }
};

// derived class for compile-time polymorphism
class SpecificCRTPExample : public CRTPExample<SpecificCRTPExample> {
 public:
  void doWork_impl() { std::cout << "SpecificCRTPExample::doWork_impl()\n"; }
};

int main() {
  RuntimeExample *runtime_example = new SpecificRuntimeExample();
  runtime_example->doWork();

  CRTPExample<SpecificCRTPExample> crtp_example;
  crtp_example.doWork();
}
```
