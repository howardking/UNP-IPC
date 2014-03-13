#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

struct shmstruct {
	int count;
};

sem_t *mutex;

int main(int argc, char *argv[])
{
	int fd;
	struct shmstruct *ptr;

	if (3 != argc){
		printf("usage: server1 <shmname> <semname>\n");
		exit(1);
	}

	shm_unlink(argv[1]);
	fd = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0666);
	ftruncate(fd, sizeof(struct shmstruct));
	ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	sem_unlink(argv[2]);
	mutex = sem_open(argv[2], O_CREAT | O_EXCL, 0666, 1);
	sem_close(mutex);
	
	exit(0);
}
