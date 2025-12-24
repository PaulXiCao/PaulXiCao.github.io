---
title: "Proxmox: Blank login screen"
categories:
- linux
---

**tldr;** The `proxmoxlib.js` file was corrupted and needed to be manually replaced.

There was a weird bug when accessing our proxmox web interface: The page was available via a mobile browser but just blank when accessing it via a desktop browser.

This seems to be a recurring issue as there have been multiple related questions scattered around forums over the last years.

It started with an update using debian's `apt`. (Dont know which exact commands were performed.)

Inspecting with a browser developer console showed a Status 500 error for accessing `/PVE/StdWorkspace.js`.
Standard troubleshooting via clearing cache (ctrl+F5 in FireFox), reinstalling packages (`# apt install --reinstall pve-manager proxmox-widget-toolkit`), and restarting services (`# systemctl restart pveproxy.service`) did not help.

After endless debugging (and countless AI queries) I found that the file `/usr/share/javascript/proxmox-widget-toolkit/proxmoxlib.js` was corrupted. It had two major issues:
1. Around line 600 a closing brace was missing, e.g.
```js
Ext.define('Proxmox.Utils', {
    utilities: {
        yesText: gettext('Yes'),
        ...
        alertResponseFailure: (res) =>
            Ext.Msg.alert(gettext('Error'), res.htmlStatus || res.result.message),
    // }, <---- !!! this was missing
    checked_command: function(orig_cmd) { orig_cmd(); },
    singleton: true,
```
2. Multiple functions were missing, e.g. `override_task_descriptions`.
Giving errors like
```
Uncaught TypeError: Proxmox.Utils.override_task_descriptions is not a function
Uncaught TypeError: Proxmox.Utils.get_health_icon is not a function
```

Explanation from Claude about the connection to `StdWorkspace.js`:
1. The syntax error in proxmoxlib.js prevented Proxmox.Utils from being properly initialized
2. This caused ExtJS to fall back to its dynamic class loader
3. When the page tried to create PVE.StdWorkspace via Ext.create('PVE.StdWorkspace'), ExtJS attempted to dynamically load it from /PVE/StdWorkspace.js
4. This file doesn't exist because modern Proxmox bundles all code in pvemanagerlib.js

The final solution was to replace the corrupted `proxmoxlib.js` manually with the file from the official package repository.
```bash
# Download and extract the package
mkdir /tmp/A
cd /tmp/A
apt-get download proxmox-widget-toolkit
dpkg -x proxmox-widget-toolkit*.deb extracted

# Copy the correct file
cp ./extracted/usr/share/javascript/proxmox-widget-toolkit/proxmoxlib.js /usr/share/javascript/proxmox-widget-toolkit/proxmoxlib.js

# Restart the proxy service
systemctl restart pveproxy.service
```