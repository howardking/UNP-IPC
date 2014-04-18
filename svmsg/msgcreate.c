#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define SVMSG_MODE 0644

int main(int argc, char *argv[])
{
	int c, oflag, mqid;
	
	oflag = SVMSG_MODE | IPC_CREAT;
	while((c = getopt(argc, argv, "e")) != -1){
		switch (c){
		case 'e':
			oflag |= IPC_EXCL;
			break;
		}
	}

	if (optind != argc - 1){
		printf("Usage: msgcreate [-e] <name>\n");
		exit(1);
	}
	
	mqid = msgget(ftok(argv[optind], 0), oflag);
	if(mqid == -1){
		perror("msgget failed");
		exit(1);
	}

	exit(0);
}
