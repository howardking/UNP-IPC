#include "fifo.h"

void server(int, int);

int main(int argc, char *argv)
{
	int readfd, writefd;

	if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST)){
		perror("can't create fifo.1");
		exit(1);
	}
	if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST)){
		unlink(FIFO1);
		perror("can't create fifo.2");
		exit(1);
	}

	readfd = open(FIFO1, O_RDONLY);
	writefd = open(FIFO2, O_WRONLY);

	server(readfd, writefd);
	exit(0);
}
