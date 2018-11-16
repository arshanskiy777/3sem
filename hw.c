#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<assert.h>

#define MAX_SIZE 100
#define MAX_TIME 5

void Split(char* string,char* delimiters,char*** tokens,int* tokensCount){

	int len = strlen(string);
	*tokensCount = 0;
	(*tokens)[*tokensCount] = strtok(string, delimiters);
	while((*tokens)[*tokensCount] != NULL){
		(*tokensCount)++;
		(*tokens)[*tokensCount] = strtok(NULL, delimiters);
	}
}

int main(){
	int i;
	char string[MAX_SIZE] = {};
	int tokensCount = 0;
	char *estr;
	int status;
	char str[MAX_SIZE] = {};
	char delimiters[2] = {'\t','\n'};
	FILE *mf;
	mf = fopen("primer.txt", "r");
	do{
      		estr = fgets(str, MAX_SIZE, mf);
		strcat(string, str);
		if (estr == NULL){
			break;
		}
	}while(estr);
	
	char **tokens;
	tokens = (char**)calloc(MAX_SIZE, sizeof(char*));
	for(i = 0; i < MAX_SIZE; i++){
		tokens[i] = (char*)calloc(MAX_SIZE, sizeof(char));
	}
	Split(string, delimiters, &tokens, &tokensCount);
	char **argv;
	argv = (char**)calloc(MAX_SIZE, sizeof(char*));
	int k;
	for(k = 0; k < MAX_SIZE; k++){
		argv[k] = (char*)calloc(MAX_SIZE, sizeof(char));
	}
	for (i = 0; i < tokensCount; i++){
		int j = 0;
		char* delimiter = " ";
		Split(tokens[i], delimiter, &argv, &j);
		int timeout = atoi(argv[0]);
		pid_t  pid;
		pid = fork();
		if (pid == 0){
			pid = fork();
			if (pid == 0){
				sleep(timeout);
				execvp(argv[1], argv + 1);
				exit(0);
			} else { 
				sleep(timeout + MAX_TIME);
				if(waitpid(pid, &status, WNOHANG))
					exit(0);
				else
				{
					kill(pid, SIGKILL);
					exit(0);
				}
			}
		}
	}
        free(tokens);
        return 0;	
}
