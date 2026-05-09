---
title: 'Status update of work on `libc++`: cartesian product view'
categories:
- C++
tags:
- llvm
- libc++
---

Initial post of libc++ contribution: [My first patch to `libc++`]({% link _posts/2024-07-27-my-first-patch-to-libc++.md %}).

# C++23 `std::ranges::views::cartesian_product`

I had started work on the C++23 `views::cartesian_product` implementation roughly 2 years ago and finally came back around to it ([mr link](https://github.com/llvm/llvm-project/pull/111215/)).
Implementation was mostly done but testing was almost non-existent.
This proved to be a perfect use case of AI assistance.
It read the test folders of other views and then created a comprehensive test structure (mostly inspired by `views::zip`'s tests).
Thus, for the ~500LOC main implementation another ~2500LOC tests were added.
It was quite time consuming even to only understand all of this test code.
Only some minor bugs were found in the main implementation.

There was already a merge request review done right after rebasing the mr and before adding the tests.
Fingers crossed, someone will look through this large mr.

# C++26 Remove `constexpr` from `owner_less/before`

I had a look into this C++26 issue and as it turned out it was already implemented.
Thus, nothing more to do than update the paper status.

[mr link](https://github.com/llvm/llvm-project/pull/191534).

# C++17 PSTL's `std::transform_{ex,in}clusive_scan`

This was a very interesting case ([mr link](https://github.com/llvm/llvm-project/pull/195548))!
The two functions `std::transform_{ex,in}clusive_scan` were missing their parallel STL overloads.
Adding them was non-trivial as one had to go through 3 layers of abstraction within libc++'s PSTL implementation.
AI was helpful in multiple ways here:
- quickly understanding this section of the code base
- discussing possible implementations
- initially implementing the algorithms
- creating tests

One possible explanation why AI performed this well in this scenario is that the C++ standard is defined in a very detailed manner.

# C++17 mathematical special functions

We initially rolled custom implementations for some of the mathematical special functions (e.g. `std::hermite`).
This has always many drawbacks.

Thankfully, the discussions about using Boost code inside the libc++ (or more generally llvm project) has concluded that Boost.Math is allowed.
It has already been added into the third-party folder and there was already initial POC work done for this (in 2023 - lets see how much adjustment is needed for today's repo).
Have not really started looking into this yet though.

[Parent issue link](https://github.com/llvm/llvm-project/issues/99939) for all mathematical special functions.
(Note, this is one of the last missing features to be C++17 complete!)