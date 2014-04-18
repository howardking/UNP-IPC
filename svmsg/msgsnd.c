#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/msg.h>

int main(int argc, char *argv[])
{
	int mqid;
	size_t len;
	long type;
	struct msgbuf *ptr;

	if (argc != 4){
		printf("Usage: msgsnd <pathname> <#bytes> <type>\n");
		exit(1);
	}

	len = atoi(argv[2]);
	type = atoi(argv[3]);
	mqid = msgget(ftok(argv[1], 0), 0200);
	if (mqid == -1){
		perror("msgget failed");
		exit(1);
	}

	ptr = calloc(sizeof(long) + len, sizeof(char));
	ptr->mtype = type;

	if(-1 == msgsnd(mqid, ptr, len, 0)){
		perror("msgsnd failed");
		exit(1);
	}

	exit(0);
}
