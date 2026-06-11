---
title: "Disabling the Startup Sound on a 2011 iMac (iMac12,2) running Arch Linux"
categories: linux apple firmware
tags: [arch-linux, efi, mac, iMac]
---

Lets disable the startup sound on an 2011 iMac running arch linux: 2011 27-inch iMac (iMac12,2).

The sound is controlled directly by Apple firmware via EFI NVRAM variables:

- `SystemAudioVolume`
- `SystemAudioVolumeDB`

These variables persist independently of the operating system and are still respected by the Mac firmware.

On this system, the values were:

```
SystemAudioVolume = 0x4c
SystemAudioVolumeDB = 0xed
```

The key variable is `SystemAudioVolume`, which controls whether the startup sound is audible (`SystemAudioVolumeDB` is for the intensity).

Since the system boots in UEFI mode, the variables can be modified from Linux using `efivar`. 
Apple firmware interprets `0x80` as a mute value for startup audio.

```bash
printf '\x80' > /tmp/SystemAudioVolume.bin
sudo efivar \
  -n 7c436110-ab2a-4bbb-a880-fe41995c9f82-SystemAudioVolume \
  -f /tmp/SystemAudioVolume.bin \
  -w
sudo poweroff
```

A full shutdown is required for the change to take effect. Then wait a few seconds and power the system back on.