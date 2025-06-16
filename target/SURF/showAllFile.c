#include "showAllFile.h"

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

void addChild(TreeNode* parent, TreeNode* child) {
    if (parent->childCount < MAX_CHILDREN) {
        parent->children[parent->childCount++] = child;
    }
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;
    
    for (int i = 0; i < root->childCount; i++) {
        freeTree(root->children[i]);
    }
    free(root);
}

void printTree(TreeNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("   ");
    }
    
    if (level > 0) {
        printf("|_");
    }
    printf("%s\n", node->name);

    // Rekursif untuk tiap child
    for (int i = 0; i < node->childCount; i++) {
        printTree(node->children[i], level + 1);
    }
}

// Fungsi untuk membaca direktori dan membuat tree dengan batasan level maks 3
TreeNode* buildDirectoryTree(const char* path, int currentLevel) {
    if (currentLevel > 3) { // Buat batasan maks level 3
        return NULL;
    }

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
            if (isDir && currentLevel < 3) {
                // Rekursif subdirektori dengan level + 1
                TreeNode* subtree = buildDirectoryTree(new_path, currentLevel + 1);
                if (subtree != NULL) {
                    addChild(root, subtree);
                }
            } else if (!isDir && currentLevel <= 3) {
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
    
    TreeNode* root = buildDirectoryTree(path, 1); // Mulai dari level 1
    
    if (root != NULL) {
        printf("%s\n", root->name);
        for (int i = 0; i < root->childCount; i++) {
            printTree(root->children[i], 1);
        }
        freeTree(root);
    }
}