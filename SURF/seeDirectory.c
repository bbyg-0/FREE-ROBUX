#include "seeDirectory.h"

void seeDirectory() {
    DIR *dir;
    struct dirent *entry;

    chdir("C:\\"); // Set direktori kerja ke C:

    // Buka direktori root C:
    char path[MAX_PATH];
    _fullpath(path, "C:\\", MAX_PATH);
    dir = opendir(path);

    int choice = 1;
    
    do {
        printf("Pilih direktori yang ingin dibuka:\n");
        // Periksa apakah direktori berhasil dibuka
        if (dir == NULL) {
            printf("Tidak dapat membuka direktori.\n");
            return;
        }

        // Baca dan cetak nama entri direktori
        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name);
        }
        printf("\n");

        printf("Masukkan nama direktori yang ingin dibuka (0 untuk keluar): ");
        char dir_name[MAX_PATH];

        fgets(dir_name, sizeof(dir_name), stdin); // Baca input dari pengguna
        dir_name[strcspn(dir_name, "\n")] = 0;

        if (strcmp(dir_name, "0") == 0) {
            choice = 0;
        } else if (strcmp(dir_name, "..") == 0) {
            closedir(dir);
            // Kembali ke direktori sebelumnya
            char *last_slash = strrchr(path, '\\');

            if (last_slash != NULL) {
                *last_slash = '\0'; // Hapus karakter '/' di akhir
            }
            printf("%s\n", path);

            dir = opendir(path);
        } else if (strcmp(dir_name, ".") == 0) {
            // Tetap di direktori yang sama
            dir = opendir(path);
        } else {
            closedir(dir);
            
            // Tambahkan backslash jika belum ada
            if (path[strlen(path)-1] != '\\') {
                strncat(path, "\\", sizeof(path) - strlen(path) - 1);
            }
            
            // Buka direktori yang dipilih
            strncat(path, dir_name, sizeof(path) - strlen(path) - 1);
            dir = opendir(path);
            
            if (dir == NULL) {
                printf("Tidak dapat membuka direktori: %s\n", path);
                // Kembalikan path ke keadaan sebelumnya
                char *last_slash = strrchr(path, '\\');
                if (last_slash != NULL) {
                    *last_slash = '\0';
                }
                dir = opendir(path);
            } else {
                printf("Membuka direktori: %s\n", path);
            }
        }
    } while (choice != 0);

    // Tutup direktori
    closedir(dir);
}
