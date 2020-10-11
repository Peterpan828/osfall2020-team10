#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "../osfall2020-team10/include/linux/prinfo.h"

void print_prinfo(struct prinfo p)
{	
	printf("%s,%d,%lld,%d,%d,%d,%lld\n", p.comm, p.pid, p.state,
  	p.parent_pid, p.first_child_pid, p.next_sibling_pid, p.uid);	
}

