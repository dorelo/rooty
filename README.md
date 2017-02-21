## Disclaimer
This repository contains code I have written for my own education. Most of it can hardly be considered rootkit-worthy, and is just for learning.

It was also a first introduction to linux kernel programming.

### To Do:
1. Rewrite for use with VFS
2. Add remote control functionality

#### Building the kernel module notes
- /lib/modules/($shell uname -r)/build doesn't exist by default, do: apt-get install linux-headers-($uname -r)
- `obj-name` not `Obj-name`
