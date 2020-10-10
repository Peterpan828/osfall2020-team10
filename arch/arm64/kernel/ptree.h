#ifndef PTREE_H
#define PTREE_H

#include <linux/syscalls.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/init_task.h>
#include <linux/list.h>

struct task_struct * get_init_task(void)
{
	return &init_task;
}


int no_children(struct task_struct* cur_proc)
{

	struct list_head children;
	children = cur_proc -> children;
	return list_empty(&children);
}

int no_sibling(struct task_struct* cur_proc)
{
	struct list_head sibling;
	struct list_head head = cur_proc -> parent -> children;
	sibling = cur_proc -> sibling;
	return list_is_last(&sibling, &head);
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

int get_procs(struct prinfo *kbuf, int *nr)
{
	struct task_struct *cur_proc = get_init_task();
	int total = 0;
	int count = 0;
	struct prinfo new_pr;

	while(cur_proc != NULL)
	{
		total++;

		new_pr = get_prinfo(cur_proc);
		
		if(!no_children(cur_proc))
		{
			cur_proc = list_entry(\
					cur_proc -> children.next, 
					struct task_struct,
					sibling);
			continue;
		}

		while(no_sibling(cur_proc))
		{
			cur_proc = cur_proc -> parent;
			if(cur_proc -> pid == 0) // Came back!!
			{
				cur_proc = NULL;
				continue;
			}
		}


		cur_proc = list_entry(\
				cur_proc -> sibling.next,
				struct task_struct,
				sibling);
			       	
		if(count < *nr)
		{
			count ++;
			kbuf[count] = new_pr;
		}
			
	}	
	*nr = count;
	return total;
}
	

#endif
