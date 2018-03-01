#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

struct msgmbuf
{
	long msg_type;
	char msg_text[512];
};

int main()
{
	int qid;
	key_t key;
	int len;
	struct msgmbuf msg;

	if ((key = ftok(".", 'a')) == -1)
	{
		perror("ftok");
		exit(1);
	}

	if ((qid = msgget(key, IPC_CREAT | 0666)) == -1)
	{
		perror("msgget");
		exit(1);
	}

	printf("create/open queue id is : %d\n", qid);
	puts("please input send queue msg : ");

	if ((fgets((&msg)->msg_text, 512, stdin)) == NULL)
	{
		puts("no message");
		exit(1);
	}

	msg.msg_type = getpid();
	len = strlen(msg.msg_text);

	if ((msgsend(qid, &msg, 512, 0, 0)) < 0)
	{
		perror("msgsend");
		exit(1);
	}

	if (msgrcv(qid, &msg, 512, 0, 0) < 0)
	{
		perror("msgrcv");
		exit(1);
	}

	printf("msg received is : %s\n", (&msg)->msg_text);

	if (msgctl(qid, IPC_RMID, NULL) < 0)
	{
		perror("msgctl");
		exit(1);
	}
	
	return 0;
}