---
title: "DSA: Hashing"
date: 2024-03-16
lesson: 4
layout: posts
categories:
  - it basics
tags:
  - DSA
  - hash table
---

Previously we have seen that finding a key in a sorted array takes $$ O(\log n) $$.
There is an alternative which takes only $$ O(1) $$ but has its own drawbacks.

## The concept of hashing

We can employ our whole RAM (formally a _Direct Access Array_) to store each given item at the position specified by its key (assuming integer representation).

You need to allocate the memory for which you might want to save items, i.e. for every possible key.
The space of possible keys is typically very large relative to the size of RAM or even to the number of items to store.

To minimize the space overhead one employs the idea of _hashing_ where the space of keys is mapped to a smaller range, i.e.

$$
h : \{ \text{key}_0 , \dots, \text{key}_{n-1} \} \rightarrow \{ 1, \dots, m \}
$$

where $$ h(\text{key}_i) $$ is the _hash_ of a given key and $$ m \ll n $$.

The array holding the items together with the hash function is called a _hash table_.

## Collisions

A _collision_ occurs when the hash function maps two different keys to the same hash, i.e.
$$
\text{key}_i = \text{key}_j \quad \text{for} \quad i \neq j \,.
$$
This is possible as $$ m < n $$.

There are multiple methods to work around this issue:
- _Chaining_: The hash table stores only pointers to a separate data structure that holds the items, e.g. a linked list.
- _Open addressing_: The key's hash is used as an initial index into the hash table.
  If that slot is already taken then other indices are computed iteratively.
  This is possible as the hash table marks its slots as empty, filled, or deleted.

Open addressing is the method used most often in real-world implementation (e.g. Python dictionaries).

## Hash functions

Standard examples of hash functions use modulo operations, e.g.
$$
h(k) := k \; \text{mod } m \,.
$$

The runtime behavior strongly depends on the given input data.
One switches from standard runtime analysis to an _expected cost_ which averages over all possible input data.
This is similar to amortized cost which averages over applying the same operation multiple times.

## Implementations

The C++ STL contains multiple hash table data structures.
They contain the prefix "unordered_", e.g. `unordered_set`, `unordered_map`.

In Python hash tables are available via dictionaries.
