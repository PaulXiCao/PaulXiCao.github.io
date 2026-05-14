---
title: "clangd in VSCode/Codium on NixOS: fixing missing standard headers"
categories:
- C++
tags:
- nixos
- clangd
- vscode
---

If you use the clangd VSCode extension on NixOS, you've probably seen this in the clangd log:

```txt
IncludeCleaner: Failed to get an entry for resolved path '' from include <array> : No such file or directory
```

And red underlines on every `#include <vector>`, `std::atomic`, etc.

## Why it happens

NixOS does not follow the FHS. The nix `clang-wrapper` is a shell script that injects C++ stdlib include paths (`-isystem /nix/store/.../include/c++/14`) via environment variables at invocation time. These paths never appear in `compile_commands.json`.

When clangd runs a compilation it invokes the wrapper, which finds the headers fine. But clangd's **IncludeCleaner** resolves include paths independently -- without running the compiler -- so it sees no stdlib paths and reports empty resolved paths.

The fix is `--query-driver`, which tells clangd to interrogate the compiler binary directly (`-v -E`) to extract its system include search paths. With those paths known, IncludeCleaner can resolve `<array>` to the correct nix store path.

## Why `clangd.arguments` in `.vscode/settings.json` doesn't help

The clangd VSCode extension sometimes fails to pass workspace `clangd.arguments` to the clangd process on startup. Confirming this is easy: check `argv[0]` in the clangd log output -- if only one `argv` entry appears, no extra flags were passed.

## The fix: wrap clangd in `shell.nix`

Create a `clangd` wrapper that bakes `--query-driver` in, and put it first in `PATH` so Codium finds it before the unwrapped binary from `clang-tools`.

```nix
# file: shell.nix
{ pkgs ? import <nixpkgs> {} }:

let
  clangd-wrapped = pkgs.writeShellScriptBin "clangd" ''
    exec ${pkgs.clang-tools}/bin/clangd \
      --query-driver="/nix/store/*/bin/clang++,/nix/store/*/bin/g++" \
      "$@"
  '';
in

pkgs.mkShell {
  packages = with pkgs; [
    clangd-wrapped  # must be first: shadows clang-tools' clangd in PATH
    gcc
    clang
    clang-tools  # clang-format, clang-tidy
    cmake
    ninja
    git
    curl
  ];

  shellHook = ''
    export CC=clang
    export CXX=clang++
  '';
}
```

The glob `"/nix/store/*/bin/clang++"` is passed literally to clangd (double quotes suppress shell glob expansion). clangd expands it itself when matching the compiler path from `compile_commands.json`.

Also add a `.clangd` at the project root so clangd finds `compile_commands.json` in the `build/` directory CMake generates it to:

```yaml
# file: .clangd
CompileFlags:
  CompilationDatabase: build

Diagnostics:
  UnusedIncludes: None
  MissingIncludes: None
```

## Usage

Launch Codium from within a nix-shell:

```bash
nix-shell
codium .
```

Codium inherits the nix-shell `PATH` containing the wrapper. The clangd extension picks up `clangd` from `PATH`, runs the wrapper, and the wrapper passes `--query-driver` to real clangd.

Verify it worked: the clangd log should show `argv[1]: --query-driver=...` and the IncludeCleaner errors should be gone.
