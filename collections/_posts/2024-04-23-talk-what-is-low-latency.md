---
title: 'Talk: What is low latency C++?'
categories:
- C++ Talks
tags:
- c++
- CppNow
- low latency
- real time
- efficient programming
- CPU hazard
- lock-free
- wait-free
---

Talk: _What is Low Latency C++? - Timur Doumler - CppNow 2023_

Resources:
- videos on YouTube:
  [part 1](https://www.youtube.com/watch?v=EzmNeAhWqVs),
  [part 2](https://www.youtube.com/watch?v=5uIsadq-nyk)
- slides:
  [part 1](https://github.com/boostcon/cppnow_presentations_2023/blob/a7823c7a598db50cfcfb481a251d364c0ed1fbfd/cppnow_slides/What_Is_Low_Latency_Cpp_Part1.pdf)
  [part 2](https://github.com/boostcon/cppnow_presentations_2023/blob/a7823c7a598db50cfcfb481a251d364c0ed1fbfd/cppnow_slides/What_Is_Low_Latency_Cpp_Part2.pdf)
- Timur's previous talks on low latency
  - [C++ in the Audio Industry (CppCon 2015)](https://www.youtube.com/watch?v=boPEO2auJj4)
  - [C++ in the Audio Industry, Episode II: Floating Atomics (JUCE Summit 2015)](https://www.youtube.com/watch?v=2vmXy7znEzs)
  - [Want fast C++? Know your hardware! (CppCon 2016)](https://www.youtube.com/watch?v=BP6NxVxDQIs)
  - [Lock-free programming with modern C++ (ACCU 2017)](https://www.youtube.com/watch?v=qdrp6k4rcP4)
  - [Using Locks in Real-Time Audio Processing, Safely (ADC 2020)](https://www.youtube.com/watch?v=zrWYJ6FdOFQ)
  - [Real-time Programming with the C++ Standard Library (CppCon 2021)](https://www.youtube.com/watch?v=Tof5pRedskI)
  - [A Lock-Free Atomic Shared Pointer in Modern Cpp (CppCon 2022)](https://www.youtube.com/watch?v=gTpubZ8N0no)
  - [Thread synchronisation in real-time audio processing with RCU (ADC 2022)](https://www.youtube.com/watch?v=7fKxIZOyBCE)

# Introduction

This is a high-level overview of techniques used in low-latency programming across different industries.
It is in parts a summary of the above linked talks Timur gave.

(I will give mostly provide the notes I took while watching this talk as well as some further links to other mentioned talks.)

# What is this low latency thing about?

- _latency_ (time between question and answer) vs _throughput_ (how much work per unit of time). latency bifurcates into
  - _ultra low latency_: minimize average response time, e.g. HFT
  - _real-time programming_: hard upper bound, optimize for worst case, e.g. audio, gaming
- _efficiency_: definition is to minimize work needed for task. thus, it lies inbetween latency and throughput.
- common terms for low latency (even across industries)
  - _hot path_: one main thread which has hard time constraints. other threads are more relaxed.
  - _real-time_: program needs to produce correct result within a time limit (_deadline_)
- C++ advantages: manual memory management, zero-cost abstractions, many libraries
- C++ techniques: efficient programming, programming for deterministic execution time
- measuring is important and hard
  - [Fedor Pikus: The Art of Writing Efficient Programs (Packt Publishing, 2021)](https://www.packtpub.com/product/the-art-of-writing-efficient-programs/9781800208117)
  - [Optimising a Real-Time Audio Processing Library - Dave Rowland - ADC22](https://www.youtube.com/watch?v=FpymA7NLNDs)
  - profiling, performance analysis, inspect assembly, benchmarking (see collection of tools on hackingcpp.com - see cpp-resources)
  - microbenchmark are tricky: warm up cache, randomize heap, different optimizations

# Efficient programming

- _efficient_: do the minimal required amount of work for given task
- important topics but covered more in depth in other talks
  - knowledge of programming language
    - avoid unnecessary copies, e.g.
    ```cpp
    std::vector<std::string> strings = { /* */ };
    for (auto s : strings) // bad: copies of strings are costly
    for (const auto& s : strings) // good
    ```
    - avoid unnecessary function calls: use `inline`; prefer `std::variant`, CRTP, "deducing this" (C++23) over virtual functions
    - compile time computations: `constexpr`, TMP, lookup tables at compile time
  - knowledge of libraries used
  - knowledge of compiler (optimizer, ABI)
- knowledge of hardware architecture: CPU architecture, cache hierarchy, prefetcher, branch predictor
  - CPU
    - multiple pipelines working in parallel. each pipeline has different stages.
    - _CPU hazards_: interrupting optimal execution of pipelines
      - _branch hazard_: next instruction depends on condition. branch predictor might help.
        - branchless programming: avoiding branch mispredicts
          - [Branchless Programming in C++ - Fedor Pikus (CppCon 2021)](https://www.youtube.com/watch?v=g-WPhYREFjk)
          - examples:
          ```cpp
          bool x = ...;
          bool y = ...;

          // slow: extra branch (x OR y ELSE)
          if (x || y) do_this();
          else do_that();

          // fast: evaluates x, y in one go ((x OR y) ELSE)
          if (bool(x) + bool(y)) do_this();
          else do_that();
          ```
        - ``[likely]`` does not affect branch predictor, but code layout
      - _data hazard_: result from one pipeline is needed for another pipeline.
        - example:
        ```cpp
        // converts c-style string to unsigned int representation
        unsigned atoui(const char* c) {
          unsigned ret = 0;
          while ((*c >= '0') && (*c <= '9'))
            ret = ret * 10 + (*(c++) - '0'); // hazard! iteration depends on previous iteration!
          return ret;
        }
        ```
        - [Writing Fast Code I - Andrei Alexandrescu (code::dive conference 2015)](https://www.youtube.com/watch?v=vrfYLlR8X8k)
      - _hardware hazard_: CPU has properties, e.g. limited adders/shifters.
    - SIMD: operation on multiple objects in parallel
      - usage: auto-vectorization, explicit SIMD libraries
      - SWAR ("SIMD Within A Register"):
  - cache hierarchy
    - access times: register < L1 cache < L2 cache < main memory
    - data cache (accessing memory) vs instruction cache
    - data cache miss: ~100 cycles. need to be avoided!
      - cache-friendly: data locality, cachelines, contiguous traversal
      - use `std::flat_{map,set}` (C++23). container adaptor ontop of `std::vector`
      - cache-friendly algorithms(!) available
    - instruction cache misses: avoid branches, virtual functions
    - keep cache warm
      - data cache: periodically poke data
      - instruction cache: periodically run hot path with dummy input
    - talks:
      - [C++ Algorithmic Complexity, Data Locality, Parallelism, Compiler Optimizations, & Some Concurrency - Avi Lachmish (CppCon 2022)](https://www.youtube.com/watch?v=0iXRRCnurvo)
      - [What Do You Mean by "Cache Friendly"? - Björn Fahller (C++ on Sea 2022)](https://www.youtube.com/watch?v=yyNWKHoDtMs)
      - [Want fast C++? Know your hardware! - Timur Doumler (CppCon 2016)](https://www.youtube.com/watch?v=BP6NxVxDQIs)
      - [Cpu Caches and Why You Care - Scott Meyers (code::dive 2014)](https://www.youtube.com/watch?v=WDIkqP4JbkE)
  - hardware problems
    - CPU throttling: slowdown because of overheating. need to transform code to dissipate heat better.
    - low energy mode because of low activity

# Programming for deterministic execution time

The following must be avoided on the hot path:

- dynamic memory allocation
  - no data types that allocate
  - no algorithms that allocate, e.g. `std::stable_sort`, `std::stable_partition_`, `std::inplace_merge`
  - no data structures that allocate
    - use: `std::{array,pair,tuple,optional,variant}`
    - don't use: `std::{any, function, vector, deque,...}` (type erasure)
    - workaround: custom allocator
      - general-purpose allocators (e.g. tcmalloc, rpmalloc) optimize for average not worst case. thus not useful for low latency
      - preallocate memory (`std::pmr` allocators)
      - custom data types
    - lambdas do not allocate but coroutines might
- block thread
  - problems with mutexes
    - unclear when mutex is released
    - priority inversion: low prio thread holds mutex, high prio thread waits for mutex. thus high prio thread turned into a low prio thread.
  - _atomic_: indivisble, race-free (but might involve mutex)
  - _lock-free_: at least one thread makes progress
    - thus no mutex possible (because thread holding mutex might go to sleep and then no progress)
  - _wait-free_: all threads make progress
    - only use `std::atomic`, `std::atomic_ref` (C++20)
    - no CAS loop on atomic
    - two main scenarios:
      - passing data to/from hot path thread: wait-free queue
        - different flavours possible. most used: single-producer single-consumer via ring buffer
      - sharing data with hot path thread:
        - hot path reads:
          - `try_lock`: tries to read data via locking mutex but fails if mutex already locked. needs fallback strategy
            - example
            ```cpp
            struct Coefficients;
            Coefficients coeffs;
            crill::spin_mutex mtx; // std::mutex involves syscalls. better spinlock with pregressive fallback

            // hot path's thread
            void process(audio_buffer& buffer) {
              if (std::unique_lock lock(mtx, std::try_to_lock); lock.owns_lock())
                process(buffer, coeffs);
              else
                /* fallback strategy, e.g. use values from previous loop */
            }

            // non hot path thread
            void update_coeffs(Coefficients new_coeffs) {
              std::lock_guard lock(mtx); // locking here is ok
              coeffs = new_coeffs;
            }
            ```
          - _spin-on-write_: use atomic unique_ptr and make sure that when one thread changes the pointer another thread is not using the old object
          ```cpp
          std::unique_ptr<Coefficients> storage;
          std::atomic<Coefficients*> coeffs;

          // hot path's thread
          void process(audio_buffer& buffer) {
            auto* current_coeffs = coeffs.exchange(nullptr); // nullptr -> make sure other thread does not invalidate object inbetween
            process(buffer, *current_coeffs);
            coeffs.store(current_coeffs);
          }

          // non hot path thread
          void update_coeffs(Coefficients new_coeffs) {
            auto new_storage = std::make_unique<Coefficients>(new_coeffs);
            for (auto* expected = storage.get();
                  !coeffs.compare_exchange_weak(expected, new_storage.get();
                  expected = storage.get())
              /* spin */;
            storage = std::move(new_storage);
          }
          ```
            - issue: 2(!) std::atomic::exchange for reading

          - _RCU (Read, Copy, Update)_
            - reading: single load
            - writing: copying in new data, deferred reclamation of previous data
            - _deferred reclamation_: "delete later when it's safe".
              3 alternatives to implement: RCU, Hazard pointer, atomic shared ptr
              - [Read, Copy, Update, then what? RCU for non-kernel  programmers - Fedor Pikus (CppCon 2017)](https://www.youtube.com/watch?v=rxQ5K9lo034)
            - atomic shared ptr: lock-free, but lots of CAS loops. slow and not wait-free.
        - hot path writes:
          - _double-buffering_
            - writing always in buffer A
            - reader comes along. atomically swaps buffer (now writer writes into buffer B)
            - reader reads buffer A
            - example shown in "What is Low Latency C++? Part 2" 1:07:45 (taken from different talk)
          - _SeqLock_
            - use atomic counter
            - writer: increment counter, write data, increment counter
            - reader: wait until even counter, read, check if counter still the same
        - simplest solution: put data into `std::atomic` if it fits
- I/O
  - with other threads: use single-producer single-consumer queue
  - with other processes: shared memory
  - with hardware: direct memory access
- exceptions: no exceptions, error codes, std::optional, std::expected
- context switching (user/kernel space): use thread priority
- syscalls: avoid them
- calling into unknown code
- loops without
