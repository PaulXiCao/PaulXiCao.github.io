---
title: "Taming the hazard: Implementing C++26 hazard pointer for libstdc++ - Part 1"
categories: [cpp, libstdc++]
tags: [c++, c++26, libstdc++, hazard pointer]
---

This will be a longer blog post series on implementing C++26 [`std::hazard_pointer`](https://en.cppreference.com/cpp/header/hazard_pointer) for libstdc++.

## What the hazard?

In multithreaded context it is a hard problem to figure out when it is safe to reclaim (e.g. delete) an object shared across threads.
Hazard pointers are one advanced technique that solve this. 
It has its own advantages and drawbacks compared to others (e.g. atomic shared pointers, RCU).

Think of a `std::hazard_pointer` as a lock/guard/RAII-type that can be used to protect a shared `std::atomic<T*>` object.
The protection will yield a raw `T*` that can be accessed without dangers of the underlying object being reclaimed.
Once the `std::hazard_pointer` goes out-of-scope (or the protection is manually reset) it is no longer safe to access the raw `T*`.

The underlying object should not be directly deleted but rather "retired".
This will mark the object and on next cleanup it will be automatically deleted as long as there are no more outstanding protections via hazard pointers.
Cleanups are automatic and an implementation detail, e.g. once a thread local retire list has exceeded a threshold or a thread is joined.

A detailed explanation can be found at [Wikipedia/Hazard_pointer](https://en.wikipedia.org/wiki/Hazard_pointer).

## Example

Reader and writer threads concurrently access a shared atomic pointer; hazard pointer ensures the reader never accesses a reclaimed object.

```cpp
#include <atomic>
#include <hazard_pointer> // <-- NEW
#include <thread>

struct Config : std::hazard_pointer_obj_base<Config> {
    int value;
    explicit Config(int v) : value(v) {}
};

std::atomic<Config*> g_cfg{new Config(1)};

void reader() {
    std::hazard_pointer hp = std::make_hazard_pointer();
    Config* p = hp.protect(g_cfg);  // atomic load + protect: object won't be reclaimed
    (void)p->value;                 // safe: hp prevents reclamation of p
}                                   // hp dtor: protection released

void writer() {
    Config* old = g_cfg.exchange(new Config(2));
    old->retire();  // deferred: reclaimed once retire threshold exceeded or thread exits
}

int main() {
    {
        std::jthread _(reader);
        std::jthread _(writer);
    }
    delete g_cfg.load();  // current node was never retired, delete directly
}
```

## Prototype implementation

During the last weeks I have been working on implementing a proper prototype.
This now includes the full API for the standard, e.g. `std::hazard_pointer`, `std::hazard_pointer_obj_base`, `std::make_hazard_pointer`.

The public repo can be found here: https://github.com/PaulXiCao/hazard_pointer_prototype
It includes
- CMake build files
- hazard_ptr.hpp: single file, header-only implementation
- extensive test suite under tests/
- ci jobs: clang-format, clang-tidy, gcc/clang/msvc build matrix, TSan/ASan sanitizer 
- documentation: README.md and code comments

## libstdc++ integration

The goal is to integrate this into gcc's libstdc++.
There is still some implementation work needed (see PORTING_NOTES.md in repo), but I have already started a RFC thread on their mailing list: https://gcc.gnu.org/pipermail/libstdc++/2026-May/066648.html
I am quite curious about the feedback.
Let's see how long and extensive those discussions will become!

## gcc ICE

While implementing this I came across an ICE in gcc.
I have done the honorable act and filed a bug report :)
See this blog post: {% link _posts/2026-05-24-gcc-ICE-contracts.md %} .

## AI experience

Thanks to AI usage this implementation has been more straightforward than initially anticipated.
I will give a short list of things that went well as well as pointing out deficiencies:
- good
  - initial discussions about hazard pointers and comparisons to other methods
  - general code review and best practices
  - implementing test suites
  - commenting code and keeping them up-to-date
- bad
  - lacking deeper understanding of multithreaded computing
  - hallucinating memory ordering issues when discussing TSan reports
  - long waiting phases (>40min) for detailed discussions

## References/link collection

I have looked at and read most of the following official resources to some degree:
- the paper for adding hazard pointers to the std: http://wg21.link/p2530
- the original reference paper: https://www.cs.otago.ac.nz/cosc440/readings/hazard-pointers.pdf
- cppreference: https://en.cppreference.com/cpp/header/hazard_pointer
- the C++26 std section about hazard pointers: https://eel.is/c++draft/saferecl.hp#hazard.pointer.syn
- the reference implementation from Meta's folly: https://github.com/facebook/folly/blob/main/folly/synchronization/Hazptr.h
- Wikipedia: https://en.wikipedia.org/wiki/Hazard_pointer

Personal links:
- prototype repo: https://github.com/PaulXiCao/hazard_pointer_prototype
- RFC on libstdc++ mailing list: https://gcc.gnu.org/pipermail/libstdc++/2026-May/066648.html
- gcc ICE blog post: {% link _posts/2026-05-24-gcc-ICE-contracts.md %}

## Coming next

The next posts in this series will mostly touch on in-depth explanation of implementation details.