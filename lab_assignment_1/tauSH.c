#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//macros
#define MAXLEN 1024

//SplitString function in order to divide the commands.
char **SplitString(char str[]){
	
	char **argv = (char**)malloc(MAXLEN * sizeof(char*));
	if(!argv){
		perror("ALLOCATION FAILED\n");
		_exit(EXIT_FAILURE);
	}
	int numberOfArguments = 0;		
	int length = strlen(str);		//length of input str to run the loop
	char skippingChars[] = " \t\n";	//breaking characters.
	char *temp ;	//temporary variable to store string.
	
	//strtok finds the token where it finds the skippingChars's any character and returns a pointer to it 
	temp = strtok(str,skippingChars);
	while(temp){
		argv[numberOfArguments++] = temp;
		temp = strtok(NULL, " \t\n");
	}
	return argv;
}

int tauSH_run(char** argv){
	pid_t pid, wpid;
	int status;
	pid = fork();
	if(!pid){
		if(execvp(argv[0],argv) == -1){
			perror("COMMAND NOT FOUND");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid < 0){
		perror("FORK FAILED");
	}
	else{
		do{
			wpid = waitpid(pid,&status,WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		//WIFEXITED = true if child exited normally.
		//WIFSIGNALED = true if child exited due to uncaught signal.
	}
	return 1;
}

void tauSHLoop(){
	int status = 0;
	char line[MAXLEN];
	char **argv;
	//int argc;
	do{
		printf("> ");
		gets(line);
		argv = SplitString(line);
		status = tauSH_run(argv);
	}while(status);
}

void welcomeScreen(){
	system("reset");
	printf("WELCOME TO MY FIRST SHELL! - author : Md. Tahsin Tausif\n");
}
int main(int argc, char *argv[]){
	welcomeScreen();
	tauSHLoop();
	return 0;
}
