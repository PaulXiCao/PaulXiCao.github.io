---
title: "DSA: Introduction (I)"
date: 2024-03-14
lesson: 1
layout: posts
categories:
  - it basics
tags:
  - DSA
---

Computer Science and related studies typically include a lecture on _Data Structures and Algorithms (DSA)_ in their first semesters.
The purpose is to present solutions to common problems and lay the foundation needed to address more complex problems in later semesters.

## Problems

A problem is typically defined by a set of operations that need to be supported on a given dataset.
For example, operations like `sort_by_key()` or `find_min()`.

In the literature, you may encounter different terms for this concept: _problem, interface, or abstract data type (ADT)_.

## Solutions

A solution to such a problem involves a specific way to store data and algorithms to implement the required operations.
This is referred to as a _data structure_.

Algorithms are examined to show their _correctness_ aka "do they really solve the problem"?
Different Algorithms are compared according by their _efficiencies_ in time and space.
This is usually only done in an asymptotic fashion, e.g. if we would have a very large data set of size `n` then to do that operation would take at least `n^2` time.
Bounds can be specified as follows: upper bound $O$, lower bound $\Omega$, tight bound $\Theta$.

## Upcoming Content

Throughout this DSA series, we will cover fundamental concepts such as:
- Arrays
- Sorting
- Trees
- Searching
