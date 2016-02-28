#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>


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
	char skippingChars[] = " \t\n\"";	//breaking characters.
	char *temp ;	//temporary variable to store string.
	
	//strtok finds the token where it finds the skippingChars's any character and returns a pointer to it 
	temp = strtok(str,skippingChars);
	while(temp){
		argv[numberOfArguments++] = temp;
		temp = strtok(NULL, skippingChars);
	}
	argv[numberOfArguments] = NULL;
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
		_exit(EXIT_FAILURE);
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
// > is to file, < is input from file

int IORedirection(char** argv){
	int found = 0;
	char* file_in = NULL;
	char* file_out = NULL;
	int i;
	for(i = 0 ; i < MAXLEN ; i++){
		if(argv[i] == NULL) break;
		if(!strcmp(argv[i],">")){
			found = 1;
			argv[i] = NULL;
			file_out = argv[i+1];
		}
		else if(!strcmp(argv[i],"<")){
			found = 1;
			argv[i] = NULL;
			file_in = argv[i+1];
		}
		else if(!strcmp(argv[i],">>")){
			found = 2;
			argv[i] = NULL;
			file_out = argv[i+1];
		}
	}
	if(found){
		pid_t pid, wpid;
		int status;
		pid = fork();
		if(!pid){
			if(file_out && found == 1){
				//0_TRUNC sets the length to zero, which means it will clear the file after opening.
				int file_o = open(file_out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				if(file_o < 0){
					perror("file_out error");
					_exit(EXIT_FAILURE);
				} 
				dup2(file_o,1);
			}
			else if(file_out && found == 2){
				//0_APPEND doesn't set length to zero, which means it will let user append from the last save.
				int file_o = open(file_out, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				if(file_o < 0){
					perror("file_out error");
					_exit(EXIT_FAILURE);
				}
				dup2(file_o,1);
			}
			if(file_in){
				int file_i = open(file_in, O_RDONLY);
				if(file_i < 0){
					perror("file_in error");
					_exit(EXIT_FAILURE);
				}
				dup2(file_i,0);
			}
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
	}
	return found;
}

void execute_pipe(char **argv1, char **argv2){
	int fd[2],wpid,status;
	if(pipe(fd)){
		perror("piped failed");
		_exit(EXIT_FAILURE);
	}
	pid_t pid = fork();
	if(!pid){
		dup2(fd[1],1);
		close(fd[0]);
		execvp(argv1[0],argv1);
	}
	else if(pid < 0){
		perror("fork failed");
	}
	else{
		do{
			wpid = waitpid(pid,&status,WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		dup2(fd[0],0);
		close(fd[1]);
		execvp(argv2[0],argv2);
	}
}

int tauSH_pipe(char** argv){
	int found = 0, wpid,status;
	int fileDesc[2];
	//printf("over here");
	char **argv2 = (char**)malloc(MAXLEN * sizeof(char*));
	int i;
	for(i = 0 ; i < MAXLEN ; i++){
		if(argv[i] == NULL) break;
		if(!strcmp(argv[i],"|")){
			found = 1;
			argv[i] = NULL;
			int j,k;
			for(j = i + 1,k = 0; j < MAXLEN ; j++){
				argv2[k++] = argv[j];
			}
			break;
		}
	}
	if(found){
		pid_t pid = fork();
		if(!pid){
			execute_pipe(argv,argv2);
		}
		else if(pid < 0){
			perror("fork failed");
			_exit(EXIT_FAILURE);
		}
		else{
			do{
				wpid = waitpid(pid,&status,WUNTRACED);
			}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
	
	return found;
}
void runBGProcess(char** argv){
	pid_t pid , wpid;
	int status;
	pid = fork();
	if(!pid){
		execvp(argv[0],argv);
	}
	else if(pid < 0){
		perror("fork failed");
		_exit(EXIT_FAILURE);
	}
	
}
int backGroundProcess(char** argv){
	int i ;
	int found = 0;
	for(i = 0 ; i < MAXLEN; i++){
		if(argv[i] == NULL) break;
		if(!strcmp(argv[i],"&")){
			found = 0;
			argv[i] = NULL;
			break;
		}
	}
	if(found){
		/*pid_t pid = fork();
		if(!pid){
			runBGProcess(argv);
		}
		else if(pid < 0){
			perror("fork failed");
			_exit(EXIT_FAILURE);
		}
		else{
			return found;
		}*/
		runBGProcess(argv);
	}
	return found;
}

int tauSH_execute(char** argv){
	//here the built-ins will be written because they can't be done by using child process.
	if(IORedirection(argv)){
		return 1;
	}
	if(tauSH_pipe(argv)){
		return 1;
	}
	
	if(backGroundProcess(argv)){
		return 1;
	}
	
	//chdir changes the DIRECTORY, that is pretty convinient. 
	if(!strcmp(argv[0],"cd")){
		if(argv[1] == NULL){
			//i couldn't implement "cd " :( 
			fprintf(stderr,"PARAMETER NEEDED");
		}
		else{
			if(chdir(argv[1]) != 0){
				fprintf(stderr,"DIRECTORY NOT FOUND\n");
			}
		}
		return 1;
	}
	else if(!strcmp(argv[0],"help")){
		printf("WELCOME TO MY VERY OWN SHELL\nIt is very much like the normal bash.\n");
		printf("IO redirection and one level pipe can be done.\n");
		printf("Normal commands are added.\nBuilt-in commands such as cd, echo, help, exit are implemented.\n");
		printf("IO redirection and pipe doesn't work in same command. Please use without caution.\n");
		printf("Background Process doesn't work.\n");
		printf("Press either CTRL+C or write \"$exit\" to escape the program.\n");
		return 1;
	}
	//only \t will not work, as it was taken as a breaking character while parsing.
	else if(!strcmp(argv[0],"echo")){
		int i;
		for(i = 1 ; i < MAXLEN; i++){
			if(!argv[i]) break;
			printf("%s ",argv[i]);
		}
		printf("\n");
		return 1;
	}
	else if(!strcmp(argv[0],"exit")){
		return 0;
	}
	return tauSH_run(argv);
}

void tauSHLoop(){
	int status = 0;
	char line[MAXLEN];
	char **argv;
	do{
		printf("> ");
		gets(line);
		argv = SplitString(line);
		status = tauSH_execute(argv);
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
