#include <stdio.h>
#include <stdlib.h>
/*#include <sys/msg.h>*/
#include <linux/msg.h>
//#include <sys/ipc.h>

/*#define SVMSG_MODE (MSG_R | MSG_W | MSG_R>>3 | MSG_R>>6)*/
#define SVMSG_MODE 0644

int main(int argc, char *argv[])
{
	int msqid;
	struct msqid_ds info;
	struct msgbuf  buf;

	msqid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT);
	if (-1 == msqid){
		perror("msgget error");
		exit(1);
	}

	buf.mtype = 1;
	buf.mtext[0] = 1;
	if (-1 == msgsnd(msqid, &buf, 1, 0)){
		perror("msgsnd error");
		exit(1);
	}
	
	if (-1 == msgctl(msqid, IPC_STAT, &info)){
		perror("msgctl error");
		exit(1);
	}

	printf("read-write: %03o, cbytes = %lu, qnum = %lu, qbytes = %lu\n", info.msg_perm.mode & 0777, (unsigned long) info.msg_cbytes, (unsigned long) info.msg_qnum, (unsigned long) info.msg_qbytes);

	system("ipcs -q");

	msgctl(msqid, IPC_RMID, NULL);
	exit(0);
}
