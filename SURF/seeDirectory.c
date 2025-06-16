#include "seeDirectory.h"

void showMenu() {
    printf("\nPilihan Menu:\n");
    printf("1. Lihat isi direktori saat ini\n");
    printf("2. Tampilkan struktur direktori\n");
    printf("3. Copy file\n");
    printf("4. Lihat pwd saat ini\n");
    printf("5. Enkripsi/Dekripsi file\n");
    printf("6. Keluar\n");
    printf("Pilih menu (1-6): ");
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
                printf("\nIsi direktori saat ini:\n");
                rewinddir(dir);
                
                while ((entry = readdir(dir)) != NULL) {
                    printf("%s\n", entry->d_name);
                }
                printf("\nMasukkan nama direktori yang ingin dibuka (0 untuk menu): ");
                
                char dir_name[MAX_PATH];
                if (fgets(dir_name, sizeof(dir_name), stdin) != NULL) {
                    dir_name[strcspn(dir_name, "\n")] = 0;
                    
                    if (strcmp(dir_name, "0") == 0) {
                        continue;
                    } else if (strcmp(dir_name, "..") == 0) {
                        closedir(dir);
                        char *last_slash = strrchr(pwd, '\\');
                        if (last_slash != NULL && last_slash != pwd) {
                            *last_slash = '\0';
                            dir = opendir(pwd);
                        }
                    } else if (strcmp(dir_name, ".") != 0) {
                        closedir(dir);
                        
                        if (pwd[strlen(pwd)-1] != '\\') {
                            strncat(pwd, "\\", sizeof(pwd) - strlen(pwd) - 1);
                        }
                        
                        strncat(pwd, dir_name, sizeof(pwd) - strlen(pwd) - 1);
                        dir = opendir(pwd);
                        
                        if (dir == NULL) {
                            printf("Tidak dapat membuka direktori: %s\n", pwd);
                            char *last_slash = strrchr(pwd, '\\');
                            if (last_slash != NULL) {
                                *last_slash = '\0';
                            }
                            dir = opendir(pwd);
                            printf("Tekan Enter untuk melanjutkan...");
                            getchar();
                        }
                    }
                }
                break;

            case '2':
                printf("\nStruktur direktori saat ini:\n");
                showInorderFile(pwd);
                printf("\nTekan Enter untuk kembali ke menu...");
                getchar();
                break;

            case '3': {
                char source[MAX_PATH], dest[MAX_PATH];
                printf("Masukkan path file sumber: ");
                if (fgets(source, sizeof(source), stdin) != NULL) {
                    source[strcspn(source, "\n")] = 0;
                }

                printf("Masukkan path file tujuan: ");
                if (fgets(dest, sizeof(dest), stdin) != NULL) {
                    dest[strcspn(dest, "\n")] = 0;
                }

                int result = copyFile(source, dest);
                if (result == 0) {
                    printf("File berhasil disalin.\n");
                } else {
                    printf("Gagal menyalin file. Kode error: %d\n", result);
                }
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;
            }

            case '4':
                showPWD(pwd);
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
                break;

            case '5':
                handleEncryption(pwd);
                break;

            case '6':
                choice = 0;
                break;

            default:
                printf("Pilihan tidak valid!\n");
                printf("Tekan Enter untuk melanjutkan...");
                getchar();
        }
    } while (choice != 0);

    if (dir != NULL) {
        closedir(dir);
    }
}
