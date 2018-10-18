#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 1000

void Write_Read (int procIndec, int *fd, char *input, char *output) {

        if (procIndec == 1) {

                fgets(input, SIZE, stdin);
                write(fd[1], input, SIZE);

        } else if (procIndec == 0) {

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
                printf ("Write: %s      Read: %s       Zero client\n\n", fileAdressWrite, fileAdressRead);

        } else {

                fileAdressWrite = name2;
                fileAdressRead = name1;
                printf ("Write: %s       Read: %s      First client \n\n", fileAdressWrite, fileAdressRead);
        }

        pid_1 = fork();

        int procIndec;

        if (clientID == 0) { // O clientID

                if (pid_1 == 0) {
                        procIndec = 1;
                        fd[1] = open (fileAdressWrite, O_WRONLY);
                        while(1) {
                                Write_Read (procIndec, fd, input, output);
                        }

                } else if (pid_1 > 0) {
                        procIndec = 0;
                        fd[0] = open (fileAdressRead, O_RDONLY);
                        while(1) {
                                Write_Read (procIndec, fd, input, output);
                        }

                }

        } else if (clientID == 1) { //1 clientID

                if (pid_1 == 0) {
                        procIndec = 1;
                        fd[1] = open (fileAdressWrite, O_WRONLY);
                        while(1) {
                                Write_Read (procIndec, fd, input, output);
                        }


                } else if (pid_1 > 0) {
                        procIndec = 0;
                        fd[0] = open (fileAdressRead, O_RDONLY);
                        while(1) {
                                Write_Read (procIndec, fd, input, output);
                        }
                }

        } else {// 'O_o ???-ClientID-??? o_O'

                printf ("Error clientID\n");
                exit(-1);
        }
        return 0;
}
