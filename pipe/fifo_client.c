#include "fifo.h"

int main(int argc, char *argv[])
{
	int readfifo, writefifo;
	size_t len;
	ssize_t n;
	char *ptr, fifoname[MAXLINE], buff[MAXLINE];
	pid_t pid;

	pid = getpid();
	snprintf(fifoname, sizeof(fifoname), "fifo.%ld", (long)pid);
	if ((mkfifo(fifoname, FILE_MODE) < 0) && errno != EEXIST){
		printf("can't create %s\n", fifoname);
		exit(1);
	}
	snprintf(buff, sizeof(buff), "%ld ", (long)pid);
	len = strlen(buff);
	ptr = buff + len;

	fgets(ptr, MAXLINE-len, stdin);
	len = strlen(buff);

	writefifo = open(SERV_FIFO, O_WRONLY, 0);
	write(writefifo, buff, len);
	readfifo = open(fifoname, O_RDONLY, 0);
	while((n = read(readfifo, buff, MAXLINE)) > 0){
		write(STDOUT_FILENO, buff, n);
	}
	close(readfifo);
	unlink(fifoname);
	
	exit(0);
}
