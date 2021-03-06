#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SVSHM_MODE (SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)

int main(int argc, char *argv[])
{
	int i, id;
	struct shmid_ds buff;
	unsigned char c, *ptr;
	
	if(2 != argc){
		printf("usage: SVshmread <pathname>\n");
		exit(0);
	}

	id = shmget(ftok(argv[1], 0), 0, SVSHM_MODE);
	ptr = shmat(id, NULL, 0);
	shmctl(id, IPC_STAT, &buff);

	for (i = 0; i < buff.shm_segsz; i++){
		if ((c = *ptr++) != (i % 256)){
			printf("ptr[%d] = %d\n", i, c);
		}
		
		printf("ptr[%d] = %d\n", i, c);
	}
	exit(0);
}
