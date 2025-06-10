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
#define PORT 8080
#define ADDRESS "127.0.0.1"
int main() {
	paramThread param;

	inisialisasiParamThread(&param);
	isiPort(&param, PORT);
	isiAddress(&param, ADDRESS);
	isiStatus(&param, 'C');


#ifdef _WIN32
	HANDLE cliSocket, sendMSG, getMSG;
	DWORD cliSocketId, sendMSGId, getMSGId;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);

	cliSocket = CreateThread(NULL, 0, clientSocket, (LPVOID)&param, 0, &cliSocketId);
	sendMSG = CreateThread(NULL, 0, sendMessage, (LPVOID)&param, 0, &sendMSGId);
	getMSG = CreateThread(NULL, 0, getMessage, (LPVOID)&param, 0, &getMSGId);


	WaitForSingleObject(cliSocket, INFINITE);
	CloseHandle(cliSocket);

	WaitForSingleObject(sendMSG, INFINITE);
	CloseHandle(sendMSG);

	WaitForSingleObject(getMSG, INFINITE);
	CloseHandle(getMSG);
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

