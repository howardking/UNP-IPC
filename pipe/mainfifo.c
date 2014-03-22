#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILE_MODE 0666
#define FIFO1 "fifo.1"
#define FIFO2 "fifo.2"

void client(int, int), server(int, int);

int main(int argc, char *argv[])
{
	int readfd, writefd;
	pid_t childpid;

	if ((mkfifo(FIFO1, FILE_MODE) <0) && (errno != EEXIST)){
		perror("can't create fifo.1");
		exit(1);
	}
	if ((mkfifo(FIFO2, FILE_MODE) <0) && (errno != EEXIST)){
		unlink(FIFO1);
		perror("can't create fifo.2");
		exit(1);
	}

	if((childpid = fork()) == 0){
		readfd = open(FIFO1, O_RDONLY, 0);
		writefd = open(FIFO2, O_WRONLY, 0);

		server(readfd, writefd);
		exit(0);
	}

	writefd = open(FIFO1, O_WRONLY, 0);
	readfd = open(FIFO2, O_RDONLY, 0);

	client(readfd, writefd);

	waitpid(childpid);

	close(readfd);
	close(writefd);

	unlink(FIFO1);
	unlink(FIFO2);

	exit(0);
}
