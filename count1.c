#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>

struct shared{
	sem_t mutex;
	int   count;
}shared;

int main(int argc, char *argv[])
{
	int fd, i, nloop;
	struct shared *ptr;

	if (3 != argc){
		printf("usage: incr3 <pathname> <#loop>\n");
		exit(1);
	}
	
	nloop = atoi(argv[2]);
	fd = open(argv[1], O_RDWR | O_CREAT, 0666);
	write(fd, &shared, sizeof(shared));
	ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	sem_init(&ptr->mutex, 1, 1);

	setbuf(stdout, NULL);
	if (0 == fork()){
		for (i = 0; i < nloop; i++){
			sem_wait(&ptr->mutex);
			printf("child: %d\n", ptr->count ++);
			sem_post(&ptr->mutex);
		}
		exit(0);
	}

	for (i = 0; i < nloop; i++){
		sem_wait(&ptr->mutex);
		printf("parent: %d\n", ptr->count ++);
		sem_post(&ptr->mutex);
	}
	exit(0);
}

