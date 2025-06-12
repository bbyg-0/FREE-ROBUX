#include "showAllFile.h"
#include <windows.h>
#include <stdlib.h>

// Fungsi untuk membuat node baru
TreeNode* createNode(const char* name, int isDirectory) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        return NULL;
    }
    strncpy(newNode->name, name, MAX_NAME - 1);
    newNode->name[MAX_NAME - 1] = '\0';
    newNode->isDirectory = isDirectory;
    newNode->childCount = 0;
    return newNode;
}

// Fungsi untuk menambahkan child ke parent node
void addChild(TreeNode* parent, TreeNode* child) {
    if (parent->childCount < MAX_CHILDREN) {
        parent->children[parent->childCount++] = child;
    }
}

// Fungsi untuk membebaskan memori tree
void freeTree(TreeNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i < root->childCount; i++) {
        freeTree(root->children[i]);
    }
    free(root);
}

// Fungsi untuk mencetak tree
void printTree(TreeNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    
    if (level > 0) {
        printf("|_");
    }
    printf("%s\n", node->name);

    // Rekursif untuk setiap child
    for (int i = 0; i < node->childCount; i++) {
        printTree(node->children[i], level + 1);
    }
}

// Fungsi untuk membaca direktori dan membuat tree
TreeNode* buildDirectoryTree(const char* path) {
    DIR *dir;
    struct dirent *entry;
    char new_path[MAX_PATH];
    
    // Dapatkan nama direktori dari path
    const char* dirName = strrchr(path, '\\');
    if (dirName == NULL) {
        dirName = path;
    } else {
        dirName++; // Skip backslash
    }

    TreeNode* root = createNode(dirName, 1);
    if (root == NULL) return NULL;

    dir = opendir(path);
    if (dir == NULL) {
        return root;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(new_path, MAX_PATH, "%s\\%s", path, entry->d_name);
        DWORD fileAttributes = GetFileAttributesA(new_path);
        
        int isDir = (fileAttributes != INVALID_FILE_ATTRIBUTES && 
                    (fileAttributes & FILE_ATTRIBUTE_DIRECTORY));

        TreeNode* child = createNode(entry->d_name, isDir);
        if (child != NULL) {
            if (isDir) {
                // Rekursif untuk subdirektori
                TreeNode* subtree = buildDirectoryTree(new_path);
                if (subtree != NULL) {
                    addChild(root, subtree);
                }
            } else {
                // Tambahkan file sebagai leaf node
                addChild(root, child);
            }
        }
    }

    closedir(dir);
    return root;
}

void showInorderFile(const char* currentPath) {
    char path[MAX_PATH];
    _fullpath(path, currentPath, MAX_PATH);
    
    TreeNode* root = buildDirectoryTree(path);
    
    if (root != NULL) {
        printf("%s\n", root->name);
        for (int i = 0; i < root->childCount; i++) {
            printTree(root->children[i], 1);
        }
        freeTree(root);
    }
}