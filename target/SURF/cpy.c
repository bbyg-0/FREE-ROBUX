#include "cpy.h"

void DeAlokasi (address *p){
	if(p == NULL || (*p) == NULL) return;

	free((*p)->file_name);	
	free((*p)->content);	
	free(*p);
	(*p)->file_name = NULL;
	(*p)->content = NULL;
	(*p) = NULL;
}

void Create_Q(address *p){
	*p = (address)malloc(sizeof(qList));
	if(isEmpty(*p)){
		printf("Memori gagal dialokasi");return;
	}
}

void Isi_Q (address *p, char * judul, char * isi){
	if(isEmpty(*p)) return;

	(*p)->file_name = myStrdup(judul);
	(*p)->content = myStrdup(isi);
}

void enqueue (address *p, address *pNew){
	if(isEmpty(*p)){
		(*p) = *pNew;
		return;
	}	
	address temp = (*p);
	while(!isEmpty((*p)->next)){
		(*p) = (*p)->next;
	}
	(*p)->next = (*pNew);
	
	(*p) = temp;
}

void getValue (address *p, char * name, char * isi){
	if(isEmpty(*p)) return;
	
	(name) = myStrdup((*p)->file_name);
	(isi) = myStrdup((*p)->content);
}

void dequeue (address *p){
	if(isEmpty(*p)) return;
	
	address temp = (*p)->next;
	DeAlokasi(p);
	(*p) = temp;

	if(isEmpty(*p)) printf("List telah kosong");
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
	address q;
	address node;

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
			Create_Q(&node);
			Isi_Q(&node, entry->d_name, content);
			free(content);
			enqueue(&q, &node);
		}
	}

	memset(buffer, 0, strlen(buffer));
	strcpy(buffer, "MAKEFOLDER");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	Sleep(101);

	memset(buffer, 0, strlen(buffer));
	strcpy(buffer, (param)->input);
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	Sleep(101);

	char * NAMA; char * ISI;

	while(q != NULL){
		printf("YADAYADA\n");
		memset(buffer, 0, strlen(buffer));
		strcpy(buffer, "GETFILE2");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));

		getValue(&q, NAMA, ISI);
		Sleep(101);
		printf("\nGET: %s\n", NAMA);
		send((param)->paramT->clientSocket, NAMA, strlen(NAMA), 0);
		Sleep(101);

		send((param)->paramT->clientSocket, ISI, strlen(ISI), 0);
		Sleep(101);
		
		strcpy(buffer, "ENDGETFILE");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));

		dequeue(&q);
		Sleep(101);
	}
}
