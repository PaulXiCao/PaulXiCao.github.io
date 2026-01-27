---
title: "NixOS: Deleting old generations"
categories:
- linux
- nixos
---

NixOS will keep all previous generations (builds) by default.
This can quickly take up signification amount of disk space.
E.g. for my local setup of 46 builds it totaled up to roughly ~136GB (estimated via `$ du -sch /nix/store`).

Executive summary: Removing old generations is a two-step process: 
1. "unreferencing" them
2. calling the garbage collector (removing unreferenced items)

# At boot

Changing the maximal number of generations shown at boot is readily done  by setting:
```nix
# file: configuration.nix
...
# 5 builds
boot.loader.systemd-boot.configurationLimit = 5; 
```

# Unreferencing old generations

To unreference old generations one needs to manually run a command or write a custom script.
NixOS does not provide any out-of-the-box automatization as this is an "you-know-what-you-do" scenario that **irreversibly breaks the rollback feature**.

Sidenote: We talk here about _unreferencing_ and not _deleting_.
NixOS workflow is to unreference files and then deleting happens via a consecutive garbage collector step.

To manually unreference all but the latest 5 generations run:
```bash
$ nix-env -p /nix/var/nix/profiles/system --delete-generations +5
```

## Scripted

This will create a systemd service to remove all but the latest 5 generations and gets triggered once a week:
```nix
# file: configuration.nix
...
# keep only 5 previous generations
systemd = {
    services.prune-nixos-generations = {
        description = "Prune old NixOS system generations";
        serviceConfig = {
            Type = "oneshot";
            ExecStart = ''
            ${pkgs.nix}/bin/nix-env -p /nix/var/nix/profiles/system --delete-generations +5
            '';
        };
    };
    timers.prune-nixos-generations = {
        wantedBy = [ "timers.target" ];
        timerConfig = {
            OnCalendar = "weekly";
            Persistent = true;
        };
    };
};
```

Checking number of generations and starting the service manually:
```bash
$ sudo nix-env -p /nix/var/nix/profiles/system --list-generations
$ sudo systemctl start prune-nixos-generations.service # removes references to older generations
$ sudo nix-collect-garbage # deletes unreferenced files
$ sudo nix-env -p /nix/var/nix/profiles/system --list-generations
```

# Garbage collecting

To fully delete the older generations one needs to invoke the garbage collector.

This is manually possible via `$ sudo nix-collect-garbage` or calling it periodically via
```nix
# file: configuration.nix
...
nix.gc = {
    automatic = true;
    dates = "weekly";
    options = "--delete-older-than 7d";
};
```

# Optional: Deleting home-manager's generations

<wip>