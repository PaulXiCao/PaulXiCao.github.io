---
title: "DSA: Single_Source_Reachability graph problem"
date: 2024-03-25
lesson: 8
layout: posts
categories:
  - it basics
tags:
  - DSA
  - graph
  - DFS
---

Another interesting graph problem is `Single_Source_Reachability(G, s)`.
It asks to return a parent tree of pointers back to `s` (aka which vertices are reachable and parents along which one would reach source `s`).
Note, this does not return distances nor does it return the shortest path!

An algorithm called _depth-first search (DFS)_ solves the above mentioned problem.
A sketch in pseudocode looks as follows

- initialization: `P(s) = None`
- start at root: `visit(s)`
  - definition of `visit(u)`:
    - for edges `(u,v)`
      - if `v` not in `P`
        - set `P(v)=u`
        - call `visit(v)`

Runtime is only $$ O (|E|) $$.
