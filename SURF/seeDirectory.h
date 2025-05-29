#ifndef _SEE_DIRECTORY_H
#define _SEE_DIRECTORY_H

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif

#define MAX_PATH 260

void seeDirectory();

#endif