#include "mesg.h"

ssize_t mesg_send(int fd, struct mymesg *mptr)
{
	return (write(fd, mptr, MESGHDRSIZE + mtr->mesg_len));
}
