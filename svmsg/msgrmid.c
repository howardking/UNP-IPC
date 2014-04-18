#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*#include <sys/ipc.h>*/
#include <linux/msg.h>

int main(int argc, char *argv[])
{
	int mqid;
	
	if (argc != 2){
		printf("Usage: msgrmid <pathname>\n");
		exit(1);
	}

	if( -1 == (mqid = msgget(ftok(argv[1], 0), 0))){
		perror("msgget error:");
		exit(1);
	}

	if (-1 == msgctl(mqid, IPC_RMID, NULL)){
		perror("msgctl failed");
		exit(1);
	}
	exit(0);
}
