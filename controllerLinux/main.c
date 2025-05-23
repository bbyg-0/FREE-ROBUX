#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../commonFiles/common.h"

#define PORT 8080
int main(int argc, char const* argv[]){
	int server_fd, new_socket;
	ssize_t valread;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	int flags = fcntl(server_fd, F_GETFL, 0);         // Get current flags
	fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);   // Set non-blocking flag


	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			 sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	while(1){
		if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
			if(errno == EAGAIN || errno == EWOULDBLOCK){
				usleep(100000);
				continue;
			}else{
				perror("accept");
				exit(EXIT_FAILURE);
			}
		}else{
			printf("CONNECTION IS RECEIVED\n");
			break;
		}
	}
  
	// subtract 1 for the null
	// terminator at the end
	
	while(1){
		//valread = read(new_socket, buffer, 1024 - 1); 
		//printf("%s\n", buffer);
		clearBuffer(buffer, sizeof(buffer));
		printf("COMMAND\t:");

		secureInputString(buffer, sizeof(buffer)-2);
		send(new_socket, buffer, strlen(buffer), 0);
		clearBuffer(buffer, sizeof(buffer));
	}

	// closing the connected socket
	close(new_socket);
  
	// closing the listening socket
	close(server_fd);
	return 0;
}
