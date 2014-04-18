#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/msg.h>

#define MAXMSG (8192 + sizeof(long))

int main(int argc, char *argv[])
{
	int mqid;
	ssize_t n;
	struct msgbuf *buff;
	
	if (argc != 2){
		printf("Usage: msgrcvid <mqid>");
		exit(1);
	}

	mqid = atoi(argv[1]);

	buff = malloc(MAXMSG);
	if (NULL == buff){
		printf("Alloc buff Failed\n");
		exit(1);
	}

	if(-1 == (n = msgrcv(mqid, buff, MAXMSG, 0, 0))){
		printf("msgrcv failed\n");
		exit(1);
	}
	printf("read %d bytes, type = %ld\n", n, buff->mtype);

	exit(0);
}
