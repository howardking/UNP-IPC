#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SVSHM_MODE (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)

int main(int argc, char *argv[])
{
	int id;
	if (2 != argc){
		printf("usage: shmrmid <pathname>\n");
		exit(1);
	}
	
	id = shmget(ftok(argv[1], 0), 0, SVSHM_MODE);
	shmctl(id, IPC_RMID, NULL);

	exit(0);
}
