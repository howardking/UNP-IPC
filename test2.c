/*当映射的内存部分大于文件初始大小时，可以动态的增加已经映射的文件的大小，防止SIGBUS错误*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILENAME "test.data"
#define SIZE 32768

int main(int argc, char *argv[])
{
	int fd, i;
	char *ptr;
	
	fd = open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
	ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	for (i = 4096; i <= SIZE; i += 4096){
		printf("setting file size to %d\n", i);
		ftruncate(fd, i);
		printf("ptr[%d] = %d\n", i - 1, ptr[i-1]);
	}

	exit(0);
}
