#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include "../common/common.h"
#include "../common/socket.h"

#define PORT 8080
int main(int argc, char const* argv[]){
	
	pthread_t servSocket, send, get;
	paramThread param;
	inisialisasiParamThread(&param);
	isiPort(&param, PORT);
	isiStatus(&param, 's');

	void * dump = NULL;

	pthread_create(&servSocket, NULL, serverSocket, (void *)&param);

	pthread_create(&send, NULL, sendMessage, (void *)&(param.clientSocket));
	pthread_create(&get, NULL, getMessage, (void *)&param);

	pthread_join(servSocket, NULL);
	pthread_join(send, NULL);
	pthread_join(get, NULL);

	// closing the connected socket
	close((param).clientSocket);
  
	// closing the listening socket
	close((param).serverSocket);
	return 0;
}
