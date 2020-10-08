#include <linux/syscalls.h>

SYSCALL_DEFINE0(ptree)
{
	printk("Hello World\n");
	printk(KERN_INFO "this syscall iscalled!\n");
	return 0;
}

