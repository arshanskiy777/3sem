#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Запускать программу: ./program <first number to multiplication> <second number to multiplication>
// Пример: ./Client 4 19
// Запущенный со значениями 0 и 0 клиент удаляет очередь сообщений

int main(int argc, char *argv[]) {
	int msqid;
	char pathname[] = "key.c";
	key_t key;
	int i, len, maxlen;
	int mult1 = atoi(argv[1]), mult2 = atoi(argv[2]);
	long type;

	struct mymsgbuf{
                long mytype;
                struct msinfo{
                        int num1, num2;
                        long mstype;
                } info;
        } mybuf;

        key = ftok(pathname, 0);

	if (key < 0){
		printf("Can\'t generate key.\n");
		exit(-1);
	}

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get mesqid\n");
		exit(-1);
	}

	if ((mult1 == 0)&&(mult2 == 0)){
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
        }

	pid_t pid = getpid();
	type = pid;
	mybuf.info.mstype = pid;
	mybuf.mytype = pid;
	printf("My id = %d\n", pid);
	len = sizeof(struct mymsgbuf) - sizeof(long);
	mybuf.info.num1 = mult1;
	mybuf.info.num2 = mult2;
	if(msgsnd(msqid, &mybuf, len, 0) < 0) {
		printf("Can\'t send message.\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
			exit(-1);
	} else {
	printf("Message was sent\n");
	}
	//принимаем результат работы сервера
	if(maxlen = msgrcv(msqid, &mybuf, len, type, 0) < 0){
                printf("Can\'t receive message from queue\n");
                exit(-1);
        } else {
        printf ("The message is received.\nResult: %d * %d = %d\n", mult1, mult2, mybuf.info.num1);
        }

	//
	return 0;

}
