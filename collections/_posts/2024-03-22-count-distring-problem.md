---
title: "Count-distinct problem"
date: 2024-03-16
lesson: 5
layout: posts
categories:
  - CS
tags:
  - algorithm
  - HyperLogLog
  - probabilistic counting
---

I have recently watched a talk on the Count-distinct problem.
Let me summarize it and give a bit of background information to this topic.
Resources:

- [(German) talk on YouTube](https://www.youtube.com/watch?v=bS9Qewqmd3c)
- [Count-distinct problem on Wikipedia](https://en.wikipedia.org/wiki/Count-distinct_problem)

## Count-distinct problem

Assume you have a large data set, and need to find the number of unique occurrences in it.
For many applications it is valid to give only an approximation of the number.
This opens the possibility for algorithms which use only a fraction compared to counting the exact number.

## Base algorithm

A general data set can contain any type of object, e.g. German car plates of Format `XX-YY 1234`.
We will convert them to a binary representation via hashing.
Finding a useful hash function is important and quite hard.



, e.g. AES
For generalization

We need to convert each object to something which is easy to handle
Each object is converted to something
