#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_NIDS 4096
#define SVSHM_MODE (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)

int main(int argc, char *argv[])
{
	int i, j, shmid[MAX_NIDS];
	void *addr[MAX_NIDS];
	unsigned long size;

	for (i = 0; i <= MAX_NIDS; i++){
		shmid[i] = shmget(IPC_PRIVATE, 1024, SVSHM_MODE | IPC_CREAT);
		if (-1 == shmid[i]){
			printf("%d identifiers open at once\n", i);
			break;
		}
	}

	for (j = 0; j < i; j++){
		shmctl(shmid[j], IPC_RMID, NULL);
	}

	for (i = 0; i <= MAX_NIDS; i++){
		shmid[i] = shmget(IPC_PRIVATE, 1024, SVSHM_MODE | IPC_CREAT);
		addr[i] = shmat(shmid[i], NULL, 0);
		if ((void *)-1 == addr[i]){
			printf("%d shared memory segments attached at once\n", i);
			shmctl(shmid[i], IPC_RMID, NULL);
			break;
		}
	}

	for (size = 1; ; size++){
		shmid[0] = shmget(IPC_PRIVATE, size, SVSHM_MODE | IPC_CREAT);
		if (-1 != shmid[0]){
			printf("minimum size of shared memory segment = %lu\n", size);
			shmctl(shmid[0], IPC_RMID, NULL);
			break;
		}
	}

	for (size = 65536; ; size += 4096){
		shmid[0] = shmget(IPC_PRIVATE, size, SVSHM_MODE | IPC_CREAT);
		if (-1 == shmid[0]){
			printf("maximum size of shared memory segment = %lu\n", size-4096);
			break;
		}
		shmctl(shmid[0], IPC_RMID, NULL);
	}
	exit(0);
}
