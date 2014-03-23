#include "fifo.h"
#include "readline.h"

void server(int, int);

int main(int argc, char *argv[])
{
	int readfifo, writefifo, dummyfd, fd;
	char *ptr, buff[MAXLINE + 1], fifoname[MAXLINE];
	pid_t pid;
	size_t n;

	if ((mkfifo(SERV_FIFO, FILE_MODE) < 0) && (errno != EEXIST)){
		perror("Can't create fifo.serv");
		exit(1);
	}
	
	readfifo = open(SERV_FIFO, O_RDONLY, 0);
	dummyfd  = open(SERV_FIFO, O_WRONLY, 0);

	while((n = Readline(readfifo, buff, MAXLINE)) > 0){
		if (buff[n-1] == '\n')
			n--;
		buff[n] = '\0';

		if ((ptr = strchr(buff, ' ')) == NULL){
			printf("bogus request: %s\n", buff);
			continue;
		}
		*ptr++ = 0;
		pid = atol(buff);
		snprintf(fifoname, sizeof(fifoname), "fifo.%ld", (long)pid);
		if ((writefifo = open(fifoname, O_WRONLY, 0)) < 0){
			printf("can't open: %s\n", fifoname);
			continue;
		}

		if ((fd = open(ptr, O_RDONLY)) < 0){
			snprintf(buff + n, sizeof(buff) -n, ": can't open, %s\n", strerror(errno));
		n = strlen(ptr);
		write(writefifo, ptr, n);
		close(writefifo);
		}else {
			while ((n = read(fd, buff, MAXLINE)) > 0){
				write(writefifo, buff, n);
			}
			close(fd);
			close(writefifo);
		}
	}

	exit(0);
}
