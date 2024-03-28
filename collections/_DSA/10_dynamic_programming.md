---
title: "DSA: Dynamic Programming"
date: 2024-03-28
lesson: 10
layout: posts
categories:
  - it basics
tags:
  - DSA
  - dynamic programming
---

## Introduction

This far we have seen solutions to standard problems, e.g.
- Search data structures
  - sorted array
  - hash table
  - binary heap
- Sort algorithms
  - merge sort
  - heap sort
- Graph algorithms
  - Breadth-first search
  - Dijkstra

We will look into solving custom problems using recursive algorithms in a general manner.
The problems can be classified by their graph representation, i.e.

| Class                 | Graph    |
|-----------------------|----------|
| Brute Force           | Star     |
| Decrease & Conquer    | Chain    |
| Divide & Conquer      | Tree     |
| _Dynamic Programming_ | _DAG_    |
| Greedy/Incremental    | Subgraph |

## Dynamic programming

_Dynamic programming_ is a generalization of "Divide & Conquer" where the subproblems form a DAG instead of tree.
It often comes up in optimization problems of a single scalar value.
As it is intended to be a recursive algorithm we will need to think of subproblems forming the original problem in the end.
A general framework by the acronym _SRTBOT_ helps solving these kinds of problems.

### SRTBOT

To solve a general dynamic programming problem one can follow the framework with the acronym _SRTBOT_.

1. *S*ubproblem: Define a subproblem.
2. *R*elate: Write a recursive relation of subproblems.
3. *T*opological order: Define how to iterate the recursive relations.
  Making sure the relation is acyclic.
4. *B*ase case: State solution for all initial problems.
5. *O*riginal problem: Describe which subproblem solution solves the question.
6. *T*ime analysis: Analyze runtime cost by adding work per subproblem and counting recursive calls as constant runtime.

### Examples

To understand this framework and how to apply it, one should look at a group of examples.

These high quality [lecture notes](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/pages/lecture-notes/) (i.e. lectures 15-18) provide a large facet of different types of examples.
You can either watch the lectures on YouTube or just scan the pdfs.
