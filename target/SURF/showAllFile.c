#include "showAllFile.h"
#include "seeDirectory.h"

#include <string.h>

// Fungsi untuk membuat node baru
TreeNode *createNode(const char *name, int isDirectory)
{
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (newNode == NULL)
    {
        return NULL;
    }
    strncpy(newNode->name, name, MAX_NAME - 1);
    newNode->name[MAX_NAME - 1] = '\0';
    newNode->isDirectory = isDirectory;
    newNode->childCount = 0;
    return newNode;
}

void addChild(TreeNode *parent, TreeNode *child)
{
    if (parent->childCount < MAX_CHILDREN)
    {
        parent->children[parent->childCount++] = child;
    }
}

void freeTree(TreeNode *root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < root->childCount; i++)
    {
        freeTree(root->children[i]);
    }
    free(root);
}

void printTree(TreeNode *node, int level, void * paramS)
{
    if (node == NULL)
        return;

    paramSurf * param = (paramSurf *)paramS;
    char buffer[128] = {0};

    for (int i = 0; i < level; i++){
        strcpy(buffer, "   ");
        send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, strlen(buffer));
    }

    if (level > 0)
    {
        strcpy(buffer, "|_");
        send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, strlen(buffer));
    }
    sprintf(buffer, "%s\n", node->name);
    send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
    memset(buffer, 0, strlen(buffer));

    // Rekursif untuk tiap child
    for (int i = 0; i < node->childCount; i++)
    {
        printTree(node->children[i], level + 1, param);
    }
}

// Fungsi untuk membaca direktori dan membuat tree dengan batasan level maks 3
TreeNode *buildDirectoryTree(const char *path, int currentLevel)
{
    if (currentLevel > 3)
    { // Buat batasan maks level 3
        return NULL;
    }

    DIR *dir;
    struct dirent *entry;
    char new_path[MAX_PATH];

    // Dapatkan nama direktori dari path
    const char *dirName = strrchr(path, '\\');
    if (dirName == NULL)
    {
        dirName = path;
    }
    else
    {
        dirName++; // Skip backslash
    }

    TreeNode *root = createNode(dirName, 1);
    if (root == NULL)
        return NULL;

    dir = opendir(path);
    if (dir == NULL)
    {
        return root;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(new_path, MAX_PATH, "%s\\%s", path, entry->d_name);
        DWORD fileAttributes = GetFileAttributesA(new_path);

        int isDir = (fileAttributes != INVALID_FILE_ATTRIBUTES &&
                     (fileAttributes & FILE_ATTRIBUTE_DIRECTORY));

        TreeNode *child = createNode(entry->d_name, isDir);
        if (child != NULL)
        {
            if (isDir && currentLevel < 3)
            {
                // Rekursif subdirektori dengan level + 1
                TreeNode *subtree = buildDirectoryTree(new_path, currentLevel + 1);
                if (subtree != NULL)
                {
                    addChild(root, subtree);
                }
            }
            else if (!isDir && currentLevel <= 3)
            {
                // Tambahkan file sebagai leaf node
                addChild(root, child);
            }
        }
    }

    closedir(dir);
    return root;
}

void showInorderFile(void * paramS){
    paramSurf * param = (paramSurf *)paramS;

    char buffer[512] = {0};

    strcpy(buffer, "\nStruktur direktori saat ini:\n");
    send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
    memset(buffer, 0, strlen(buffer));

    char path[MAX_PATH];
    _fullpath(path, (param)->pwd, MAX_PATH);

    TreeNode *root = buildDirectoryTree(path, 1); // Mulai dari level 1

    if (root != NULL)
    {
        sprintf(buffer, "%s\n", root->name);
        send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, strlen(buffer));

        for (int i = 0; i < root->childCount; i++)
        {
            printTree(root->children[i], 1, param);
        }
        freeTree(root);
    }
}

void seeCurrentDirectory(void * paramS) {
	paramSurf * param = (paramSurf *) paramS;
    struct dirent *entry;

    char buffer[2048] = {0};

	if ((param)->dir == NULL) {
        strcpy(buffer, "Tidak dapat membuka direktori.\n");
        send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, strlen(buffer));
        strcpy((param)->input, "END");
		return;
	}

    sprintf(buffer,"Direktori saat ini: %s\n", (param)->pwd);
    send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
    memset(buffer, 0, strlen(buffer));
    Sleep(101);

	strcpy(buffer, "\nIsi direktori saat ini:\n");
    send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
    memset(buffer, 0, strlen(buffer));
    Sleep(101);

	rewinddir((param)->dir);

	while ((entry = readdir((param)->dir)) != NULL) {
		sprintf(buffer, "%s\n", entry->d_name);
        send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, strlen(buffer));
        Sleep(101);
	}
	strcpy(buffer, "\nMasukkan nama direktori yang ingin dibuka (0 untuk menu): ");
    send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
    memset(buffer, 0, strlen(buffer));
    Sleep(101);

    strcpy((param)->input, "NULL");
    while(strcmp((param)->input, "NULL")==0) Sleep(500);
	if (strcmp((param)->input, "NULL") != 0) {
        strcpy((param)->dir_name, (param)->input);
        strcpy((param)->input, "1");

		(param)->dir_name[strcspn((param)->dir_name, "\n")] = 0;

		if (strcmp((param)->dir_name, "0") == 0) {
            strcpy((param)->input, "END");
			return;
		}
		else if (strcmp((param)->dir_name, "..") == 0) {
			closedir((param)->dir);
			char *last_slash = strrchr((param)->pwd, '\\');
			if (last_slash != NULL && last_slash != (param)->pwd) {
				*last_slash = '\0';
				(param)->dir = opendir((param)->pwd);
			}
		}
		else if (strcmp((param)->dir_name, ".") != 0) {
			closedir((param)->dir);

			if ((param)->pwd[strlen((param)->pwd) - 1] != '\\') {
				strncat((param)->pwd, "\\", sizeof((param)->pwd) - strlen((param)->pwd) - 1);
			}

			strncat((param)->pwd, (param)->dir_name, sizeof((param)->pwd) - strlen((param)->pwd) - 1);
			(param)->dir = opendir((param)->pwd);

			if ((param)->dir == NULL) {
                sprintf(buffer, "Tidak dapat membuka direktori: %s\n", (param)->pwd);
                send(((param)->paramT)->clientSocket, buffer, strlen(buffer), 0);
                memset(buffer, 0, strlen(buffer));
                Sleep(101);
				char *last_slash = strrchr((param)->pwd, '\\');
				if (last_slash != NULL) {
					*last_slash = '\0';
				}
				(param)->dir = opendir((param)->pwd);
			}
		}
	}
}

void inputSourceDest(char *source, char *dest)
{
    printf("Masukkan path file sumber: ");
    if (fgets(source, MAX_PATH, stdin) != NULL)
    {
        source[strcspn(source, "\n")] = 0; // Hapus newline
    }

    printf("Masukkan path file tujuan: ");
    if (fgets(dest, MAX_PATH, stdin) != NULL)
    {
        dest[strcspn(dest, "\n")] = 0; // Hapus newline
    }
}
