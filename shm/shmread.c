#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int i, fd;
	struct stat stat;
	unsigned char *ptr, c;
	
	if (2 != argc){
		printf("usage: shmread <name>\n");
		exit(0);
	}
	
	fd = shm_open(argv[1], O_RDONLY, 0666);
	fstat(fd, &stat);
	ptr = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);

	for (i = 0; i < stat.st_size; i++){
	//	if ((c = *ptr++) != (i%256)){
			printf("ptr[%d] = %d\n", i, c);
	//	}
	}

	exit(0);
}
