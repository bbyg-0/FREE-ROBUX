#pragma once

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "../target/command.h"


#ifdef _WIN32
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock Library

typedef struct parameterThread{
	SOCKET serverSocket;
	SOCKET clientSocket;
	int addrlen;
	struct sockaddr_in address;
	char * litAddress;
	char socketStatus;
} paramThread;
#include "../target/SURF/seeDirectory.h"

#else
#include <netinet/in.h>
#include <sys/socket.h>

typedef struct param{
	int serverSocket;
	int clientSocket;
	socklen_t addrlen;
	struct sockaddr_in address;
	char * litAddress;
	char socketStatus;
} paramThread;
#endif

int openFolder(char * path);

void inisialisasiParamThread(paramThread * param);

void isiStatus(paramThread * param, char status);

void isiPort(paramThread * param, int port);

void isiAddress(paramThread * param, char * address);

#ifdef _WIN32
void inisialisasiSocket(WSADATA * wsa);

DWORD WINAPI serverSocket(LPVOID param);

DWORD WINAPI clientSocket(LPVOID param);

DWORD WINAPI sendMessage(LPVOID param);

DWORD WINAPI getMessage(LPVOID param);

DWORD WINAPI getMessageController(LPVOID param);

DWORD WINAPI execMessage(LPVOID paramT);
#else
void * serverSocket(void * vParamT);

void * clientSocket(void * vParamT);

void * repairingServer(void * vParamT);

void * sendMessage (void * client);

void * getMessage (void * client);

void * getMessageController (void * client);
#endif
