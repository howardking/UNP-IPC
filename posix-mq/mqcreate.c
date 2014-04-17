#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <mqueue.h>

struct mq_attr attr;

int main(int argc, char *argv[])
{
	int c, flags;
	mqd_t mqd;
	
	flags = O_RDWR | O_CREAT;
	while ((c = getopt(argc, argv, "em:z:")) != -1){
		switch (c){
		case 'e':
			flags |= O_EXCL;
			break;
		case 'm':
			attr.mq_maxmsg = atol(optarg);
			break;
		case 'z':
			attr.mq_msgsize = atol(optarg);
			break;
		}
	}

	if (optind != argc -1){
		printf("usage: mqcreate [-e] [-m maxmsg -z msgsize] <name>\n");
		exit(1);
	}

	if ((attr.mq_maxmsg != 0 && attr.mq_msgsize == 0) || 
	    (attr.mq_maxmsg == 0 && attr.mq_msgsize != 0)){
		printf("must specify both -m maxmsg and -z msgsize\n");
		exit(1);
	}

	if ((mqd = mq_open(argv[optind], flags, 0666, (attr.mq_maxmsg !=0)? &attr : NULL)) == -1){
		perror("mq_open failed");
		exit(1);
	}

	if ((mq_close(mqd)) == -1){
		perror("mq_close failed");
		exit(1);
	}

	exit(0);
}
