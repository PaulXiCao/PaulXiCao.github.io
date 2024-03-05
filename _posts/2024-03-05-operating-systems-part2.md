---
title: "Operating Systems (Part 2): Signals, and Race Conditions"
categories:
  - it basics
tags:
  - operating system
---

This post is part of a series on _Operating Systems_:
1. [File Systems, System Calls, Multiprocessing]({% post_url 2024-03-04-operating-systems-part1 %})
2. This post

# Inter-process communication

## Pipes

Sending arbitrary data between two processes is possible via the [`pipe()`](https://www.man7.org/linux/man-pages/man2/pipe.2.html) command.
See the following "Hello World type" example which sends a message from the parent to its child process:
```c
{% include operating-systems-part2_pipe.c %}
```

For documentation on `pipe` see the manual page, e.g. [linux.die.net](https://linux.die.net/man/2/pipe).

## Signals

Signals are like the light version of pipes which only allows sending small info (`short`) between processes.
This is done on kernel side.

Sending a signal is done with the following commands: [`kill(pid, sig)`](https://linux.die.net/man/3/kill) which sends signals to any process, or an alias to send to itself via [`raise(sig)`](https://linux.die.net/man/3/raise).

To react to a specific signal one can specify a custom signal handler.
This is done as follows:
```c
void reactToSIGCHLD() {...}

int main() {
  signal(SIGCHLD, reactToSIGCHLD);
  // ...
}
```
Once the specific process receives that signal it will halt its current execution and run the specified handler instead.
Note that `signal(sig, handler)` has been deprecated in-favour of `sigaction()`.

There is often a need to not interrupt instantaneously but rather at predefined places.
This can be achieved by using `sigprocmask()`.
Example code which defines a block that will not be interrupted by `SIGCHLD`:
```c
sigset_t set;
sigsetempty(&set);
sigaddset(&set, SIGCHLD);
sigprocmask(SIG_BLOCK, &set, NULL);
{
  // this block will not be interrupted by SIGCHLD signals
}
sigprocmask(SIG_UNBLOCK, &set, NULL);
```

Full documentation around signals can be found [here](https://linux.die.net/man/7/signal).


# Race conditions

_Race condition_ is a general term.
Think of two processes changing a specific shared state simultaneously and the order is not well-defined.

A simple but inefficient way to specify an order is to make one process actively wait for a specific condition.
This is usually called _spin wait_ and looks similar to this in code:
```c
while (cond) {
  ;
}
changeSharedState();
```
A better approach is to insert a call to `pause()`.
This will make the current process sleep until an incoming signal was caught.
Note that this introduces the possibility for a _dead lock_ where multiple processes wait for each other and no forward progress is made.

There is also the frequent need to simultaneously change a signal mask (e.g. unblock a signal) and additionally pause.
A call to `sigsuspend()` implements this in an atomic way (no race condition in-between possible).
