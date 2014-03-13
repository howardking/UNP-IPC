#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	int i, fd;
	struct stat stat;
	unsigned char *ptr;

	if (2 != argc){
		printf("usage: shmwrite <name>\n");
		exit(1);
	}

	fd = shm_open(argv[1], O_RDWR, 0666);
	fstat(fd, &stat);
	ptr = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	for (i = 0; i < stat.st_size; i++){
		*ptr ++ = i % 256;
		printf("stat.st_size = %d\n", stat.st_size);
	}

	exit(0);
}
