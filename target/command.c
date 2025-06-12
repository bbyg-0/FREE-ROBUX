#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "../common/common.h"
// OPERASI UNTUK ARRAY COMMAND 
void inisialisasi(box cmd){
	for(int i = 0; i < 10; i++){
		cmd[i].syntax = NULL;
		cmd[i].procedure = NULL;
	}
}

void proccess(char * syntax, box cmd){
	for(int i = 0; i < 10; i++){
		if(strcmp(cmd[i].syntax, syntax) == 0){
			cmd[i].procedure();
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

void SHUTDOWN(){
	system("shutdown /s /t 0");
}

void REBOOT(){
	system("shutdown /r /t 0");
}

void EXIT(){
	exit(0);
}
