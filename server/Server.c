#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>

#define CLIENT_NUMBER 2
#define SV_TYPE 1
#define SLEEP_TIME 10

int main (/*int argc, char *argv[]*/) {
	//int id = atoi(argv[1]);
	int msqid, semid;
        char pathname[] = "key.c";
        key_t key;
        int i, len, result;
	struct sembuf A;
        struct sembuf D;
        struct mymsgbuf{
                long mytype;
                struct msinfo{
			int num1, num2;
			long mstype;
		} info;
        } mybuf;

        if ((key = ftok(pathname, 0)) < 0){
                printf("Can\'t generate key.\n");
                exit(-1);
        }
	semid = semget(key, 1, 0666 | IPC_CREAT);
	A.sem_num = 0;
        A.sem_op = 1;
        A.sem_flg = 0;
        D.sem_num = 0;
        D.sem_op = -1;
        D.sem_flg = 0;
	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get msqid\n");
		exit(-1);
	}
	for (int i = 0; i < CLIENT_NUMBER; i++){
		semop(semid, &A, 1);
	}
	int maxlen = sizeof(struct mymsgbuf) - sizeof(long);
	pid_t pid;

	//Получение данных:
	//Обработка полученных данных:
	while(1){		
		if(len = msgrcv(msqid, &mybuf, maxlen, 0, 0) < 0){
	       	        printf("Can\'t receive message from queue\n");
	       	        exit(-1);
	       	} else {
		       	printf ("The message is received.\n");
	        }
		semop(semid, &D, 1);
		pid = fork();
		if (pid == 0){
			sleep(SLEEP_TIME);
	        	result = mybuf.info.num1 * mybuf.info.num2;
	        	printf("Working %d * %d = %d\n", mybuf.info.num1, mybuf.info.num2, result);
	        	mybuf.info.num1 = result;
	       		mybuf.mytype = mybuf.info.mstype;

	        	if(msgsnd(msqid, &mybuf, maxlen, 0) < 0) {
		                printf("Can\'t send message.\n");
		                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                        	exit(-1);
	        	} else {
		        	printf("Message was sent to client with id: %ld\n", mybuf.mytype);
	        	}
			semop(semid, &A, 1);
			exit(0);
		}
	}
		//Конец обработки данных и отправки результата



	msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

	return 0;
}
