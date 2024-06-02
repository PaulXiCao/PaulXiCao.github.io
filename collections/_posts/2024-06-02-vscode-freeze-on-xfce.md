---
title: 'VSCode freezes on XFCE'
categories:
- linux
tags:
- vscode
---

VSCode might freeze on a freshly installed Manjaro running XFCE whenever one opens a folder containing a git repository.

This is independent of the different flavours of VSCode that are available on ArchLinux systems (i.e. Code - OSS, Visual Studio Code, VSCodium).

The debug log contains this information:
```bash
$ codium --verbose --log debug
...
[55416:0602/221217.144233:INFO:CONSOLE(656)] "%cTRACE color: #888 [mainThreadSecretState] Getting password for vscode.github-authentication extension:  github.auth", source: vscode-file://vscode-app/opt/vscodium-bin/resources/app/out/vs/workbench/workbench.desktop.main.js (656)
...
```

A fix is to turnoff GitHub specific features within the user settings, i.e.
```json
# User/settings.json
...
"github.gitAuthentication": false,
"github.gitProtocol": "ssh",
"github.branchProtection": false
```