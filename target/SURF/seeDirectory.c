#include "seeDirectory.h"

void showMenu(void * paramT) {
	if(paramT == NULL) return;

	paramThread * param = (paramThread *)paramT;

	char buffer[128] = {0};

	strcpy(buffer, "\nPilihan Menu:\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "1. Lihat isi direktori saat ini\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "2. Tampilkan struktur direktori\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "3. Copy file\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "4. Copy direktori\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "5. Lihat pwd saat ini\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "6. Enkripsi/Dekripsi file\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);

	strcpy(buffer, "Input \"END\" untuk keluar dari SURF MODE\n");
	send((param)->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);
}

void showPWD(void * paramS) {
	if(param == NULL) return;

	paramSurf * param = (paramSurf *)paramS;

	char buffer[128] = {0};

	Sleep(101);
	strcpy(buffer, "Direktori kerja saat ini: %s\n");
	send((param)->paramT->clientSocket, buffer, strlen(buffer), 0);
	memset(buffer, 0, strlen(buffer));
	Sleep(101);
}

void handleEncryption(const char *pwd) {
    char password[256];
    char encrypted_dir[MAX_PATH];
    char decrypted_dir[MAX_PATH];
    char input[10];
    printf("\nPilih operasi:\n");
    printf("1. Enkripsi direktori\n");
    printf("2. Dekripsi direktori\n");
    printf("Pilih (1-2): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error membaca input\n");
        return;
    }
    input[strcspn(input, "\n")] = 0;
    printf("Masukkan kata sandi: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("Gagal membaca kata sandi\n");
        return;
    }
    password[strcspn(password, "\n")] = 0;
    if (input[0] == '1') {
        snprintf(encrypted_dir, sizeof(encrypted_dir), "%s_encrypted", pwd);
        printf("\nMengenkripsi direktori: %s\n", pwd);
        if (encrypt_folder(pwd, encrypted_dir, password) == 0) {
            printf("Direktori berhasil dienkripsi ke: %s\n", encrypted_dir);
        } else {
            printf("Enkripsi direktori gagal\n");
        }
    } else if (input[0] == '2') {
        snprintf(decrypted_dir, sizeof(decrypted_dir), "%s_decrypted", pwd);
        printf("\nMendekripsi direktori: %s\n", pwd);
        if (decrypt_folder(pwd, decrypted_dir, password) == 0) {
            printf("Direktori berhasil didekripsi ke: %s\n", decrypted_dir);
        } else {
            printf("Dekripsi direktori gagal\n");
        }
    } else {
        printf("Pilihan tidak valid\n");
    }
    printf("Tekan Enter untuk melanjutkan...");
    getchar();
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
		return;
	}

	int choice = 1;
	
	do {
		while(strcmp((param)->input, "END") == 0) Sleep(5000);
		showPWD(param);
		showMenu((void *)&(param)->paramT);

		
		// Bersihin newline
		(param)->input[strcspn((param)->input, "\n")] = 0;
		
		switch((param)->input[0]) {
			case '1':
				seeCurrentDirectory(dir, (param)->pwd);
				break;

			case '2':
				showInorderFile((param)->pwd);
				break;

			case '5':
				showPWD((param)->pwd);
				break;

			case '6':
				handleEncryption((param)->pwd);
				break;

			default:
				printf("Pilihan tidak valid!\n");
		}
	} while (choice != 0);

	if ((param)->dir != NULL) {
		closedir(dir);
	}

	return 0;
}
