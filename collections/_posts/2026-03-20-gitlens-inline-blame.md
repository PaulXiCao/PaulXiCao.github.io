---
title: "GitLens not showing inline blame in VSCode"
categories:
- git
---

GitLens might not show inline blame. 

Test: $ git blame someFile

Possible error: fatal: could not open object name list: .git-blame-ignore-revs

This clearly indicates that the mentioned file is not found. Remove it from .gitconfig. 