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

## DAG Relaxation

Allowing non-constant weights of _positive and negative_ integral values can be solved in linear runtime $$ O ( | V | + | E | ) $$ under the _assumption of a DAG_.
The algorithm is usually called _DAG relaxation_ where distances are upper bounded and consecutively lowered aka "relaxed".

### Algorithm

- initialization
  - `distance[s,s] = 0`
  - `distance[s,v] = inf` for v not s
- for vertices `u` in G (in topological sort order)
  - for outgoing edges `(u,v)` from `u`
    - update `distance[s,v]` if `distance[s,u] + weight[u,v]` is smaller (aka "_relax_ edge")

Note, each DAG `G` has at least one topological sort order.
[Definition from Wikipedia](https://en.wikipedia.org/wiki/Topological_sorting): "...for every directed edge (u,v) from vertex u to vertex v, u comes before v in the ordering."

## Bellman-Ford

For a general, weighted graph we can only get an algorithm in $$ O (|V| \cdot |E|) $$ which can scale as bad as $$ O(|V|^3) $$ for nearly complete graphs.

### Algorithm

Iteration works like this:
- initialization
  - `distance[s,s] = 0`
  - `distance[s,v] = inf` for v not s
- iterate for layers `i=1,..,|V|-1`
  - for edges `(u,v)`
    - update `distance[s,v]` if `distance[s,u] + weight[u,v]` is smaller
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

## Dijkstra

[_Dijkstra's Algorithm_](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) reduces the runtime of Bellman-Ford to $$ O (|V| \log |V| + |E|) $$ which is almost linear.
It applies to general graphs under the assumption of non-negative edge weights.

### Algorithm

We make use of a _changeable priority queue_ `Q` which stores items `x = (x.id, x.key)` and supports the following operations:
- priority queue's interface: `Q.build(X)`, `Q.delete_min()`
- update item: `Q.decrease_key(id, newKey)`

Algorithm:
- initialization:
  - `distance[s,s] = 0`
  - `distance[s,v] = inf` for v not s
- build up changeable priority queue `Q` with items `(vertexId, distance)`
- visit all vertices and update distances:
  - while Q not empty: pop minimum `(u, distance[s,u])`
    - for all outgoing edges `(u,v)` from `u`
      - update `distance[s,v]` in `Q` if `distance[s,u] + weight[u,v]` is smaller

### Implementation

The changeable priority queue needs to cross-link a priority queue with a dictionary.
In the general case one can use a [Fibonacci Heap](https://en.wikipedia.org/wiki/Fibonacci_heap).

Usually one has problem specific a-priori knowledge of the graph such that one can use simpler data structures instead:
- for dense graphs: use an array
- for sparse graphs: use a binary heap

## Depth-first search

Solves a different path finding problem: `Single_Source_Reachability(G, s)`.
Which needs to return a parent tree of pointers back to `s` (aka which vertex is reachable and parents along which one would reach source `s`).
Note, this does not return distances nor does it return the shortest path!

### Algorithm

- initialization: `P(s) = None`
- start at root: `visit(s)`
  - definition of `visit(u)`:
    - for edges `(u,v)`
      - if `v` not in `P`
        - set `P(v)=u`
        - call `visit(v)`

Runtime is only $$ O (|E|) $$.
