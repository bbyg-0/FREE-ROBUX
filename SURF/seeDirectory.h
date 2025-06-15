#ifndef _SEE_DIRECTORY_H
#define _SEE_DIRECTORY_H

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include "copyFile.h"
#include "showAllFile.h"

#ifdef _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif

#define MAX_PATH 260

void showMenu();
void surfMode();
void showPWD(const char *pwd);

#endif
