---
title: "Fixing NixOS rebuild warning: The jq Pretty-Printing Trap"
categories: linux nixos debugging
tags: [nixos, perl, jq, gdm, activation]
---

`nixos-rebuild switch` started printing warnings about UID changes it refused to apply:
```
warning: not applying UID change of user 'gdm-greeter-2' (60580 -> 60579) in /etc/passwd
warning: not applying UID change of user 'gdm-greeter-3' (60581 -> 60580) in /etc/passwd
warning: not applying UID change of user 'gdm-greeter-4' (60582 -> 60581) in /etc/passwd
```

GDM had left stale `gdm-greeter-N` entries in `/etc/passwd` and `/var/lib/nixos/uid-map`, including a duplicate UID.
The plan: drop them all, let NixOS recreate them cleanly.
I used `sed` for the passwd files and `jq` to strip the greeter keys from `uid-map`.

The next rebuild failed:
```
activating the configuration...
, or } expected while parsing object/hash, at character offset 2
(before "(end of string)")
at /nix/store/.../update-users-groups.pl line 11.
Activation script snippet 'users' failed (255)
```

The new `uid-map` was valid JSON — but `jq` had pretty-printed it.
NixOS's Perl reader doesn't handle multi-line `uid-map`: it ends up parsing only the first line, which after pretty-printing was just `{`.

Final solution — compact JSON with `jq -c`:
```bash
sudo sed -i '/^gdm-greeter[-0-9]*:/d' /etc/passwd /etc/shadow /etc/group
sudo jq -c 'with_entries(select(.key | startswith("gdm-greeter") | not))' \
    /var/lib/nixos/uid-map | sudo sponge /var/lib/nixos/uid-map
sudo nixos-rebuild switch
```

Pretty-printing is not free.
