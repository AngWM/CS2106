#include <stdio.h>
#include <unistd.h>

int main()
{
	int cpid;
	if((cpid=fork()) != 0)
	{
		printf("Hello, I am the parent, my pid is %d, my child's pid is %d, my parent's is %d\n", getpid(), cpid, getppid());
	}
	else
	{
		printf("I am child, my pid is %d, my parent's pid %d, fork returned %d", getpid(), getppid(), cpid);
	}
}

