---
title: "DSA: Introduction to graphs"
date: 2024-03-22
lesson: 6
layout: posts
categories:
  - it basics
tags:
  - DSA
  - graph
---

A _graph_ $$ G=(V, E) $$ is build up of _vertices_ $$ V $$, and _edges_ $$ E \subset V^2 $$.
Edges can be directed or undirected.
A simple graphs have utmost one edge between vertices and no edges start and finish at the same vertex.

Graphs are usually represented by an _adjacency list_.
A hash map that maps a vertex to an array containing all its out-connected vertices.
This implies that we need $$ \Theta ( |V| + |E|) $$ storage space.

A special graph type is the _DAG (directed acyclic graph)_ which has directed edges and no cycles.

Edges can be _weighted_ (e.g. for distances between cities).

There are multiple interesting problems related to paths on graphs.
In the following posts we will explore some of them.
