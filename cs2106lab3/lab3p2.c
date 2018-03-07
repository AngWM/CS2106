#include <stdio.h>
#include <pthread.h>

//Global variable.
int ctr = 0;
pthread_t thread;

void *child(void *t)
{
//	ctr++;
	// Print out the parameter passed in, and the current value of ctr.
	printf("I am child %d. Ctr=%d\n", t, ctr);
	// Then increment ctr
	ctr++;
	pthread_exit(NULL);
}

int main()
{
	int i=0;

	// Initialize ctr
	ctr = 0;

	// Create the threads
	//for(i=0; i<20; i++){
		pthread_create(&thread, NULL, child, (int *) 2500000000);
		printf("%d ", (int *) i);
		printf("%d\n", i);
	//}

//	for(i=0; i<20; i++)
		pthread_join(thread, NULL);

	// And print out ctr
	printf("Value of ctr=%d\n", ctr);
	return 0;
}

