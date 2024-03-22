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
  - binary heap
  - heap sort
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

## Height balanced tree

A _(height) balanced tree_ is defined to have minimal height, i.e. $$ h = O(\log n) $$.
The _height balancing property_ is stricter and states that each node's subtrees differ at most by 1, i.e.

$$
\text{skew}(X) := \text{height}(X.right) - \text{height}(X.left) \in \{ -1, 0, 1 \}
$$

Many balancing schemes are available
- _AVL tree_: first proposed by Adelson-Velsky, and Landis in 1962
- Red-Black, Splay, 2-3, ...

### Rotation

Changing height but keeping traversal order is possible via _rotations_.
Example:
```txt
     _____<D>__     rotate_right(<D>)     __<B>_____
  __<B>__    <E>        =>               <A>    __<D>__
 <A>   <C>   / \                         / \   <C>   <E>
 / \   / \  /___\       <=              /___\  / \   / \
/___\ /___\         rotate_left(<B>)          /___\ /___\
```

### AVL tree

An _AVL tree_ maintains strictly the _height balancing property_.
Rebalancing might be needed for insertion/deletion which can be implemented as $$ O(\log n) $$ operation using rotations.
This needs node's height to be augmented into the node's item.

### Implementation

Usually a _Red-Black tree_ is used, e.g. C++ STL `std::map`.
It has a relaxed balancing method s.t. balancing occurs less frequently.

## Binary heap

A _binary heap_ is a complete tree satisfying the max-heap property.
It is useful to represent a priority queue, e.g. tasks to perform next.

### Implicit complete tree

A _complete tree_ is a tree for which all upper levels are filled and the last is partially filled starting from the left.
Such a tree can be implicitly represented by an array.
Consider the following example
```txt
                                       ______0____
[0,1,2,3,4,5,6,7,8,9]   <=>        ____1____   __2__
                                __3__    __4   5   6
                                7   8    9
```

Neighbors can be computed using index arithmetics:

$$
\text{left}(i) = 2i+1\,, \quad
\text{right}(i) = 2i+2\,, \quad
\text{parent}(i) = \left \lfloor \frac{i-1}{2} \right \rfloor
$$

Using arrays is important in practice for cache-friendliness.

### Max-heap property

We can declare an order in which each node's value must be greater or equal than its children's values.

### Dynamic operations

To _insert_ an element:

- Append new element to end.
- Check max-heap property of new element and parent.
    - If failed, then swap with parent and recurse.

Running cost is $$ O(\log n) $$.

To _pop_ the maximal element:

- Swap max with last element.
- Remove last element.
- Check max-heap property of new max/root/current element with its children.
    - Exit if satisfied.
    - Else: Swap with largest child. Recurse.

Running cost is $$ O(\log n) $$.

### Implementations

C++ provides [`std::make_heap()`](https://en.cppreference.com/w/cpp/algorithm/make_heap), and [`std::priority_queue`](https://en.cppreference.com/w/cpp/container/priority_queue).


## Heap sort

Builds up a binary heap by appending one item at a time with cost $$ O(n \log n) $$.

In-place optimization possible by using an increasing prefix of the input array `A`.
E.g. start with first element `A[0]`, push back `A[1]`, ...
