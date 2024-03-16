---
title: "DSA: Binary Tree"
date: 2024-03-16
lesson: 5
layout: posts
categories:
  - it basics
tags:
  - DSA
  - binary tree
---

Our goal is to have a data structure that is $$ O(\log n) $$ cost fast for all operations, e.g. `find()`, `insert()`, `find_min()`.

## Binary Tree

A _binary tree_ is a pointer-based data structure.
Each node saves a pointer to its parent, left child, and right child.

Example:
```txt
     __A__
    /     \
    B     C
   / \
  D   E
 /
F
```

Terminology:
- _root_: The root node and has no parent, e.g. node A.
- _leaf_: A leaf node has no children, e.g. node C.
- _height_: Maximal depth counted from the root.
- _traversal order_: Iterate nodes in left subtree, then the current node, then the right subtree
    - E.g. For the above example: `FDBEAC`

## Operations

Operations on binary trees are usually defined as recursive algorithms.
Some examples should show how to define all of the other ones.

- Tree navigation
    - `find_first(X)`: Find first node in subtree of node X
        - If X has left child, `return find_first(X.left_child)`
        - Otherwise `return X`
    - `find_successor(X)`: Find next node in traversal order after X
        - If X has right child: `return find_first(X.right_child)`
        - Else walk up ancestors until moved up from left child. Return ancestor.
- Dynamic operation
    - `insert_after(Y, X)`: Insert node Y after X in traversal order
        - If X has no right child: `X.right_child = Y`
        - Else `find_first(X.right_child).left_child = Y`
    - `delete(X)`: Delete node X
        - If X has no children: pop X from ancestor
        - Else if X has left child: `swap(X, predecessor(X)); delete(X);`
        - Else: `swap(X, successor(X)); delete(X);`

## Applications

For a Set Problem: The traversal order is determined by ordering the keys.

For a Sequence Problem: We need to support operations similar to finding the i-th item `find(i)`.
Possible if we save the number of children nodes of the full subtree per node.
This is a special case of _augmentation_, i.e. saving additional data in nodes.
