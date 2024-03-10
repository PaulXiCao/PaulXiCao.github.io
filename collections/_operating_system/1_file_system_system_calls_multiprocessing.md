---
title: "OS: File Systems, System Calls, Multiprocessing (I)"
date: 2024-03-04
lesson: 1
layout: posts
categories:
  - it basics
tags:
  - operating system
---

Unix operating systems are ubiquitous in many industries.
A broad overview will be given in the upcoming blog post series on _Operating Systems_.
They are based on the [Standford CS110](https://youtube.com/playlist?list=PLai-xIlqf4JmTNR9aPCwIAOySs1GOm8sQ&si=5yMCDFm-_ED82Yae) lecture available on YouTube.
If you are more interested in a book on the subject you might like this classic one: [_Operating System Concepts_ on amazon.com](https://www.amazon.com/Operating-System-Concepts-Abraham-Silberschatz/dp/1118112733).

Post series on _Operating Systems_:
1. This post
2. [Signals, and Race Conditions](2024-03-05-operating-systems-part2)
3. [Multithreading](2024-03-06-operating-systems-part3)
4. [Networking](2024-03-09-operating-systems-part4)

# File Systems

The lowest available API to interact with the file system is given by a C API, e.g.
```c
int open(const char *pathname, int flags, /* mode_t mode */ );
int close(int fd);
ssize_t read(int fd, void buf[.count], size_t count);
ssize_t write(int fd, const void buf[.count], size_t count);
```
See man pages for their specification, e.g. [`man 2 open`](https://www.man7.org/linux/man-pages/man2/open.2.html).

Note, the C API forces you to actually check how many bytes were read/written as it might happen in smaller chunks than initially requested.
This is one of the major differences to using the C++ equivalents `FILE` objects or iostreams.

RAM is _byte-addressable_ which means that the lowest denominator one can read/write is a full byte (instead of single bits, e.g. for booleans).
Hard disks have a similar feature called _sector-addressable_, i.e. the smallest unit one can read/write is a sector which is in the order of 512 bytes for older disks (more for newer ones).
This also applies to SSDs.
Reading/writing a specific byte from the disk needs to account for offsetting w.r.t. a sector.

# System Calls

Assembler code to invoke a usual function (e.g. functions compiled into object files, or loaded shared libraries) is the `call` command.

The file system is not accessible for a standard user but the operating system supplies a way to call functions in a superuser mode.
This is called invoking a `syscall` which calls functions using a separate superuser stack as well as interrupting the current user program.

The above mentioned `open(), close()` functions are examples of those `syscall`s.
In code they look like normal function calls with the distinction that one cannot step into them with a debugger.

# Multiprocessing

Programs get their own process at startup.
The kernel shares computation time via _scheduling_ between processes to imitate programs running in parallel.

A process can start another process which runs the same code from that line onwards via the syscall `fork()`.
Initially they inhibit the same memory but it is not shared.
Rather a _copy-on-write_ functionality is employed (i.e. if a process changes one of its values a copy of the original value is written to the other process's memory).

To execute a different program from the currently running one the family of _exec_ functions can be employed.
Important to note is that on success it will _not_ return to the call site!

Examples for this C API:
```c
pid_t fork(void);
pid_t waitpid(pid_t pid, int *_Nullable wstatus, int options);
pid_t getpid(void);
int execvp(const char *file, char *const argv[]);
```
Documentation is available via man pages again, e.g. [`fork()`](https://www.man7.org/linux/man-pages/man2/fork.2.html), [exec family](https://www.man7.org/linux/man-pages/man3/exec.3.html).

