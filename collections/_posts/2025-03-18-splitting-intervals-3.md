---
title: "Splitting intervals - Part 3: Adding a trailing, optional boolean argument"
categories:
- C++
tags:
- varidic template
- constexpr
- compile-time
- C++20
---

The intervals should get a custom boolean flag which will be supplied via a trailing, defaulted argument.
Adding this to splitting functions for a hard-coded number of intervals (e.g. `std::array<interval, 2> interval::split(std::string_view, float, std::string_view, bool flag=true)`) is trivial.
It gets more complicated when using our varidic template `split` function.

At first we will need to edit the ctor and base splitting function, i.e.
```cpp
class interval {
 public:
  interval(std::string_view name, float start, float end, bool flag=true);

  interval split_off_chunk(std::string_view name, float relStart, float relEnd, bool flag=true) const {
    // ...
 private:
  bool flag;
  // ...
```

Next we will introduce a method to compute how many intervals and if the boolean `flag` were supplied  via the `Args&&... args` to the `split` function. 
More precisly, this is done on the `Tuple tup = std::forward_as_tuple...` object, i.e.
```cpp 
class interval {
 private:
  template <class Tuple>
  static consteval std::pair<size_t, bool>
  get_num_intervals_and_contains_flag() {
    constexpr size_t numArgs = std::tuple_size_v<Tuple>;
    static_assert(numArgs >= 3, "wrong number of arguments. at least 3.");

    size_t numIntervals;
    bool containsFlag;
    if (numArgs % 2 == 0) {
      static_assert(std::convertible_to<std::tuple_element_t<numArgs - 1, Tuple>, bool>);
      numIntervals = numArgs / 2;
      containsFlag = true;
    } else {
      numIntervals = numArgs / 2 + 1;
      containsFlag = false;
    }
    return {numIntervals, containsFlag};
  }

  template <class Tuple>
  static consteval size_t get_num_intervals() { return get_num_intervals_and_contains_flag<Tuple>().first; }

  template <class Tuple>
  static consteval bool get_contains_flag() { return get_num_intervals_and_contains_flag<Tuple>().second; }
  // ...
```

Lets fix the `split` function
```cpp
class interval {
 private:
  auto split(Args&&... args) const {
    constexpr size_t numArgs = sizeof...(Args);
    static_assert((numArgs >= 3) && (numArgs % 2 == 1), "wrong number of arguments. at least 3.");

    const std::tuple argsTuple =
        std::forward_as_tuple(std::forward<Args>(args)...);
    constexpr size_t num_intervals = get_num_intervals<decltype(argsTuple)>();  // NEW
    return [&argsTuple, this]<size_t... Idx>(std::index_sequence<Idx...>) {
      return std::array{get_chunk<Idx>(argsTuple)...};
    }(std::make_index_sequence<num_intervals>{});                               // NEW
  }
  // ...
```

The `get_chunk` function needs to be adjusted
```cpp
class interval {
 private:
  template <size_t IntervalIdx, class Tuple>
  interval get_chunk(const Tuple& args) const {
    // ...
    float relEnd = 1;
    if constexpr (IntervalIdx != get_num_intervals<Tuple>() - 1) {  // NEW
      constexpr size_t idxRelEnd = IntervalIdx * 2 + 1;
      static_assert(std::convertible_to<std::tuple_element_t<idxRelEnd, Tuple>, float>);
      relEnd = std::get<idxRelEnd>(args);
    }

    constexpr size_t idxName = IntervalIdx * 2;
    static_assert(std::convertible_to<std::tuple_element_t<idxName, Tuple>, std::string_view>);
    const std::string_view name = std::get<idxName>(args); // NEW

    // NEW
    if constexpr (get_contains_flag<Tuple>()) {
      const bool flag = std::get<std::tuple_size_v<Tuple> - 1>(args);
      return split_off_chunk(name, relStart, relEnd, flag);
    } else {
      return split_off_chunk(name, relStart, relEnd);
    }
  }
  // ...
```

A live-demo can be found [here](https://godbolt.org/z/77sqczjnr).

---

This was a fun little adventure in the realms of template programming.
Its implementation is far from trivial and not easy to maintain (always think of the question by the next junior dev joining your team ;) ).
Thus, be sure to wager the pros and cons of such a solution on a case-by-case basis.

An alternative simpler implementation would be possible when supplying names and split-points via two vectors, e.g.
```cpp
const auto [sub0, sub1, sub2, sub3, sub4] = base.split(
  {"first", "second", "third", "fourth", "fifth"},
    {0.2f, 0.3f, 0.7f, 0.9f});
```
But we would loose the documentation feature of alternating arguments (can you easily tell the split points for the fourth sub-interval `sub3`?).

I might add some more functionality to this at some later point.
It could be interesting to optionally not split the whole interval:
Introduce an optional split point argument before the first interval's name, e.g. `split(float relSplit0=0, std::string_view name0, ...)`.
Similarly, one after the last interval's name, e.g. `split(..., std::string_view nameLast, float relSplitLast=1)`.
Note, the interesting interaction with the optional trailing boolean argument `split(..., std::string_view nameLast, bool flag=true)`.
