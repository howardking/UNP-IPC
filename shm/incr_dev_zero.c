#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	int fd, i, nloop;
	int *ptr;
	if (2 != argc){
		printf("usage: incr_dev_zero <#loop>");
		exit(1);
	}
	nloop = atoi(argv[2]);
	
	fd = open("/dev/zero", O_RDWR);
	ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
}
