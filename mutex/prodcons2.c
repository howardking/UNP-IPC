#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAXNITEMS	1000000
#define MAXNTHREADS	100
#define min(x,y) (x)>(y)?(y):(x)
#define max(x,y) (x)>(y)?(x):(y)


int nitems;
struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
} shared = {
	PTHREAD_MUTEX_INITIALIZER
};

void *produce(void *), *consume(void *);

int main(int argc, char *argv[])
{
	int i, nthreads, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;

	if (3 != argc){
		printf("Usage: prodcons2 <#items> <#thread>\n");
		exit(1);
	}

	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);
	
/*	set_concurrency(nthreads);*/
	pthread_setconcurrency(nthreads);
	
	for (i = 0; i < nthreads; i++){
		count[i] = 0;
		pthread_create(&tid_produce[i], NULL, produce, &count[i]);
	}

	for (i = 0; i < nthreads; i++){
		pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);
	}

	pthread_create(&tid_consume, NULL, consume, NULL);
	pthread_join(tid_consume, NULL);

	exit(0);
}

void *produce(void *arg)
{
	for ( ; ; ){
		pthread_mutex_lock(&shared.mutex);
		if (shared.nput >= nitems){
			pthread_mutex_unlock(&shared.mutex);
			return NULL;
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
		pthread_mutex_unlock(&shared.mutex);
		*((int *)arg) += 1;
	}
}

void *consume(void *arg)
{
	int  i;
	for (i = 0; i < nitems; i++){
		if (shared.buff[i] != i){
			printf("buff[%d] = %d\n", i, shared.buff[i]);
		}
	}

	return NULL;
}
