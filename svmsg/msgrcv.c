#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/msg.h>

#define MAXMSG (8192 + sizeof(long))

int main(int argc, char *argv[])
{
	int c, flag, mqid;
	long type;
	ssize_t n;
	struct msgbuf *buff;

	type = flag = 0;
	while ((c = getopt(argc, argv, "nt:")) != -1){
		switch(c){
		case 'n':
			flag |= IPC_NOWAIT;
			break;
		case 't':
			type = atol(optarg);
			break;
		}
	}

	if (optind != argc -1){
		printf("Usage: msgrcv [-n] [-t type] <pathname>\n");
		exit(1);
	}

	if(-1 == (mqid = msgget(ftok(argv[optind], 0), 0400))){
		perror("msgget failed");
		exit(1);
	}
	if (NULL == (buff = malloc(MAXMSG))){
		printf("Alloc buff failed\n");
		exit(1);
	}

	n = msgrcv(mqid, buff, MAXMSG, type, flag);
	if (n == -1){
		perror("msgrcv error");
		exit(1);
	}

	printf("read %d bytes, type = %ld\n", n, buff->mtype);
	exit(0);
}
