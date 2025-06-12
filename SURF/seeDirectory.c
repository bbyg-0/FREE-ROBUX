#include "seeDirectory.h"
#include "showAllFile.h"

void showMenu() {
    printf("\nPilihan Menu:\n");
    printf("1. Lihat isi direktori saat ini\n");
    printf("2. Tampilkan struktur direktori\n");
    printf("3. Keluar\n");
    printf("Pilih menu (1-3): ");
}

void seeDirectory() {
    DIR *dir;
    struct dirent *entry;
    char path[MAX_PATH];
    char input[10];

    // Set direktori kerja ke C:
    chdir("C:\\");
    _fullpath(path, "C:\\", MAX_PATH);
    
    dir = opendir(path);
    if (dir == NULL) {
        printf("Tidak dapat membuka direktori.\n");
        return;
    }

    int choice = 1;
    
    do {
        system("cls"); // Bersihkan layar
        printf("Direktori saat ini: %s\n", path);
        showMenu();

        // Baca input dengan fgets untuk menghindari masalah buffer
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error membaca input\n");
            continue;
        }
        
        // Bersihkan newline
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
                        // Kembali ke menu
                        continue;
                    } else if (strcmp(dir_name, "..") == 0) {
                        closedir(dir);
                        char *last_slash = strrchr(path, '\\');
                        if (last_slash != NULL && last_slash != path) {
                            *last_slash = '\0';
                            dir = opendir(path);
                        }
                    } else if (strcmp(dir_name, ".") != 0) {
                        closedir(dir);
                        
                        if (path[strlen(path)-1] != '\\') {
                            strncat(path, "\\", sizeof(path) - strlen(path) - 1);
                        }
                        
                        strncat(path, dir_name, sizeof(path) - strlen(path) - 1);
                        dir = opendir(path);
                        
                        if (dir == NULL) {
                            printf("Tidak dapat membuka direktori: %s\n", path);
                            char *last_slash = strrchr(path, '\\');
                            if (last_slash != NULL) {
                                *last_slash = '\0';
                            }
                            dir = opendir(path);
                            printf("Tekan Enter untuk melanjutkan...");
                            getchar();
                        }
                    }
                }
                break;

            case '2':
                printf("\nStruktur direktori saat ini:\n");
                showInorderFile(path);
                printf("\nTekan Enter untuk kembali ke menu...");
                getchar();
                break;

            case '3':
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
