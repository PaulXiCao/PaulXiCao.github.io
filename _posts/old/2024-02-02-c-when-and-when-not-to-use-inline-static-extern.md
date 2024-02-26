---
title: 'C++: When (and when not!) to use inline (static/extern)'
date: 2024-02-02 08:31:19.000000000 +01:00
categories:
- C++
tags:
- linker
---

Were you always intrigued by `inline, static, extern` keywords in C++?
Then look no further.
This [stackoverflow answer](https://stackoverflow.com/questions/1759300/when-should-i-write-the-keyword-inline-for-a-function-method/1759575#1759575) is as short and precise as it can be.

The following is a paraphrasing of the above linked answer:

- Those three keywords are _linkage directives for the linker_ and mostly ignored by the compiler.
- `inline`: The linker will sort out multiple definitions of the same symbol (thus not to break the one-definition rule ODR).
    Especially it is _not_ treated anymore as a hint to the compiler to optimize/inline the function call!
- `static`: This [SO answer](https://stackoverflow.com/a/15235626/10774817) describes it in more detail.
    - Variables: Their lifetime is extended throughout the programs runtime.
    - Class member functions: Can be called without an object.
    - Free-standing functions: Only callable from within that translation unit.
- `extern`: Compiler need not know about the definition of a variable/function.
    Could be defined in a different translation unit or library.
    The linker will sort this out.
