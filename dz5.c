#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define SIZE 1000

void startWriter (int *fd, char *input, char *fileAdressWrite) {
        //fd[1] = open (fileAdressWrite, O_WRONLY); 
        while(1) {
                fgets(input, SIZE, stdin);
                write(fd[1], input, SIZE);
        }
}

void startReader (int *fd, char *output, char *fileAdressRead) {
       //fd[0] = open (fileAdressRead, O_WRONLY);
        while(1) {
                read(fd[0], output, SIZE);
                fprintf(stdout, "%s", output);
        }
}

int main (int argc, char *argv[]) {
        char name1[] = "aaa1fifo", name2[] = "aaa2fifo", *fileAdressRead, *fileAdressWrite;
        char input[SIZE] = {0};
        char output[SIZE] = {0};
        int clientID;
        clientID = atoi (argv[1]);
        int fd[2] = {0};
        pid_t pid_1;
        mkfifo (name1, 0777);
        mkfifo (name2, 0777);

        if (clientID == 0) {
                fileAdressWrite = name1;
                fileAdressRead = name2;
                printf ("%s      %s       Zero client\n\n", fileAdressWrite, fileAdressRead);

        } else { //All other numbers - second client
                fileAdressWrite = name2;
                fileAdressRead = name1;
                printf ("%s       %s      First client \n\n", fileAdressWrite, fileAdressRead);
        }

        pid_1 = fork();
        if (pid_1 == 0) {
                fd[1] = open (fileAdressWrite, O_WRONLY);
                startWriter(fd, input, fileAdressWrite);
        } else if (pid_1 > 0) {
                fd[0] = open (fileAdressRead, O_RDONLY);
                startReader(fd, output, fileAdressRead);
        }

        return 0;
}
