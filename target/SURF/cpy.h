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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>    // Dirent (POSIX-style), harus tersedia via MinGW atau eksternal
#include <windows.h>   // Untuk Windows path
#include <sys/stat.h>

#define FOLDER_NAME "FOLDER1"

typedef struct Node {
    char filename[256];
    char *content;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

void initQueue(Queue *q);

void enqueue(Queue *q, const char *filename, const char *content);

void dequeueAll(Queue *q);

char *read_file(const char *filepath);

void exec(paramSurf * param);
