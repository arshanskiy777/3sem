#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<assert.h>
#define Max_size 100
#define Max_Time 5


void Split(char* string,char* delimiters,char*** tokens,int* tokensCount){

	int len = strlen(string);
	(*tokensCount) = 0;
	(*tokens)[*tokensCount] = strtok(string, delimiters);

	while((*tokens)[*tokensCount] != NULL){
 		(*tokensCount)++;
 		(*tokens)[*tokensCount] = strtok(NULL, delimiters);
	}
}

int main(){
	int i;
	char string[Max_size] = {};
	int tokensCount = 0;
	char *estr;
	int status;
	char str[Max_size] = {};
	char delimiters[2] = {'\t','\n'};
	FILE *mf;
	mf = fopen("example.txt", "r");
	do {
		estr = fgets (str,Max_size,mf);
		strcat(string, str);
		if (estr == NULL){
			break;
		}
	} while (1);

	char **tokens;
	tokens = (char**)malloc(Max_size * sizeof(char*));

	for(i = 0; i < Max_size; i++){
		tokens[i] = (char*)malloc(Max_size * sizeof(char));
	}

	Split (string, delimiters, &tokens, &tokensCount);
	for (i = 0; i < tokensCount; i++){
		char* argv_[Max_size] = {0};
		int j = 0;
		argv_[0] = strtok(tokens[i], " ");
		do {
			j++;
			argv_[j] = strtok(NULL, " ");
		} while (argv_[j] != NULL);
		
		pid_t pid;
		pid = fork();

		if (pid == 0){
			pid = fork();

			if (pid == 0){
				sleep(atoi(argv_[0]));
				int g;

				for(g = 0; g < Max_size - 1; g++){

					argv_[g] = argv_[g + 1]; 
				}

				execvp(argv_[0], argv_);
			
			} else {
				sleep(atoi(argv_[0]) + Max_Time);
				if (waitpid(pid, &status, WNOHANG))
					exit(0);
				else {

					kill(pid, SIGKILL);
					exit(0);
				}
			}
		}
	}

	free(tokens);
	return 0;
	
}

