#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SVSHM_MODE (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)

int main(int argc, char *argv[])
{
	int c, id, oflag;
	char *ptr;
	size_t length;

	oflag = SVSHM_MODE | IPC_CREAT;
	while(-1 != (c = getopt(argc, argv, "e"))){
		switch(c){
		case 'e':
			oflag |= IPC_EXCL;
			break;
		}
	}

	if (optind != argc -2){
		printf("usage: shmget [-e] <pathname> <length>\n");
		exit(1);
	}

	length = atoi(argv[optind + 1]);

	id = shmget(ftok(argv[optind], 0), length, oflag);
	ptr = shmat(id, NULL, 0);
	printf("shmget at address = %p\n", ptr);

	exit(0);
}
