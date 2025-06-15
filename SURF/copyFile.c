#include "copyFile.h"
#include <sys/stat.h>
#include <direct.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define BUFFER_SIZE 4096
#define MAX_PATH 260

// Implementasi fungsi-fungsi Queue
void initQueue(FileQueue* q) {
    q->front = NULL;
    q->rear = NULL;
}

int isEmpty(FileQueue* q) {
    return (q->front == NULL);
}

void enqueue(FileQueue* q, const char* sourcePath, const char* destPath) {
    FileNode* newNode = (FileNode*)malloc(sizeof(FileNode));
    if (newNode == NULL) return;

    strncpy(newNode->sourcePath, sourcePath, MAX_PATH - 1);
    strncpy(newNode->destPath, destPath, MAX_PATH - 1);
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(FileQueue* q, char* sourcePath, char* destPath) {
    if (isEmpty(q)) return 0;

    FileNode* temp = q->front;
    strncpy(sourcePath, temp->sourcePath, MAX_PATH - 1);
    strncpy(destPath, temp->destPath, MAX_PATH - 1);

    q->front = temp->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return 1;
}

void clearQueue(FileQueue* q) {
    while (!isEmpty(q)) {
        char sourcePath[MAX_PATH], destPath[MAX_PATH];
        dequeue(q, sourcePath, destPath);
    }
}

int isDirectory(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return 0;
    }
    return S_ISDIR(path_stat.st_mode);
}

const char* getFileName(const char *path) {
    const char *fileName = strrchr(path, '\\');
    if (fileName != NULL) {
        return fileName + 1;  // Lompat backslash
    }
    return path;
}

int isFile(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return 0;
    }
    return S_ISREG(path_stat.st_mode);
}

int copyDirectory(const char *sourceDir, const char *destDir) {
    DIR *dir;
    struct dirent *entry;
    FileQueue fileQueue;
    char sourcePath[MAX_PATH];
    char destPath[MAX_PATH];
    int status = 0;

    // Periksa apakah source adalah direktori
    if (!isDirectory(sourceDir)) {
        return -1;  // Source bukan direktori
    }

    // Buka direktori sumber
    dir = opendir(sourceDir);
    if (dir == NULL) {
        return -2;  // Gagal membuka direktori sumber
    }

    // Buat direktori tujuan jika belum ada
    if (_mkdir(destDir) != 0 && errno != EEXIST) {
        closedir(dir);
        return -3;  // Gagal membuat direktori tujuan
    }

    // Inisialisasi queue
    initQueue(&fileQueue);

    // Pertama, kumpulkan semua file dalam queue
    while ((entry = readdir(dir)) != NULL) {
        // Skip . dan ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Buat path lengkap untuk source dan destination
        snprintf(sourcePath, MAX_PATH, "%s\\%s", sourceDir, entry->d_name);
        snprintf(destPath, MAX_PATH, "%s\\%s", destDir, entry->d_name);

        if (isFile(sourcePath)) {
            // Tambahkan file ke dalam queue
            enqueue(&fileQueue, sourcePath, destPath);
        } else if (isDirectory(sourcePath)) {
            // Untuk direktori, buat direktori baru dan proses secara rekursif
            status = copyDirectory(sourcePath, destPath);
            if (status < 0) {
                closedir(dir);
                clearQueue(&fileQueue);
                return -4;  // Error dalam subdirektori
            }
        }
    }
    closedir(dir);

    // Proses semua file dalam queue
    while (!isEmpty(&fileQueue)) {
        dequeue(&fileQueue, sourcePath, destPath);
        status = copyFile(sourcePath, destPath);
        if (status != 0) {
            clearQueue(&fileQueue);
            return -5;  // Error saat menyalin file
        }
    }

    return 0;  // Sukses
}

int copyFile(const char *sourcePath, const char *destPath) {
    FILE *sourceFile, *destFile;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    char finalDestPath[MAX_PATH];

    // Periksa apakah sumber adalah file
    if (!isFile(sourcePath)) {
        return -1;  // Sumber bukan file
    }

    // Buka file sumber dalam mode binary read
    sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL) {
        return -2;  // Gagal membuka file sumber
    }

    // Jika destPath adalah direktori, tambahkan nama file dari sourcePath
    if (isDirectory(destPath)) {
        strncpy(finalDestPath, destPath, MAX_PATH - 1);
        size_t len = strlen(finalDestPath);
        
        // Tambahkan backslash jika belum ada
        if (len > 0 && finalDestPath[len - 1] != '\\') {
            strncat(finalDestPath, "\\", MAX_PATH - len - 1);
        }
        
        // Tambahkan nama file dari sourcePath
        strncat(finalDestPath, getFileName(sourcePath), MAX_PATH - strlen(finalDestPath) - 1);
    } else {
        strncpy(finalDestPath, destPath, MAX_PATH - 1);
    }

    // Buka/buat file tujuan dalam mode binary write
    destFile = fopen(finalDestPath, "wb");
    if (destFile == NULL) {
        fclose(sourceFile);
        return -3;  // Gagal membuka/membuat file tujuan
    }

    // Salin file dalam chunks untuk menghemat memori
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0) {
        if (fwrite(buffer, 1, bytesRead, destFile) != bytesRead) {
            fclose(sourceFile);
            fclose(destFile);
            return -4;  // Error saat menulis
        }
    }

    fclose(sourceFile);
    fclose(destFile);

    return 0;
}
