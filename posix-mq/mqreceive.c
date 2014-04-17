#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int c, flags;
	mqd_t mqd;
	ssize_t n;
	unsigned int prio;
	void *buff;
	struct mq_attr attr;

	flags = O_RDONLY;
	while((c = getopt(argc, argv, "n")) != -1){
		switch (c){
		case 'n':
			flags |= O_NONBLOCK;
			break;
		}
	}

	if (optind != argc -1){
		printf("Usage: mqreceive [ -n ] <name>");
		exit(1);
	}

	if(-1 == (mqd = mq_open(argv[optind], flags))){
		printf("Can't open MQ %s\n", argv[optind]);
		exit(1);
	}
	
	if (-1 == mq_getattr(mqd, &attr)){
		printf("Get mq_attr failed\n");
		exit(1);
	}

	if (NULL == (buff = malloc(attr.mq_msgsize))){
		printf("Alloc buff failed\n");
		exit(1);
	}

	if (-1 == (n = mq_receive(mqd, buff, attr.mq_msgsize, &prio))){
		printf("mq_receive failed\n");
		exit(1);
	}

	printf("Read %ld bytes, priority = %u\n", (long)n, prio);

	exit(0);
}
