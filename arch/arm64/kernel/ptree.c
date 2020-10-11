#include "ptree.h"

SYSCALL_DEFINE2(ptree, struct prinfo *,buf, int *, nr)
{
	int re_sys;	
	int total;
	int knr;
	int errno;
	struct prinfo *kbuf;
	
	if (buf == NULL || nr == NULL || *nr < 0)
	{
		errno = EINVAL;
		return -errno;
	}
	
	re_sys = copy_from_user(&knr, nr, sizeof(int));
	
	if (re_sys != 0)
	{	
		errno = EFAULT; 
		return -errno;
	}


	kbuf = kcalloc(knr, sizeof(struct prinfo), GFP_KERNEL);
	//printk(KERN_INFO "this syscall is called!\n");
	
	read_lock(&tasklist_lock);
	total = get_procs(kbuf, nr);
	read_unlock(&tasklist_lock);

	re_sys = copy_to_user(buf, kbuf, sizeof(struct prinfo) * *nr);
	if (re_sys != 0)
	{
		errno = EFAULT;
		return -errno;
	}
	kfree(kbuf);

	return total;
}

