#include "seeDirectory.h"

void showMenu() {
    printf("\nPilihan Menu:\n");
    printf("1. Lihat isi direktori saat ini\n");
    printf("2. Tampilkan struktur direktori\n");
    printf("3. Copy file\n");
    printf("4. Copy direktori\n");
    printf("5. Lihat pwd saat ini\n");
    printf("6. Enkripsi/Dekripsi file\n");
    printf("7. Keluar\n");
    printf("Pilih menu (1-7): ");
}

void showPWD(const char *pwd) {
    printf("Direktori kerja saat ini: %s\n", pwd);
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

void surfMode() {
    DIR *dir;
    struct dirent *entry;
    char pwd[MAX_PATH];
    char input[10];
    char source[MAX_PATH], dest[MAX_PATH];

    // Set direktori kerja ke C:
    chdir("C:\\");
    _fullpath(pwd, "C:\\", MAX_PATH);
    
    dir = opendir(pwd);
    if (dir == NULL) {
        printf("Tidak dapat membuka direktori.\n");
        return;
    }

    int choice = 1;
    
    do {
        system("cls");
        showPWD(pwd);
        showMenu();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error membaca input\n");
            continue;
        }
        
        // Bersihin newline
        input[strcspn(input, "\n")] = 0;
        
        switch(input[0]) {
            case '1':
                seeCurrentDirectory(dir, pwd);
                backToMenu();
                break;

            case '2':
                showInorderFile(pwd);
                backToMenu();
                break;

            case '3': {
                inputSourceDest(source, dest);
                int result = copyFile(source, dest);
                backToMenu();
                break;
            }

            case '4':
                inputSourceDest(source, dest);
                int result = copyDirectory(source, dest);
                backToMenu();
                break;

            case '5':
                showPWD(pwd);
                backToMenu();
                break;

            case '6':
                handleEncryption(pwd);
                backToMenu();
                break;

            case '7':
                choice = 0;
                break;

            default:
                printf("Pilihan tidak valid!\n");
                backToMenu();
        }
    } while (choice != 0);

    if (dir != NULL) {
        closedir(dir);
    }
}
