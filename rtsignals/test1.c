#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h>

#include "signal_rt.h"

static void sig_rt(int, siginfo_t *, void *);

int main(int argc, char *argv[])
{
	int i, j;
	pid_t pid;
	sigset_t newset;
	union sigval val;

	printf("SIGRTMIN = %d, SIGRTMAX = %d\n", (int) SIGRTMIN, (int) SIGRTMAX);
	if ((pid = fork()) == 0){
		sigemptyset(&newset);
		sigaddset(&newset, SIGRTMAX);
		sigaddset(&newset, SIGRTMAX - 1);
		sigaddset(&newset, SIGRTMAX - 2);
		sigprocmask(SIG_BLOCK, &newset, NULL);

		Signal_rt(SIGRTMAX, sig_rt, &newset);
		Signal_rt(SIGRTMAX - 1, sig_rt, &newset);
		Signal_rt(SIGRTMAX - 2, sig_rt, &newset);

		sleep(6);

		sigprocmask(SIG_UNBLOCK, &newset, NULL);
		sleep(3);
		exit(0);
	}

	sleep(3);
	for (i = SIGRTMAX; i >= SIGRTMAX-2; i--){
		for (j = 0; j <= 2; j++){
			val.sival_int = j;
			sigqueue(pid, i, val);
			printf("sent signal %d, val = %d\n", i, j);
		}
	}
	
	exit(0);
}

static void sig_rt(int signo, siginfo_t *info, void *context)
{
	printf("receive signal #%d, code = %d, ival = %d\n", signo, info->si_code, info->si_value.sival_int);
}
