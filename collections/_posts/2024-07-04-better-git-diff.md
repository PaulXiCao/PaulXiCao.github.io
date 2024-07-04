---
title: 'Better git diff'
categories:
- linux
tags:
- git
---

The output of git calls, e.g. `git diff`, can be improved.
Instead of only showing changed lines it can highlight the parts of a line that actually changed.
See [this](collections/_posts/2024-06-02-vscode-freeze-on-xfce.md) thorough post.

To use the `diff-highlight` program (already bundled with `git`!?) do the following:
1. add the program to be available from your $PATH, e.g. `ln -s ~/.local/bin/diff-highlight /usr/share/git/diff-highlight/diff-highlight`
2. edit your `.gitconfig`:
```
$ vim ~/.gitconfig
# ...
[pager]
  log  = diff-highlight | less
  show = diff-highlight | less
  diff = diff-highlight | less
```