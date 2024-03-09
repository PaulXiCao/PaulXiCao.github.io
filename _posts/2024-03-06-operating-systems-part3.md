---
title: "Operating Systems (Part 3): Multithreading"
categories:
  - it basics
tags:
  - operating system
---

This post is part of a series on _Operating Systems_:
1. [File Systems, System Calls, Multiprocessing]({% post_url 2024-03-04-operating-systems-part1 %})
2. [Signals, and Race Conditions]({% post_url 2024-03-05-operating-systems-part2 %})
3. This post
4. [Networking]({% post_url 2024-03-09-operating-systems-part4 %})

# Threads

Previously we have covered running multiple processes.
_Threads_ are similar to a lightweight version of such a process.
The distinct difference is that threads spawned from one process will share the same memory space.
Important implications are that one can share variables across threads as well as the kernel does not need to create separate copies of the memory as there is for processes (even though COW optimizations are available).

[pthreads](https://www.man7.org/linux/man-pages/man7/pthreads.7.html) is a low-level C library making multithreading possible.
A basic use case revolves around creating a thread via `pthread_create()`, and to wait for a thread to finish via `pthread_join()`.
A simple hello world example:
```c
{% include operating-systems-part3_pthreads.c %}
```

Since C++11 there is threading support built into standard C++ via the [`<thread>`](https://en.cppreference.com/w/cpp/thread) header.
The thread is created by constructing a `std::thread` object and one can wait for it to end via `.join()`.
Another simple hello world example:
```c++
{% include operating-systems-part3_thread.cpp %}
```
Note, `std::thread` should be seen as deprecated since C++20 which introduced `std::jthread`.
Notable differences are that it joins automatically on destruction, and one can use `std::stop_source, std::stop_token` to cancel/stop in certain situations.

# Synchronization Primitives

## Mutex

A _mutual exclusion_ object allows threads to wait for other threads before accessing shared data.
The C++ header `<mutex>` defines multiple different of such objects.
Its usage is quite simple as a thread can only try to call `.lock()` or `.unlock()` on them.
Only one thread at a time will succeed to lock such a mutex.
It is important to note here that the actual shared data object which is protected by a mutex is only by convention.
The compiler does not enforce that the mutex must be locked beforehand.

## Locks

There are common tasks around the usage of mutexes:
- Locking and then shortly after unlocking a mutex again.
- Unlocking just before leaving a function.
Both can be simplified by using locks.
In their constructor they call `.lock()` on the mutex and in their destructor `.unlock()`.

An example showing mutexes and locks:
```c++
{% include operating-systems-part3_mutex_locks.cpp %}
```

## Condition Variables

There is often the need that a thread waits until another one has finished.
Instead of using a spin/busy wait one can make use of `std::condition_variable`.
It allows to wait for a signal (i.e. via `.wait()`) as well as send signals to other (possibly waiting) threads (i.e. via `.notify_all()`).

An example usage can be found on [cppreference.com](https://en.cppreference.com/w/cpp/thread/condition_variable).

## Semaphores

Since C++20 there is support for semaphores within the standard <semaphore> header.
It represents a light-weight version of condition variables where one checks an integral object for a specific condition.
Standard use-case is that no more than `n` threads should do a task in parallel (e.g. open web connections).

See [cppreference.com](https://en.cppreference.com/w/cpp/thread/counting_semaphore) for an example.

# Standard Problem: _Dining Philosophers_

The [_Dining Philosophers_](https://en.wikipedia.org/wiki/Dining_philosophers_problem) is a usual problem in text books highlighting deadlocks in concurrent programs.

![Philosophers think and eat](/assets/images/dining_philosophers.png)

A round table has five philosophers, five plates, and five forks.
The philosophers do 3 rounds of thinking-then-eating each for different durations.
For eating they take the left fork first, and then the right one.
In a naive approach there a deadlocks possible, e.g. every philosopher takes the fork to his left first and then waits indefinitely.
Solutions would need to make use of some synchronization primitives, e.g. mutexes and condition variables or semaphores.
