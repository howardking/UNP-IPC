#include "fifo.h"

int main(int argc, char *argv[])
{
	if (2 != argc){
		printf("usage: pipeconf <pathname>\n");
		exit(1);
	}

	printf("PIPE_BUF = %ld, OPEN_MAX = %ld\n", pathconf(argv[1], _PC_PIPE_BUF), sysconf(_SC_OPEN_MAX));
	exit(0);
}
