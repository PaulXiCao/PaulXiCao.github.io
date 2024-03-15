---
title: "DSA: Sorting"
date: 2024-03-15
lesson: 3
layout: posts
categories:
  - it basics
tags:
  - DSA
---

Often one comes across the following problem: Given a fixed set of items which are addressed by keys that can be ordered.
The task is to do a large number of searches.

In an _unordered_ set one needs to do a slow _linear search_ with cost $$ O(n) $$.
If the data set were _ordered_ one could employ _binary search_ that has cost of only $$ O(\log n) $$.

Common features to compare sorting algorithms by:
- time cost
- space cost
    - in-place: only need $$ O(1) $$ further memory
- stable: does not change relative order of items with equal keys

Well-known searching algorithms:
- _Selection sort_
    - searches for largest element in unsorted section, and swaps it with back
    - iterates until sorted (leaving sorted back untouched)
    - $$ O(n^2) $$ cost
- _Insertion sort_
    - moves current element further upfront until smaller element found
    - $$ O(n^2) $$ cost
- _Merge sort_
    - sort current set and merge two neighboring sets
    - start with sets of size 1 and iterate until sorted
    - $$ O(n \log n) $$ cost which is optimal!
    - not in-place (merging needs additional space)

C++ standard library implementations: [`std::sort()`](https://en.cppreference.com/w/cpp/algorithm/sort), [`std::stable_sort()`](https://en.cppreference.com/w/cpp/algorithm/stable_sort).
