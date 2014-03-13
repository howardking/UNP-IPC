#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/param.h>

int main(int argc, char *argv[])
{
	int  fd, i;
	char *ptr;
	size_t filesize, mmapsize, pagesize;

	if (4 != argc){
		printf("usage: test1 <pathname> <filename> <mmapsize>");
		exit(1);
	}
	filesize = atoi(argv[2]);
	mmapsize = atoi(argv[3]);

	fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
	lseek(fd, filesize - 1, SEEK_SET);
	write(fd, "", 1);

	ptr = mmap(NULL, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	pagesize = sysconf(_SC_PAGESIZE);
	printf("PAGESIZE = %ld\n", (long)pagesize);

	for (i = 0; i < filesize > pagesize ? filesize : pagesize; i += pagesize){
		printf("ptr[%d] = %d\n", i, ptr[i]);
		ptr[i] = 1;
		printf("ptr[%d] = %d\n", i + pagesize -1, ptr[i + pagesize -1]);
		ptr[i + pagesize -1] = 1;
	}

	printf("ptr[%d] = %d\n", i, ptr[i]);

	exit(0);
}
