#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

int main(int argc, char *argv[])
{
	mqd_t mqd;
	struct mq_attr attr;

	if (2 != argc){
		printf("usage: mqgetattr <name>\n");
		exit(1);
	}

	if ((mqd = mq_open(argv[1], O_RDONLY)) == -1){
		perror("mq_open failed");
		exit(1);
	}

	if ((mq_getattr(mqd, &attr)) == -1){
		perror("mq_getattr failed");
		exit(1);
	}
	
	printf("max #msgs = %ld, max #bytes/msg = %ld, "
		"#currently on queue = %ld\n",
		attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

	if ((mq_close(mqd)) == -1){
		perror("mq_close failed");
		exit(1);
	}
	
	exit(0);
}
