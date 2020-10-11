#ifndef PTREE_H
#define PTREE_H

#include <linux/syscalls.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/init_task.h>
#include <linux/list.h>
#include <uapi/asm-generic/errno-base.h>

struct task_struct * get_init_task(void)
{
	return &init_task;
}


int no_children(struct task_struct* cur_proc)
{

	struct list_head head_children;
	struct task_struct *children;

	head_children = cur_proc -> children;
	if(list_empty(&head_children)) return true;
	children = list_entry(\
			head_children.next,
			struct task_struct,
			sibling);
	if (children -> pid == 0) return true;

	else return false;
}

int no_sibling(struct task_struct* cur_proc)
{
	struct list_head head_sibling;
	struct list_head head = cur_proc -> parent -> children;
	struct task_struct *sibling;
	head_sibling = cur_proc -> sibling;
	if(list_empty(&head_sibling)) return true;
	if(list_is_last(&head_sibling, &head)) return true;
	sibling = list_entry(\
			head_sibling.next,
			struct task_struct,
			sibling);
	if (sibling -> pid == 0) return true;
	else return false;
}

struct prinfo get_prinfo(struct task_struct * cur_proc)
{
	struct prinfo new_pr;
	struct list_head children;
	struct list_head sibling;
	struct task_struct *first_child;
	struct task_struct *next_sibling;
				
	new_pr.state = cur_proc -> state;
	new_pr.pid = cur_proc -> pid;
	new_pr.uid = (cur_proc -> cred) -> uid.val;
	strcpy(new_pr.comm, cur_proc -> comm);
	new_pr.parent_pid = cur_proc -> parent -> pid;
	
       	children = cur_proc -> children;
        sibling	= cur_proc -> sibling;
	
	if (no_children(cur_proc))
	{
		new_pr.first_child_pid = 0;
	}
	else
	{
		first_child = list_entry(\
				cur_proc -> children.next,
				struct task_struct,
				sibling);
		new_pr.first_child_pid = first_child -> pid;
	}

	if (no_sibling(cur_proc))
	{
		new_pr.next_sibling_pid = 0;
	}
	else
	{
		next_sibling = list_entry(\
				cur_proc -> sibling.next,
				struct task_struct,
				sibling);
	new_pr.next_sibling_pid = next_sibling -> pid;
	}
		
	new_pr.state = cur_proc -> state;
	new_pr.pid = cur_proc -> pid;
	new_pr.parent_pid = cur_proc -> parent -> pid;
	return new_pr;
}


struct task_struct *next_proc(struct task_struct *cur_proc)
{
	struct task_struct *new_proc;
	if(!no_children(cur_proc))
	{	
		//printk(KERN_INFO "Has Children!!");
		new_proc = list_entry(\
				(cur_proc->children).next,
				struct task_struct,
				sibling);
		return new_proc;
	}
	
	while(no_sibling(cur_proc))
	{	
		//printk(KERN_INFO "No Sibling");
		cur_proc = cur_proc -> parent;
		if (cur_proc -> pid == 0) return NULL; // Came Back to Base!!
	}

	//printk(KERN_INFO "Next Sibling");
	new_proc = list_entry(\
			(cur_proc->sibling).next,
			struct task_struct,
			sibling);

	if (new_proc -> pid == 0) return NULL;
	
	return new_proc;
}
		

		
int get_procs(struct prinfo *kbuf, int *nr)
{
	struct task_struct *cur_proc = get_init_task();
	int total = 0;
	int count = 0;
	struct prinfo new_pr;

	while(cur_proc != NULL)
	{
		//printk(KERN_INFO "cur_proc: %d", cur_proc -> pid);

		if ((cur_proc -> cred) != NULL)
		{ 
			//printk(KERN_INFO "FILL buf");
			total++;
			new_pr = get_prinfo(cur_proc);
		
			if(count < *nr)
			{
				kbuf[count] = new_pr;
				count ++;
			}

		}
		
		cur_proc = next_proc(cur_proc);

	}	
	*nr = count;
	return total;
}
	

#endif
