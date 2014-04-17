#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <signal.h>

mqd_t mqd;
void *buff;
struct mq_attr attr;
struct sigevent sigev;

static void sig_usr1(int);

int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("Usage: mqnotifysig1 <name>\n");
		exit(1);
	}

	if(-1 == (mqd = mq_open(argv[1], O_RDONLY))){
		printf("Can't open mq %s\n", argv[1]);
		exit(1);
	}

	if (-1 == mq_getattr(mqd, &attr)){
		printf("Get mq_attr failed\n");
		exit(1);
	}

	if (NULL == (buff = malloc(attr.mq_msgsize))){
		printf("Alloc buff failed\n");
		exit(1);
	}
	
	signal(SIGUSR1, sig_usr1);
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	if (-1 == mq_notify(mqd, &sigev)){
		printf("mq_notify failed\n");
		exit(1);
	}

	for (;;){
		pause();
	}

	exit(0);
}

static void sig_usr1(int signo)
{
	ssize_t n;
	n = mq_receive(mqd, buff, attr.mq_msgsize, NULL);
	if (n != -1){
		printf("SIGUSR1 received, read %ld bytes\n", (long)n);
	}else{
		printf("mq_receive error in sig_usr1\n");
	}

	return;
}


