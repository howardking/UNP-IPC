#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>

#define FILE_MODE 0666

int main(int argc, char *argv[])
{
	int c, flags;
	mqd_t mqd;

	flags = O_RDWR | O_CREAT;
	while ((c = getopt(argc, argv, "e")) != -1){
		switch (c){
		case 'e':
			flags |= O_EXCL;
			break;
		}
	}

	if (optind != argc -1){
		printf("usage: mqcreate [-e] <name>\n");
		exit(1);
	}

	if ((mqd = mq_open(argv[optind], flags, FILE_MODE, NULL)) == -1){
		perror("mq_open failed");
		exit(1);
	}
	
	if ((mq_close(mqd)) == -1){
		perror("mq_close failed");
		exit(1);
	}
	exit(0);
}
