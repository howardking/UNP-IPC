#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>

volatile sig_atomic_t mqflag;
static void sig_usr1(int);

int main(int argc, char *argv[])
{
	mqd_t mqd;
	void *buff;
	ssize_t n;
	sigset_t zeromask, newmask, oldmask;
	struct mq_attr attr;
	struct sigevent sigev;

	if (2 != argc){
		printf("Usage: mqnotifysig2 <name>\n");
		exit(1);
	}

	if (-1 == (mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK))){
		printf("mq_open %s failed\n", argv[1]);
		exit(1);
	}

	if (mq_getattr(mqd, &attr)){
		printf("get attr failed\n");
		exit(1);
	}
	
	if (!(buff = malloc(attr.mq_msgsize))){
		printf("Alloc buff failed\n");
		exit(1);
	}

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigemptyset(&oldmask);
	sigaddset(&newmask, SIGUSR1);

	signal(SIGUSR1, sig_usr1);
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	if (-1 == mq_notify(mqd, &sigev)){
		printf("Set mq_notify failed\n");
		exit(1);
	}

	for (;;){
		sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		while(mqflag == 0){
			sigsuspend(&zeromask);
		}
		mqflag = 0;

		if (-1 == mq_notify(mqd, &sigev)){
			printf("Set mq_notify failed\n");
			exit(1);
		}

		while ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)>=0)){
			printf("read %ld bytes\n", (long)n);
		}
		if (errno != EAGAIN){
			printf("mq_receive error\n");
			exit(1);
		}

		sigprocmask(SIG_UNBLOCK, &newmask, NULL);
	}
	exit(0);
}

static void sig_usr1(int signo)
{
	mqflag = 1;
	return;
}
