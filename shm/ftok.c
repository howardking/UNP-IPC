#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>

int main(int argc, char *argv[])
{
	struct stat stat;
	
	if (2 != argc){
		printf("usage: ftok <pathname>\n");
		exit(1);
	}

	stat(argv[1], &stat);
	printf("st_dev: %lx, st_ino: %lx, key: %x\n", (u_long) stat.st_dev, (u_long) stat.st_ino, ftok(argv[1], 0x57));

	exit(0);
}
