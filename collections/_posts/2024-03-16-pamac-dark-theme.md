---
title: "Manjaro's pamac in dark mode"
categories:
  - linux
tags:
  - manjaro
  - dark mode
---

Manjaro's pamac (gui alternative to pacman) does not adhere to the global theme set via the "Appearance" menu.
You additionally need to set the `GTK_THEME` variable with the chosen theme, e.g.
```bash
sudo echo "GTK_THEME=adwaita-dark" >> /etc/environment
```

Source: [forum.endeavouros.com](https://forum.endeavouros.com/t/pamac-in-dark-mode-in-xfce/47877)
