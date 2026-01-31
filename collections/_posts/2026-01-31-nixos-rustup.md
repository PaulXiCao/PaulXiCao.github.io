---
title: "NixOS: cargo linker error"
categories:
- linux
- nixos
- rust
---

There are multiple ways to install a rust toolchain under NixOS.
One easy way is to install `gcc, rustup` system-wide, e.g.
```nixos
# file: /etc/nixos/configuration.nix:
environment.systemPackages = with pkgs; [
  rustup
  gcc
];
```

This setup might brake after updating NixOS (and removing older generations) as this will delete files from /nix/store/ and cargo might still use stale paths (e.g. for the linker).

Example error message:
```bash
$ cargo run --bin ex-5-1-simple-mutex-based-channel 
   Compiling rust-atomics-mara-bos v0.1.0 (/home/paul/info/rust/rust-atomics-mara-bos)
error: linking with `cc` failed: exit status: 1
  |
  = note:  "cc" "-m64" "/tmp/rustcnDnLRB/symbols.o" "<50 object files omitted>" "-Wl,--as-needed" "-Wl,-Bstatic" "<sysroot>/lib/rustlib/x86_64-unknown-linux-gnu/lib/{libstd-*,libpanic_unwind-*,libobject-*,libmemchr-*,libaddr2line-*,libgimli-*,librustc_demangle-*,libstd_detect-*,libhashbrown-*,librustc_std_workspace_alloc-*,libminiz_oxide-*,libadler2-*,libunwind-*,libcfg_if-*,liblibc-*,librustc_std_workspace_core-*,liballoc-*,libcore-*,libcompiler_builtins-*}.rlib" "-Wl,-Bdynamic" "-lgcc_s" "-lutil" "-lrt" "-lpthread" "-lm" "-ldl" "-lc" "-L" "/tmp/rustcnDnLRB/raw-dylibs" "-B<sysroot>/lib/rustlib/x86_64-unknown-linux-gnu/bin/gcc-ld" "-fuse-ld=lld" "-Wl,--eh-frame-hdr" "-Wl,-z,noexecstack" "-L" "<sysroot>/lib/rustlib/x86_64-unknown-linux-gnu/lib" "-o" "/home/paul/info/rust/rust-atomics-mara-bos/target/debug/deps/ex_5_1_simple_mutex_based_channel-4e487e50b234f6fd" "-Wl,--gc-sections" "-pie" "-Wl,-z,relro,-z,now" "-nodefaultlibs"
  = note: some arguments are omitted. use `--verbose` to show all linker arguments
  = note: /home/paul/.rustup/toolchains/stable-x86_64-unknown-linux-gnu/lib/rustlib/x86_64-unknown-linux-gnu/bin/gcc-ld/ld.lld: line 5: /nix/store/pwrvyffzg8hzvi4d864q8bmjr7vfaxqz-rustup-1.27.1/nix-support/ld-wrapper.sh: No such file or directory
          collect2: error: ld returned 127 exit status
          
error: could not compile `rust-atomics-mara-bos` (bin "ex-5-1-simple-mutex-based-channel") due to 1 previous error
```

This can be fixed via
```bash
$ rustup toolchain remove stable
$ rustup toolchain install stable
```
