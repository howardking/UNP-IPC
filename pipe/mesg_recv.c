#include "mesg.h"

ssize_t mesg_recv(int fd, struct mymesg *mptr)
{
	size_t len;
	ssize_t n;

	if ((n = read(fd, pmtr, MESGHDRSIZE)) == 0){
		return 0; /* 文件结束 */
	}else if (MESGHDRSIZE != n){
		printf("message header: expected %d, got %d\n", MESGHDRSIZE, n);
		exit(1);
	}

	if ((len = mptr->mesg_len) > 0){
		if ((n = read(fd, mptr->mesg_data, len)) != len){
			printf("message data: expected %d, got %d\n", len, n);
			exit(1);
		}
	}

	return len;

}
