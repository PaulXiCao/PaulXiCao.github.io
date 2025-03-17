---
title: 'Splitting intervals or Let me try out this variadic template thingy (Part 1/n)'
categories:
- C++
tags:
- varidic template
- constexpr
- compile-time
- C++20
---

This will be a story on how to reduce repeating code via variadic templates (hello modern C++ ;) ).

The base problem starts with trying to split a custom interval type into smaller sub-intervals, e.g.
```cpp
class interval {
 public:
  interval(std::string_view name, float start, float end);

  /// Create sub-interval.
  /// @param relStart, relEnd Parameters with 0<=relStart < relEnd <= 1
  interval split_off_chunk(std::string_view name, float relStart, float relEnd) const;

 private:
  std::string name;
  float start;
  float end;
};
```

Usage is rather clunky and a common usecase of splitting one interval into consecutive sub-intervals includes repetetive code (e.g. `relEnd` must match next's `relStart`):
```cpp
const interval base("base interval", 7, 42);
const interval sub0 = base.split_off_chunk("first", 0, 0.2f);
const interval sub1 = base.split_off_chunk("second", 0.2f, 0.7f);
const interval sub2 = base.split_off_chunk("third", 0.7f, 1);
```

For a clearer API we want to supply the split-points only once. 
The sub-interval names would be given inbetween those split-points and thus, be self-documenting for those points.
```cpp
const auto [sub0, sub1, sub2] = base.split("first", 0.2f, "second", 0.7f, "third");
```

Implementing this for a fixed number of sub-intervals is possible.
There are some benefits and drawbacks:
- DRY: duplicated code is error-prone and hard to maintain
- simpler to understand
- only supports a limited number of cases

E.g. example for splitting into 2 or 3 sub-intervals
```cpp
class interval {
 // ...
 public:
  std::array<interval, 2> split(std::string_view name0, float relSplit01, std::string_view name1) const {
    return {split_off_chunk(name0, 0, relSplit01), split_off_chunk(name1, relSplit01, 1)};
  }

  std::array<interval, 3> split(std::string_view name0, float relSplit01,
                                std::string_view name1, float relSplit12,
                                std::string_view name2) const {
    return {split_off_chunk(name0, 0, relSplit01),
            split_off_chunk(name1, relSplit01, relSplit12),
            split_off_chunk(name2, relSplit12, 1)};
  }
};
```

Supporting arbitrary number of sub-intervals is possible via [variadic templates](https://en.cppreference.com/w/cpp/language/pack). 
The entry point is this `split` function
```cpp
class interval {
 // ...
 public:
  template <class... Args>
  auto split(Args&&... args) const {
      const std::tuple argsTuple = std::forward_as_tuple(std::forward<Args>(args)...);
    return [&argsTuple, this]<size_t... Idx>(std::index_sequence<Idx...>) {
        return std::array{get_chunk<Idx>(argsTuple)...};
    }(std::make_index_sequence<sizeof...(Args)/2+1>{});
  }
};
```
The implementation is non-trivial.
Lets delve deeper into its details:
- `template <class... Args>` This is the syntax for variadic templates. The function could get instantiated for an arbitrary number of arguments. In our case for example as `split<string_view, float, string_view>`.
- `auto` return type. We want to return different number of sub-intervals, depending on the input. This is possible by not explicitly stating the return type here.
- `Args&&...` varidic template types of universal references.
- `std::forward_as_tuple` Generates a wrapper around the variadic object `args`. It is a std::tuple and thus, easier to handle (e.g. allows direct indexing and we do not need to wait for C++26 pack indexing).
- `std::forward<Args>(args)...` The `forward` is necessary to implement perfect forwarding (aka forward l- and r-values efficiently without copying). The `...` ellipsis is part of the "fold" expression (aka `forward` will be applied to each argument of `args` separately).
- `size_t... Idx` Iterating over a compile-time range (e.g. `Idx=0,..,num_intervals-1`) is possible via the construct of templated lambda, `index_sequence`, `make_index_sequence`. Note, this allows constructing a `std::array` of non-default constructible underlying-type.

Computing the separate sub-intervals is implemented within the `get_chunk()` method
```cpp
class interval {
 // ...
 private:
  template <size_t IntervalIdx, class Tuple>
  interval get_chunk(const Tuple& args) const {
    float relStart = 0;
    if constexpr (IntervalIdx != 0) {  // not the first
      relStart = std::get<IntervalIdx * 2 - 1>(args);
    }

    float relEnd = 1;
    if constexpr (IntervalIdx != std::tuple_size_v<Tuple> / 2) {  // not the last
      relEnd = std::get<IntervalIdx * 2 + 1>(args);
    }

    return split_off_chunk(std::get<IntervalIdx * 2>(args), relStart, relEnd);
  }
```

A live-demo can be found [here](https://godbolt.org/z/3PMjaoMTo).

Credits to Ted Lyngmo for [his help via stackoverflow](https://stackoverflow.com/a/79511964/10774817).

In the next parts we will expand on the following:
- add compile-time checks to assert the input types for better error messages
- complicating things abit with a trailing boolean argument
- more ?