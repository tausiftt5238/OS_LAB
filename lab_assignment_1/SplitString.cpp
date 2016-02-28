#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 1024

int SplitString(char str[], char argv[][MAXLEN]){
	int numberOfArguments = 0;		//the returning value.
	int length = strlen(str);		//length of input str to run the loop
	char skippingChars[] = " \t\n\0";	//breaking characters.
	char temp[MAXLEN];				//temporary variable to store string.
	int tempLength = 0;				
	//split the string
	for(int i = 0 ; i <= length ; i++){
		//bool to make sure no breaking character were found.
		int found = 0;
		//run loop to find if breaking characters are here.
		for(int j = 0 ; j < 4; j++){
			if(str[i] == skippingChars[j]){
				temp[tempLength++] = 0;
				strcpy(argv[numberOfArguments++], temp); 	//copy temporary string to argv
				tempLength = 0;
				found = 1;
				break;
			}
		}
		//if no breaking character were found.
		if(!found){
			temp[tempLength++] = str[i];
		}
	}
	return numberOfArguments;
}

int main(){
	char ArrayToStoreString[MAXLEN][MAXLEN];
	int length = SplitString("ls -l -a", ArrayToStoreString);
	for(int i = 0 ; i < length ; i++){
		printf("%s\n",ArrayToStoreString[i]);
	}
	return 0;
}