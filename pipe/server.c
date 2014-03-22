#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>

#ifndef MAXLINE
#define MAXLINE 4096
#endif /*MAXLINE*/

/*
int snprintf(char *buf, size_t size, const char *fmt, ...)
{
	int n;
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);

	n = strlen(buf);
	va_end(ap);
	if (n >= size){
		printf("snprintf: '%s' overflowed array", fmt);
		exit(1);
	}
	return n;
}
*/
void server(int readfd, int writefd)
{
	int fd;
	ssize_t n;
	char buff[MAXLINE+1];

	if ((n = read(readfd, buff, MAXLINE)) == 0){
		perror("end-of-file while reading pathname");
		exit(1);
	}

	buff[n] = '\n';

	if ((fd = open(buff, O_RDONLY)) < 0){
		snprintf(buff + n, sizeof(buff)-n, ": can't open, %s\n", strerror(errno));
		n = strlen(buff);

		write(writefd, buff, n);
	}else {
		while((n = read(fd, buff, MAXLINE))>0){
			write(writefd, buff, n);
		}
		close(fd);
	}
}
