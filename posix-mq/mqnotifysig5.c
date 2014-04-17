#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>

int pipefd[2];
static void sig_usr1(int);

int main(int argc, char *argv[])
{
	int nfds;
	char c;
	fd_set rset;
	mqd_t mqd;
	void *buff;
	ssize_t n;
	struct mq_attr attr;
	struct sigevent sigev;

	if (2 != argc){
		printf("Usage: mqnotifysig5 <name>\n");
		exit(1);
	}

	if (-1 == (mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK))){
		printf("mq_open failed\n");
		exit(1);
	}

	if (-1 == mq_getattr(mqd, &attr)){
		perror("Failed to get mq_attr");
		exit(1);
	}
	if (NULL == (buff = malloc(attr.mq_msgsize))){
		printf("Alloc buff failed\n");
		exit(1);
	}

	if (pipe(pipefd) !=0 ){
		printf("Initialize pipe failed\n");
		exit(1);
	}

	signal(SIGUSR1, sig_usr1);
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	if(-1 == mq_notify(mqd, &sigev)){
		printf("mq_notify failed\n");
		exit(1);
	}

	FD_ZERO(&rset);

	for (;;){
		FD_SET(pipefd[0], &rset);
		nfds = select(pipefd[0] + 1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(pipefd[0], &rset)){
			read(pipefd[0], &c, 1);
			if(-1 == mq_notify(mqd, &sigev)){
				printf("mq_notify failed\n");
				exit(1);
			}
			while((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0){
				printf("read %ld bytes\n", (long) n);
			}

			if (errno != EAGAIN){
				printf("mq_receive error\n");
				exit(1);
			}
		}
	}

	exit(0);
}

static void sig_usr1(int signo)
{
	if(write(pipefd[1], "", 1) == -1){
		printf("write text failed\n");
		exit(1);
	}

	return;
}
