---
title: 'Source code visualization'
categories:
- programming
tags:
- sourcetrail
- tool
---

Starting to work on a large code base usually involves reading lots of code and building up a mental structure of the code base.
There are tools that can aid this process.

[Sourcetrail](https://github.com/CoatiSoftware/Sourcetrail) is such a tool which is FOSS, but has been abandoned in 2021.
Nonetheless, it still works and is definitely worth trying out!
Supported languages are C, C++, Java, and Python.

## Usage

The tool needs to index your code base.
There is a GUI wizard explaining the process.

The main step is loading the compilation database file `compile_commands.json` which can be produced via cmake:
```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Indexing of the 1200 file code base took ~2:30min on my machine.
Thereafter, one can search and click through the categories, e.g. Files, Macros, Namespaces, Classes.
