---
title: "DSA: Breadth-first search"
date: 2024-03-22
lesson: 6
layout: posts
categories:
  - it basics
tags:
  - DSA
  - graph
  - BFS
---

## Graph basics

_Graph_ $$ G=(V, E) $$ is build up of _vertices_ $$ V $$, and _edges_ $$ E \subset V^2 $$.
Edges can be directed or undirected.
A simple graphs have utmost one edge between vertices and no edges start and finish at the same vertex.

Graphs are usually represented by an _adjacency list_.
A hash map that maps a vertex to an array containing all its out-connected vertices.
This implies that we need $$ \Theta ( |V| + |E|) $$ storage space.

We will look into multiple algorithms concerning paths on graphs.

## Breadth-first search

The BFS is concerned with the `Single_Source_Shortest_Paths(G, s)` graph problem.
For which we need to return the minimal distance to any reachable vertex starting at `s`.

### Shortest path tree

The desired distances will be returned via a tree-like structure:
For each vertex `v` store the parent `P(v)` for a minimal path towards `s`.
This is a space optimization.

### Algorithm

Goal: Compute minimal distances `d(s, v)`, and parents `P(v)`.

- Store level sets: vertices with same distance to `s`.
- Compute those level sets by iteratively walking along edges.
    - Starting at `s`.
    - Walk edge and check if new vertex is unvisited (not in any previous level set).

Running time is $$ O ( | V | + | E | ) $$ .
