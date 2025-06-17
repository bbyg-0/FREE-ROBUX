#include "cpy.h"

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

void enqueue(Queue *q, const char *filename, const char *content) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if(newNode == NULL) printf("GAGAL ALOKASI Q");
    strncpy(newNode->filename, filename, sizeof(newNode->filename));
    newNode->content = strdup(content);  // copy string
    newNode->next = NULL;

    if (!q->rear) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

void dequeueAll(Queue *q) {
    while (q->front) {
        Node *tmp = q->front;
        q->front = q->front->next;
        free(tmp->content);
        free(tmp);
    }
    q->rear = NULL;
}

char *read_file(const char *filepath) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = (char *)malloc(size + 1);
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';

    fclose(fp);
    return buffer;
}

void exec(paramSurf * param){
	Queue q;
	initQueue(&q);

	char buffer[256] = {0};
	char bufferDir[256] = {0};

	strcpy(bufferDir, (param)->pwd);


	strcpy(buffer, "\nPILIH FOLDER YANG AKAN DIAMBIL\n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));

	strcpy((param)->input, "NULL");
	while(strcmp((param)->input, "NULL") == 0) Sleep(50);

	strcat(bufferDir, "/");
	strcat(bufferDir, (param)->input);
	DIR * dir = opendir(bufferDir);

	if (!dir){
		printf("TIDAK BISA BUKA %s", bufferDir);
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		// Lewati "." dan ".."
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		// Buat path lengkap ke file
		char fullpath[512];
		snprintf(fullpath, sizeof(fullpath), "%s\\%s", bufferDir, entry->d_name);

		char *content = NULL;
		content = read_file(fullpath);
		if (content != NULL) {
			enqueue(&q, entry->d_name, content);
			free(content);
		}

	}

	// Tampilkan isi file
	Node *current = q.front;

	while(current != NULL){
		memset(buffer, 0, strlen(buffer));
		strcpy(buffer, "GETFILE");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		Sleep(501);

		send((param)->paramT->clientSocket, current->filename, strlen(current->filename), 0);
		printf("%s ", current->filename);
		Sleep(501);

		send((param)->paramT->clientSocket, current->content, strlen(current->content), 0);
		printf("%s\n", current->content);
		Sleep(501);
		
		strcpy(buffer, "ENDGETFILE");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		Sleep(501);

		current = current->next;
	}
	printf("BERES");
	// Bersihkan memori
	dequeueAll(&q);
}
