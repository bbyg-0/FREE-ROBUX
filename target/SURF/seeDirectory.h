#pragma once
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include "../../common/socket.h"
#include "showAllFile.h"
#include "../encryption/fileEncryption.h"

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

void DELETEFILE(void * paramS);
