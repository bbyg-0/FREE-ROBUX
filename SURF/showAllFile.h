#ifndef _SHOWALLFILE_H
#define _SHOWALLFILE_H

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
#define MAX_NAME 256
#define MAX_CHILDREN 100

// Struktur untuk node Tree
typedef struct TreeNode {
    char name[MAX_NAME];               // Nama file atau direktori
    int isDirectory;                   // Penanda apakah ini direktori
    struct TreeNode* children[MAX_CHILDREN];  // Array dari child nodes
    int childCount;                    // Jumlah child
} TreeNode;

// Function declarations
TreeNode* createNode(const char* name, int isDirectory);
void addChild(TreeNode* parent, TreeNode* child);
void freeTree(TreeNode* root);
void printTree(TreeNode* node, int level);
void showInorderFile(const char* currentPath);  // Mengubah deklarasi untuk menerima parameter

#endif