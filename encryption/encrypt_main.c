#include "file_encryption.h"
#include <stdio.h>

int main() {
    const char *input = "input.txt";
    const char *output = "output.enc";
    const char *password = "my_secure_password";

    if (encrypt_file(input, output, password) == 0) {
        printf("File encrypted successfully!\n");
    } else {
        printf("Encryption failed.\n");
    }
    return 0;
}