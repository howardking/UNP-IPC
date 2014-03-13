#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
//#include <fork.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
	int fd1, fd2, *ptr1, *ptr2;
	pid_t childpid;
	struct stat stat;
	
	if (2 != argc){
		printf("usage: test3 <name>\n");
		exit(1);
	}

	shm_unlink(argv[1]);
	fd1 = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0666);
	ftruncate(fd1, sizeof(int));
	fd2 = open("foo3", O_RDONLY);
	fstat(fd2,&stat);
	
	if (0 == (childpid = fork())){
		ptr2 = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd2, 0);
		ptr1 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
		printf("child: shm ptr = %p, foo3 ptr = %p\n", ptr1, ptr2);

		sleep(5);
		printf("shared memory integer = %d\n", *ptr1);
	}

	ptr1 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

	ptr2 = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd2, 0);
	printf("parent: shm ptr = %p, foo3 ptr = %p\n", ptr1, ptr2);

	*ptr1 = 777;

	waitpid(childpid, NULL, 0);

	exit(0);
}
