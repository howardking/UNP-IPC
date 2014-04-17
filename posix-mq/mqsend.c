#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <linux/types.h>*/
#include <mqueue.h>

int main(int argc, char *argv[])
{
	mqd_t mqd;
	void  *ptr;
	size_t len;
	unsigned int prio;

	if (4 != argc){
		printf("Usage: masend <name> <#byte> <priority>\n");
		exit(1);
	}
	
	len = atoi(argv[2]);
	prio = atoi(argv[3]);

	if (-1 == (mqd = mq_open(argv[1], O_WRONLY))){
		printf("Can't open mq %s\n", argv[1]);
		exit(1);
	}
	if (NULL == (ptr = calloc(len, sizeof(char)))){
		printf("Calloc buffer failed\n");
		exit(1);
	}

	if (-1 == (mq_send(mqd, ptr, len, prio))){
		printf("mq_send failed\n");
		exit(1);
	}

	exit(0);
}
