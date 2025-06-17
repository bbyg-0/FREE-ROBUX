#ifndef _COPY_FILE_H
#define _COPY_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <direct.h>
#include <errno.h>
#include <stdbool.h>

#include "showAllFile.h" // Buat ambil MAX_PATH

#define BUFFER_SIZE 4096

// Struktur untuk menyimpan informasi file dalam queue
typedef struct FileNode {
    char sourcePath[MAX_PATH];
    char destPath[MAX_PATH];
    struct FileNode* next;
} FileNode;

// Struktur Queue
typedef struct {
    FileNode* front;
    FileNode* rear;
} FileQueue;

// Fungsi-fungsi Queue
void createQueue(FileQueue* q);
void enqueue(FileQueue* q, const char* sourcePath, const char* destPath);
int dequeue(FileQueue* q, char* sourcePath, char* destPath);
void clearQueue(FileQueue* q);

int copyFile(const char *sourcePath, const char *destPath);
// Fungsi untuk menyalin direktori dan semua isinya menggunakan queue
int copyDirectory(const char *sourceDir, const char *destDir);
int isFile(const char *path);
int isDirectory(const char *path);

#endif // COPY_FILE_H
