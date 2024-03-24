---
title: "DSA: Graphs"
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

A special graph type is the _DAG (directed acyclic graph)_ which has directed edges and no cycles.

Edges can be _weighted_ (e.g. for distances between cities).

## Algorithms for the SSSP problem

There are multiple interesting problems related to paths on graphs.
One such problem we will delve into is the `Single_Source_Shortest_Paths(G, s)` problem, where our objective is to find the shortest distance from a starting point `s`, to any other reachable vertex.

We will investigate various algorithms tailored to different assumptions regarding the type of graph and the weights associated with its edges.

| Restrictions |              | SSSP Algorithm |                               |
| Graph        | Weights      | Name           | Running Time $$ O( \cdot ) $$ |
|--------------|--------------|----------------|------------------------------:|
| General      | Unweighted   | BFS            | $$ |V| + |E| $$               |
| DAG          | Any          | DAG Relaxation | $$ |V| + |E| $$               |
| General      | Any          | Bellman-Ford   | $$ |V| \cdot |E| $$           |
| General      | Non-negative | Dijkstra       | $$ |V| \log |V| + |E| $$      |

## Breadth-first search

A standard algorithm for unweighted general graphs resulting in a linear runtime.

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

## Bellman-Ford

For a general, weighted graph we can only get an algorithm in $$ O (|V| \cdot |E|) $$ which can scale as bad as $$ O(|V|^3) $$ for nearly complete graphs.

### Algorithm

Iteration works like this:
- initialization:
  - distance[s] = 0
  - distance[v] = inf for v not s
- iterate for layers `i=1,..,|V|-1`:
  - for all edges (u,v): if `distance[u] + weight[u,v] < distance[v]` then update `distance[v]`
- layer `i=|V|`:
  - same check as for pervious layers. if update needed, then negative cycle must be present on path.

### Reasoning

To differentiate between simple paths and paths containing cycles we look at paths containing $$ |V|-1 $$ and $$ |V| $$ vertices.
Therefore, we copy the original graph $$ |V| $$ times and position them in layers.

Now, we define paths by following an edge of the original graph and going one layer up at the same time.
This way we created a DAG for which we can already find solutions to the SSSP problem.
Note that these paths between graph copies/layers imitate short paths by staying at the same original vertex as it moves up one layer.

We keep track of the minimal distance as we build up the paths.
One layer at a time.
