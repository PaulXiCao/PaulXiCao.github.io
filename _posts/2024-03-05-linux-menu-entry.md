---
title: "Linux: Add menu entry to desktop environment"
categories:
  - linux
tags:
  - gui
  - desktop
---

One can add custom entries to the program list in graphical desktop environments such as XFCE.
You can either right-click on the menu or add a ".desktop" file with the specific keys.

For user specific applications you can use `$HOME/.local/share/applications/` or system-wide via `/use/share/applications/`.

An example:
```
$ cat $HOME/.local/share/applications/portfolioPerformance.desktop
[Desktop Entry]
Version=1.0
Type=Application
Name=Portfolio Performance
Exec=/opt/portfolio/PortfolioPerformance
Icon=/opt/portfolio/configuration/org.eclipse.osgi/3036/0/.cp/icons/pp_256.png
Categories=Finance
```
