#ifndef _SHOWALLFILE_H
#define _SHOWALLFILE_H

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <windows.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif
#ifdef MAX_PATH
    #undef MAX_PATH
#endif
#define MAX_PATH 512

#define MAX_NAME 256
#define MAX_CHILDREN 100

// Struktur untuk node Tree
typedef struct TreeNode {
    char name[MAX_NAME];
    int isDirectory;
    struct TreeNode* children[MAX_CHILDREN];
    int childCount;
} TreeNode;

// Fungsi untuk Tree
TreeNode* createNode(const char* name, int isDirectory);
void addChild(TreeNode* parent, TreeNode* child);
void freeTree(TreeNode* root);
void printTree(TreeNode* node, int level, void * paramS);
TreeNode* buildDirectoryTree(const char* path, int currentLevel);
void showInorderFile(void * param);

void seeCurrentDirectory(void * paramS);
void inputSourceDest(char *source, char *dest);

#endif