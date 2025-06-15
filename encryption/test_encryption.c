#include <stdio.h>
#include <string.h>
#include "file_encryption.h"

int main() {
    char input_dir[1024];
    char password[256];
    char encrypted_dir[1024];
    char decrypted_dir[1024];

    // Get directory path
    printf("Enter directory to encrypt (e.g., test_dir): ");
    if (fgets(input_dir, sizeof(input_dir), stdin) == NULL) {
        printf("Failed to read directory.\n");
        return 1;
    }
    size_t len = strlen(input_dir);
    if (len > 0 && input_dir[len - 1] == '\n') {
        input_dir[len - 1] = '\0';
    }

    // Get password
    printf("Enter password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("Failed to read password.\n");
        return 1;
    }
    len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }

    // Set output directories
    snprintf(encrypted_dir, sizeof(encrypted_dir), "%s_encrypted", input_dir);
    snprintf(decrypted_dir, sizeof(decrypted_dir), "%s_decrypted", input_dir);

    // Encrypt the folder
    printf("\nEncrypting folder: %s\n", input_dir);
    if (encrypt_folder(input_dir, encrypted_dir, password) == 0) {
        printf("Folder encrypted successfully to: %s\n", encrypted_dir);
    } else {
        printf("Folder encryption failed.\n");
        return 1;
    }

    // Decrypt the folder
    printf("\nDecrypting folder: %s\n", encrypted_dir);
    if (decrypt_folder(encrypted_dir, decrypted_dir, password) == 0) {
        printf("Folder decrypted successfully to: %s\n", decrypted_dir);
    } else {
        printf("Folder decryption failed.\n");
        return 1;
    }

    return 0;
}