#include "test_ptree.h"

int main(int argc, char **argv)
{
	int nr;
	int num_procs;
	int tab;
	int count;
	int stack_tab[300]; // enough stack for tab number

	struct prinfo *buf;

	nr = 100;
	tab = 0; // current tab number
	count = 0; // stack_counter
	stack_tab[0] = 0;	// tab_stack
	
	if(argv[1] != NULL) nr = atoi(argv[1]);
	
	printf("Initial nr : %d\n", nr);

	buf = calloc(nr, sizeof(struct prinfo));	
	num_procs = syscall(398, buf, &nr);
	
	if (errno == EINVAL)
	{
		perror("Wrong Argument");
		return 0;
	}
	if (errno == EFAULT)
	{
		perror("Bad Address");
		return 0;
	}
	
	printf("Modified nr / Total # : %d / %d \n",nr, num_procs);

	for (int i= 0; i < nr ;i++)
	{
		for (int j=0; j < tab ; j++)
		{
			printf("\t");
		}

		print_prinfo(buf[i]);
		if(buf[i].first_child_pid != 0)
		{
			if (buf[i].next_sibling_pid !=0)
			{
				count++;
				stack_tab[count] = tab;
			}
			tab++;
		}

		else if(buf[i].next_sibling_pid == 0)
		{
			tab = stack_tab[count];
			count--;
		}
	}

	free(buf);
	return 0;
}

