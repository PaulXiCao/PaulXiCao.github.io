---
title: "Connecting to synology per ssh"
categories:
- linux
- ssh
---

Synology run deprecated versions of sshd. 
The cbc cipher needs to be explicitly allowed.

```bash
ssh -c aes256-cbc -o PubkeyAuthentication=no <USER>@<MACHINE>
ssh-copy-id -o "Ciphers=aes256-cbc" -o "PubkeyAuthentication=no" <USER>@<MACHINE>
```