#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd[2], n;
	char c;
	pid_t childpid;

	if (pipe(fd) == -1){
		perror("Create pipe failed");
		exit(1);
	}

	if ((childpid = fork()) == 0){
		sleep(3);
		if((n = read(fd[0], &c, 1)) != 1){
			printf("child read return %d\n", n);
			//exit(1);
		}
		printf("child read %c\n", c);
		write(fd[0], "c", 1);
		exit(0);
	}

	write(fd[1], "p", 1);
	if ((n = read(fd[1], &c, 1)) != 1){
		printf("parent read returned %d\n", c);
		//exit(1);
	}
	sleep(4);
	if ((n = read(fd[1], &c, 1)) != 1){
		printf("parent read returned %d\n", c);
		//exit(1);
	}
	printf("parent read %c\n", c);
	exit(0);
}
