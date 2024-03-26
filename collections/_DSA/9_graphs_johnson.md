---
title: "DSA: `All-Pairs Shortest Paths` graph problem"
date: 2024-03-26
lesson: 9
layout: posts
categories:
  - it basics
tags:
  - DSA
  - graph
  - Johnson
---

The `All_Pairs_Shortest_Path(G)` problem is interested in finding minimal distances between every vertex pair for a general graph `G` with arbitrary weights.

## Johnson's algorithm

- Construct `G_x` from `G` by adding a vertex `x` with outgoing 0-edge to every other vertex.
- Compute minimal distances `d_x(x,v)` for all vertices `v` (e.g. using Bellman-Ford)
- If `d_x(x,v)=-inf`: abort (negative weight cycle)
- Else:
  - Reweight each edge from `G` via `w'(u,v) := w(u,v) + d_x(x,u) - d_x(x,v)` to form `G'`
  - Compute `d'(u,v)` for `G'` (e.g. using Dijkstra)
  - Transform `d(u,v) := d'(u,v) - d_x(x,u) + d_x(x,v)`

This has running time $$ O ( |V|^2 \log |V| + |V| \, |E|) $$.

## Reasoning

We can change edge weights as long as for each vertex it holds: `change per incoming edge = - change per outgoing edge`.
As an example: for vertex `v3` which might have incoming edges `edge0, edge1`, and outgoing edges `edge2, edge3, edge4`.
We could change the weights by: `w(edge0) -= 3`, `w(edge1) -= 3`, `w(edge2) += 3`, `w(edge3) += 3`, `w(edge4) += 3`.

As change per vertex we use the minimal distance to some vertex.
This is obvious after rearranging formula.

Minimal distance is problematic as `G` might be disconnected.
Thus, we introduce a vertex `x` which is connected to every other vertex via a 0-weight outgoing edge to form `G_x`.
Compute distance via Bellman-Ford.

Changing the weights to `w'` as stated in the algorithm yields non-negative weights!
Thus, we can employ Dijkstra's Algorithm!
