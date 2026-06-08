---
title: "Taming the hazard - Part 2 - From prototype to libstdc++"
categories: [cpp, libstdc++]
tags: [c++, c++26, libstdc++, hazard pointer]
---

Previous post in this series: [Part 1 - Introduction]( {% link _posts/2026-05-25-hazard-libstdc++-part1.md %} ).

How hard could it be to port a working prototype to libstdc++? :D Answer: Roughly 10h with extensive AI help...

Just a simple list of tasks that were necessary:
1. Setup repo: Forked the [gcc-mirror](https://github.com/gcc-mirror/gcc) on GitHub.
2. Understand gcc/libstdc++ repo structure: There is a subfolder libstdc++-v3/ that contains all libstdc++ related code.
3. Understand build process
   ```bash
   mkdir build && cd build
   ../libstdc++-v3/configure \
   CXX="ccache g++" \
   LDFLAGS="-fuse-ld=mold" \
   --disable-multilib \
   --enable-libstdcxx-threads \
   --prefix=/tmp/gcc-inst
   make -j$(nproc)
   ```
4. Understanding testing framework, e.g. `make check RUNTESTFLAGS="--target_board=unix/-std=gnu++26 conformance.exp=30_threads/hazard_pointer/*"`
5. Pitfalls while building/testing
   * General feeling: Building takes quite some time and even for small incremental changes. 
   * Major issues arose when I initially built full gcc from scratch. My linux box kept crashing and when the frustration finally hit a threshold I eventually filed a [bug report](https://bugzilla.kernel.org/show_bug.cgi?id=221611).
   * libstdc++ standalone build does not seem to be well supported:
       * libgcc/gthr.h `#includes "gthr-default.h"` which is normally generated during full libgcc build. Chicken-and-egg. Solution: Manually `cp libgcc/gthr-posix.h libgcc/gthr-default.h` is needed once per build dir.
       * The DejaGnu test harness does not pick up new headers. I needed to fix `libstdc++-v3/testsuite/lib/libstdc++.exp`. Will propose it as a separate patch to the mailing list.
6. Actual porting code
   1. Find out where to register new files
   2. Adopting libstdc++ code style (e.g. "uglification" with "__variable_name" and macro usage)
   3. Porting tests from GoogleTest to DejaGnu
7. Writing to mailing list
   1. Squash commits to only a few (each commit must be a separate mail - wtf? reviewers might want to comment on commits independently..)
   2. Produce informative mail text
   3. Setup `git send-email`
   4. Made use of scripts from `contrib/` folder for code style checking and creating changelogs per commit
   5. Send to [maillinglist](https://gcc.gnu.org/pipermail/libstdc++/2026-June/066816.html)

3 commits, 18 test files, 566 LoC header

The general workflow via mailing list does seem quite outdated, but I must say that this has lead to a different methodology for me. 
I usually create merge requests rather quickly in a draft status and then iterate on them until they reach a certain quality.
Here I needed to make sure it is perfect on first try as I cannot easily update it once it has been sent.
This presumably benefits maintainers by filtering out low-quality submissions.

Each step was heavily discussed with AI. Some were manually implemented, others were done by AI.

Upcoming possible blog posts:
* General feedback on interactions via mailinglist
* In-depth discussion of implementation details