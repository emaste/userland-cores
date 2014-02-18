#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct thread_info
{
	int thread_num;
	void *stack_var_addr;
	void *fn_addr;

};

void *
thread_fn_1(void *arg)
{
	struct thread_info *ti = arg;
	int stack_var;

	stack_var = ti->thread_num;
	ti->stack_var_addr = &stack_var;
	ti->fn_addr = thread_fn_1;
	sleep(60);
	return(NULL);
}

void *
thread_fn_2(void *arg)
{
	struct thread_info *ti = arg;
	float stack_var;

	stack_var = ti->thread_num;
	ti->stack_var_addr = &stack_var;
	ti->fn_addr = thread_fn_2;
	usleep(100000);
	abort();
}

#define NUM_THREADS 3
struct thread_info info[NUM_THREADS];
pthread_t thread[NUM_THREADS];

int
main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < NUM_THREADS; i++) {
		info[i].thread_num = i;
		pthread_create(&thread[i], NULL,
		    i < NUM_THREADS - 1 ? thread_fn_1 : thread_fn_2, &info[i]);
	}
	sleep(60);
	return(0);
}
