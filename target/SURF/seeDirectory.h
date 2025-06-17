#pragma once
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include "copyFile.h"
#include "showAllFile.h"
#include "../../common/socket.h"
#include "../encryption/fileEncryption.h"

typedef struct paramSurff{
	paramThread * paramT;
	DIR * dir;
	char pwd[2048];
	char input[10];
} paramSurf;

#ifdef _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif

void showMenu();
DWORD WINAPI surfMode(LPVOID param);
void showPWD(void * paramS);
//void handleEncryption(const char *pwd);
