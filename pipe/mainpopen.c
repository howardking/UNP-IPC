#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAXLINE 4096

int main(int argc, char *argv[])
{
	size_t n;
	char buff[MAXLINE], command[MAXLINE];
	FILE *fp;

	fgets(buff, MAXLINE, stdin);
	n = strlen(buff);
	if (buff[n-1] == '\n'){
		n--;
	}

	snprintf(command, sizeof(command), "cat %s", buff);
	fp = popen(command ,"r");
	if (fp == NULL){
		perror("popen failed:");
		exit(1);
	}

	while (fgets(buff, MAXLINE, fp) != NULL){
		fputs(buff, stdout);
	}

	if(pclose(fp) == -1){
		perror("pclose failed:");
		exit(1);
	}

	exit(0);
	
}
