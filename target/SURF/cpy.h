#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>    // Dirent (POSIX-style), harus tersedia via MinGW atau eksternal
#include <windows.h>   // Untuk Windows path
#include <sys/stat.h>

#include "../../common/common.h"
#include "seeDirectory.h"

typedef int infotype1;
typedef char* infotype2;
typedef struct Q *address;
typedef struct Q {
	char * file_name;
	char * content;
	address next;
} qList;

void DeAlokasi (address *p);

void Create_Q(address *p);

void Isi_Q (address *p, char* nama, char * X);

void enqueue (address *p, address *pNew);

void getValue (address *p, char * name, char * isi);

void dequeue (address *p);

char *read_file(const char *filepath);

void exec(paramSurf * param);
