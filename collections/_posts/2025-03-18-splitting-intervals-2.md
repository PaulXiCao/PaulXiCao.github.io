---
title: "Splitting intervals - Part 2: Making template errors suck less"
categories:
- C++
tags:
- varidic template
- constexpr
- compile-time
- C++20
---

Template error messages in C++ can be horrendeous.
Adding compile-time checks via `concept`s or `static_assert`s can go a long way.
Lets do this here.

The number of arguments should be checked first
```cpp
class interval {
 // ...
 public:
  template <class... Args>
  auto split(Args&&... args) const {
    constexpr size_t numArgs = sizeof...(Args);
    static_assert((numArgs >= 3) && (numArgs % 2 == 1),
                  "wrong number of arguments. at least 3.");
    // ...                  
```

Assigning each argument via tuple's `std::get<>` should get a check next, i.e.
```cpp
class interval {
 // ...
 private:
  template <size_t IntervalIdx, class Tuple>
  interval get_chunk(const Tuple& args) const {
    float relStart = 0;
    if constexpr (IntervalIdx != 0) {  // not the first
      constexpr size_t idxRelStart = IntervalIdx * 2 - 1;                                   // NEW
      static_assert(std::convertible_to<std::tuple_element_t<idxRelStart, Tuple>, float>);  // NEW
      relStart = std::get<idxRelStart>(args);
    }

    float relEnd = 1;
    if constexpr (IntervalIdx != std::tuple_size_v<Tuple> / 2) {  // not the last
      constexpr size_t idxRelEnd = IntervalIdx * 2 + 1;                                   // NEW
      static_assert(std::convertible_to<std::tuple_element_t<idxRelEnd, Tuple>, float>);  // NEW
      relEnd = std::get<idxRelEnd>(args);
    }

    constexpr size_t idxName = IntervalIdx * 2;                                                 // NEW
    static_assert(std::convertible_to<std::tuple_element_t<idxName, Tuple>, std::string_view>); // NEW
    return split_off_chunk(std::get<idxName>(args), relStart, relEnd);
  }
```

Next we could add some runtime-checks about the interval bounds and relative scaling parameters, i.e.
```cpp
class interval {
 public:
  interval(std::string_view name, float start, float end) : name(name), start(start), end(end) {
    if (start >= end) // NEW
      throw std::invalid_argument("`start` must be smaller than `end`!"); 
  }

  interval split_off_chunk(std::string_view name, float relStart, float relEnd) const {
    if (!((0 <= relStart) && (relStart < relEnd) && (relEnd <= 1))) // NEW
      throw std::invalid_argument(
          "`relStart`, `relEnd` must be relative values, e.g. `0 <= relStart < relEnd <= 1`!");
    // ...
```

A live-demo can be found [here](https://godbolt.org/z/qboaa1j1n).