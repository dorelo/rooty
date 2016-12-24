#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

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

/* Load function print
 * printk() ``The kernel print function, printk(), behaves almost identically  * to the C library printf() function.``
 * Prints to kernel ring buffer
 */
int rooty_init(void) {
/*	list_del_init(&__this_module.list);
	kobject_del(&THIS_MODULE->mkobj.kobj); */ /*Hide from list*/
	printk("rooty: module loaded\n");
	return 0;
}

/* Exit function print */
void rooty_exit(void) {
	printk("rooty: module removed\n");
}
