---
title: "DSA: Data Structures"
date: 2024-03-14
lesson: 2
layout: posts
categories:
  - it basics
tags:
  - DSA
  - array
  - linked list
  - amortized cost
---

We will cover two problems:
1. _Sequence_: A group of ordered items `x0,..,xN`.
    Operations to perform: access/insertion/deletion by index `i`, etc.
2. _Set_: A group of unordered items `x0,..,xN` with keys `k0,..,kN`.
    Operations to perform: access/insertion/deletion by key `k`, etc.

## Array

A _(static) array_ consists of a contiguous chunk of memory of predefined length.
General properties: Fast access, slow insertion/deletion.

C++ example:
```C++
#include <array>

std::array<int, 3> a{7, 42, -1};
```

Note, this should be the default data structure for everyday programming under some conditions, e.g. size stays approximately fixed and is known beforehand.
The great advantage is that it is allocated on the stack (circumvents heap allocation) for reasonable sizes.

## Linked lists

A _(singly-)linked list_ is a chain of separately heap-allocated chunks.
In theory it is good for arbitrarily insertion/deletion.

The _"singly"_ highlights that each node only contains the information where the next item is.
Thus, one can only walk in one direction: from head to tail.
In contrast, a _doubly-linked list_ has nodes which contain information about the previous item s.t. one can also walk from its tail to the head (reverse order).

C++ example:
```cpp
#include <forward_list>
#include <list>

#include <iostream>
#include <ranges> // reverse

int main() {
  // singly-linked list
  {
    std::forward_list fwdList{"Hello ", "from ", "singly-linked ", "list!"};

    for (auto &&w : fwdList)
      std::cout << w;
    std::cout << '\n';

    /* fails to compile: cannot reverse iterate a singly-linked list!
    for (auto&& w : std::ranges::views::reverse(fwdList))
        std::cout << w;
    std::cout << '\n';
    */
  }

  // doubly-linked list
  {
    std::list list{"Hello ", "from ", "doubly-linked ", "list!"};
    for (auto &&w : list)
      std::cout << w;
    std::cout << '\n';

    // iterates: rbegin(), rbegin()++, .., rend()--
    for (auto &&w : std::ranges::views::reverse(list))
      std::cout << w;
    std::cout << '\n';
  }
}
```

Note, in practice the CPU needs to do lots of slow "pointer chasing".
This also prevents possible optimizations if the CPU could work on contiguous memory ("loading of cache lines").
Thus, it is very rarely used.

## Dynamic Array

A _dynamic array_ is a heap-allocated object of contiguous memory.
The number of items the object could currently hold is called its _capacity_.
The actual number that are currently stored is called its _size_.
Make sure to differentiate those two sizes and remember that the size is less or equal to the capacity.

Usual implementations allow for fast appending of data at the end.
Appending would increase the size and incase that it would grow beyond the capacity a _relocation_ needs to happen.
For that a larger (usually by a factor of 2) block of memory is allocated, the old data copied therein, and the old block deallocated.

Similarly, if data is removed from the end multiple times and the size is smaller than a threshold (relative to its current capacity) another relocation will occur.
This is an optimization for memory usage.

C++ example:
```cpp
#include <vector>

#include <print>

int main() {
  std::vector<int> v;

  for (int i = 0; i < 13; ++i) {
    std::print("size: {}, capacity: {}\n", v.size(), v.capacity());
    v.emplace_back();
  }

  return 0;
}
```
Appending data in C++ is supported via `push_back()`, and its in-place cousin `emplace_back()`.

In Python dynamic arrays are available via lists.
Take note of the `append()` method and [list comprehension](https://docs.python.org/3/tutorial/datastructures.html#list-comprehensions).

## Amortized Cost

Some operations have fixed costs, e.g. accessing an element in a static array.
Pushing back items in a dynamic array shows a different behavior.
For the case that `size < capacity` it is an O(1) operation, but for `size == capacity` the relocation shows O(n) runtime behavior (neglecting heap allocation overhead).
Thus, it makes sense to talk about an averaged cost for this operation aka an _amortized cost_.



