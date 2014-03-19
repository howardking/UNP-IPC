#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/msg.h>

int main(void)
{
	int i, msqid;
	
	for (i = 0; i < 100; i++){
		msqid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
		printf("msqid = %d\n", msqid);
		
		msgctl(msqid, IPC_RMID, NULL);
	}
	exit(0);
}
