#### Building the kernel module
/lib/modules/($shell uname -r)/build doesn't exist by default, do:
apt-get install linux-headers
ln -s /usr/src/linux-headers-X.X.XX/ /lib/modules/($shell uname -r)/build
Then make will work (hopefully)
Update: actually do apt-get install linux-headers-($uname -r)
Note bizarre non-functionality when you open Makefile with "Obj-m := rooty.c" instead of "obj-m ..." (not LC)
