A simple Linux kernel driver, that doesn't do anything particularly interesting. My first foray into linux kernel programming.

### To Do:
1. Rewrite for use with VFS
2. Add remote control functionality

#### Building the kernel module notes
- /lib/modules/($shell uname -r)/build doesn't exist by default, do: apt-get install linux-headers-($uname -r)
- `obj-name` not `Obj-name`
