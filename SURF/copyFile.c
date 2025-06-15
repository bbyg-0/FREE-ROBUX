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
void createQueue(FileQueue* q) {
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
        return fileName + 1; // Lompat backslash
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
        printf("Source bukan direktori: %s\n", sourceDir);
        return -1;
    }

    // Buka direktori sumber
    dir = opendir(sourceDir);
    if (dir == NULL) {
        printf("Error saat membuka direktori sumber: %s\n", strerror(errno));
        return -2;
    }

    // Buat direktori tujuan jika belum ada
    if (_mkdir(destDir) != 0 && errno != EEXIST) {
        closedir(dir);
        printf("Error saat membuat direktori tujuan: %s\n", strerror(errno));
        return -3;
    }

    // Buat queue
    createQueue(&fileQueue);

    // Kumpulkan semua file dalam queue
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
            // Buat direktori baru dan lakukan rekursif
            status = copyDirectory(sourcePath, destPath);
            if (status < 0) {
                closedir(dir);
                clearQueue(&fileQueue);
                printf("Error saat menyalin subdirektori: %s\n", strerror(errno));
                return -4;
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
            printf("Error saat menyalin file: %s\n", strerror(errno));
            return -5;
        }
    }

    return 0;
}

int copyFile(const char *sourcePath, const char *destPath) {
    FILE *sourceFile, *destFile;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    char finalDestPath[MAX_PATH];

    // Periksa apakah sumber adalah file
    if (!isFile(sourcePath)) {
        printf("Sumber bukan file: %s\n", sourcePath);
        return -1;
    }

    // Buka file sumber
    sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL) {
        printf("Error saat membuka file sumber: %s\n", strerror(errno));
        return -2;
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

    // Buka file tujuan
    destFile = fopen(finalDestPath, "wb");
    if (destFile == NULL) {
        fclose(sourceFile);
        printf("Error saat membuka file tujuan: %s\n", strerror(errno));
        return -3;
    }
    // Salin konten dari file sumber ke file tujuan
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0) {
        if (fwrite(buffer, 1, bytesRead, destFile) != bytesRead) {
            fclose(sourceFile);
            fclose(destFile);
            printf("Error saat menulis ke file tujuan: %s\n", strerror(errno));
            return -4;
        }
    }

    fclose(sourceFile);
    fclose(destFile);

    return 0;
}
