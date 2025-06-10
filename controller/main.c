#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#endif

#include "../common/socket.h"

#define PORT 8080
int main(int argc, char const* argv[]){
	
	paramThread param;

	inisialisasiParamThread(&param);
	isiPort(&param, PORT);
	isiStatus(&param, 'S');

#ifdef _WIN32
	HANDLE servSocket, sendMSG, getMSG;
	DWORD servSocketId, sendMSGId, getMSGId;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);

	servSocket = CreateThread(NULL, 0, serverSocket, (LPVOID)&param, 0, &servSocketId);
	sendMSG = CreateThread(NULL, 0, sendMessage, (LPVOID)&param, 0, &sendMSGId);
	getMSG = CreateThread(NULL, 0, getMessage, (LPVOID)&param, 0, &getMSGId);


	WaitForSingleObject(servSocket, INFINITE);
	CloseHandle(servSocket);

	WaitForSingleObject(sendMSG, INFINITE);
	CloseHandle(sendMSG);

	WaitForSingleObject(getMSG, INFINITE);
	CloseHandle(getMSG);
#else
	pthread_t servSocket, send, get;

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
#endif
	return 0;
}
