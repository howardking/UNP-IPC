#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

int main(int argc, char *argv[])
{
	if (2 != argc){
		printf("usage: mqunlink <name>\n");
		exit(1);
	}

	if (mq_unlink(argv[1]) == -1){
		perror("mq_unlink failed");
		exit(1);
	}

	exit(0);
}
