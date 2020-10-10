#include "ptree.h"

SYSCALL_DEFINE2(ptree, struct prinfo *,buf, int *, nr)
{
	int sc;	
	struct prinfo *kbuf;
	kbuf = kcalloc(*nr, sizeof(struct prinfo), GFP_KERNEL);
	printk(KERN_INFO "this syscall is called!\n");
	
	read_lock(&tasklist_lock);
	sc = get_procs(kbuf, nr);
	read_unlock(&tasklist_lock);

	sc = copy_to_user(buf, kbuf, sizeof(struct prinfo) * *nr);
	kfree(kbuf);

	return 0;
}

