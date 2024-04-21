---
title: 'Talk: The Basics of Profiling'
categories:
- C++ Talks
tags:
- c++
- CppCon
---

Talk: _The Basics of Profiling - Mathieu Ropert - CppCon 2021_

Resources:
- [video on YouTube](https://www.youtube.com/watch?v=dToaepIXW4s)
- [slides](https://docs.google.com/presentation/d/1G7qoNXOMRtxSNcyKAVwKZRHDAJul3qJN3rJSBneWcvU/edit?usp=sharing)

Good talk on profiling.
Differentiate between optimization (aka "Here's how I made things faster") to profiling (aka "Here's how I found what was slow").

Two types of profilers: _sampling_ vs _instrumentation_.

## Sampling

Sampling profiling in a nutshell is: Attach to program, periodically interrupt and record the stack trace.

The runtime per function is a result of statistically averaging.
It works out of the box without any code changes as long as minimal debug information is compiled into the binary.

Example tool: Intel vTune.

## Instrumentation

You add explicit hooks into your code which record metrics at runtime.
This is more accurate (e.g. not affected by inlining).

Recompilation and linking to 3rd party is needed.

Example tool: [Optick](https://github.com/bombomby/optick).
