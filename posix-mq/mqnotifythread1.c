#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <pthread.h>

mqd_t mqd;
struct mq_attr attr;
struct sigevent sigev;

static void notify_thread(union sigval);

int main(int argc, char *argv[])
{
	if (2 != argc){
		printf("Usage mqnotifythread1 <name>\n");
		exit(1);
	}
	
	if(-1 == (mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK))){
		perror("mq_open faild");
		exit(1);
	}
	
	if (-1 == mq_getattr(mqd, &attr)){
		perror("mq_getattr failed");
		exit(1);
	}

	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_value.sival_ptr = NULL;
	sigev.sigev_notify_function = notify_thread;
	sigev.sigev_notify_attributes = NULL;

	if (mq_notify(mqd, &sigev) == -1){
		perror("mq_notigy failed");
		exit(1);
	}

	for (;;){
		pause();
	}

	exit(0);
}

static void notify_thread(union sigval arg)
{
	ssize_t n;
	void *buff;

	printf("notify thread started\n");
	if (NULL == (buff = malloc(attr.mq_msgsize))){
		printf("Alloc buff failed\n");
		exit(0);
	}
	if (mq_notify(mqd, &sigev) == -1){
		perror("mq_notigy failed");
		exit(1);
	}

	while((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0) {
		printf("read %ld bytes\n", (long)n);
	}
	
	if (errno != EAGAIN){
		printf("mq_receiver error\n");
		exit(1);
	}

	free(buff);
	pthread_exit(NULL);
}
