#pragma once

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct param{
	int serverSocket;
	int clientSocket;
	socklen_t addrlen;
	struct sockaddr_in address;
	char * litAddress;
	char socketStatus;
} paramThread;

void inisialisasiParamThread(paramThread * param);

void isiStatus(paramThread * param, char status);

void isiPort(paramThread * param, int port);

void isiAddress(paramThread * param, char * address);

void * serverSocket(void * vParamT);

void * clientSocket(void * vParamT);

void * repairingServer(void * vParamT);

void * sendMessage (void * client);

void * getMessage (void * client);
