#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	if (2 != argc){
		printf("usage: shmunlink <name>\n");
		exit(0);
	}

	shm_unlink(argv[1]);

	exit(0);
}
