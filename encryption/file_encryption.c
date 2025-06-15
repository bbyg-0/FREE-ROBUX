#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <argon2.h>
#include <dirent.h>
#include <sys/stat.h>
#include "file_encryption.h"

#define SALT_LEN 16
#define KEY_LEN 32 // AES-256 requires 32-byte key
#define IV_LEN 16  // AES-CBC IV length
#define HASHLEN 32 // Argon2 output length
#define BLOCK_SIZE 16 // AES block size

// Structure to hold encryption metadata
typedef struct {
    unsigned char salt[SALT_LEN];
    unsigned char iv[IV_LEN];
} EncryptionMetadata;

// Function to derive key using Argon2
int derive_key(const char *password, unsigned char *salt, unsigned char *key) {
    uint32_t t_cost = 2;  // Iterations
    uint32_t m_cost = 1<<16; // Memory cost (64MB)
    uint32_t parallelism = 4; // Threads

    return argon2id_hash_raw(t_cost, m_cost, parallelism, password, strlen(password),
                             salt, SALT_LEN, key, KEY_LEN);
}

// Function to encrypt a single file
int encrypt_file(const char *input_file, const char *output_file, const char *password) {
    FILE *in_fp = fopen(input_file, "rb");
    FILE *out_fp = fopen(output_file, "wb");
    if (!in_fp || !out_fp) {
        perror("File open error");
        return 1;
    }

    // Generate random salt and IV
    unsigned char salt[SALT_LEN], iv[IV_LEN], key[KEY_LEN];
    if (RAND_bytes(salt, SALT_LEN) != 1 || RAND_bytes(iv, IV_LEN) != 1) {
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    // Derive key using Argon2
    if (derive_key(password, salt, key) != ARGON2_OK) {
        fprintf(stderr, "Key derivation failed\n");
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    // Write salt and IV to output file
    EncryptionMetadata metadata = {0};
    memcpy(metadata.salt, salt, SALT_LEN);
    memcpy(metadata.iv, iv, IV_LEN);
    fwrite(&metadata, sizeof(metadata), 1, out_fp);

    // Initialize OpenSSL cipher context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    // Buffer for reading/writing
    unsigned char in_buf[1024], out_buf[1024 + BLOCK_SIZE];
    int in_len, out_len;

    // Encrypt file content
    while ((in_len = fread(in_buf, 1, sizeof(in_buf), in_fp)) > 0) {
        if (EVP_EncryptUpdate(ctx, out_buf, &out_len, in_buf, in_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            fclose(in_fp); fclose(out_fp);
            return 1;
        }
        fwrite(out_buf, 1, out_len, out_fp);
    }

    // Finalize encryption
    if (EVP_EncryptFinal_ex(ctx, out_buf, &out_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        fclose(in_fp); fclose(out_fp);
        return 1;
    }
    fwrite(out_buf, 1, out_len, out_fp);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    fclose(in_fp);
    fclose(out_fp);
    OPENSSL_cleanse(key, KEY_LEN); // Securely erase key
    return 0;
}

// Function to decrypt a single file
int decrypt_file(const char *input_file, const char *output_file, const char *password) {
    FILE *in_fp = fopen(input_file, "rb");
    FILE *out_fp = fopen(output_file, "wb");
    if (!in_fp || !out_fp) {
        perror("File open error");
        return 1;
    }

    // Read salt and IV from input file
    EncryptionMetadata metadata;
    if (fread(&metadata, sizeof(metadata), 1, in_fp) != 1) {
        fprintf(stderr, "Failed to read metadata\n");
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    // Derive key using Argon2
    unsigned char key[KEY_LEN];
    if (derive_key(password, metadata.salt, key) != ARGON2_OK) {
        fprintf(stderr, "Key derivation failed\n");
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    // Initialize OpenSSL cipher context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, metadata.iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        fclose(in_fp); fclose(out_fp);
        return 1;
    }

    // Buffer for reading/writing
    unsigned char in_buf[1024], out_buf[1024 + BLOCK_SIZE];
    int in_len, out_len;

    // Decrypt file content
    while ((in_len = fread(in_buf, 1, sizeof(in_buf), in_fp)) > 0) {
        if (EVP_DecryptUpdate(ctx, out_buf, &out_len, in_buf, in_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            fclose(in_fp); fclose(out_fp);
            return 1;
        }
        fwrite(out_buf, 1, out_len, out_fp);
    }

    // Finalize decryption
    if (EVP_DecryptFinal_ex(ctx, out_buf, &out_len) != 1) {
        fprintf(stderr, "Decryption failed (wrong password?)\n");
        EVP_CIPHER_CTX_free(ctx);
        fclose(in_fp); fclose(out_fp);
        return 1;
    }
    fwrite(out_buf, 1, out_len, out_fp);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    fclose(in_fp);
    fclose(out_fp);
    OPENSSL_cleanse(key, KEY_LEN); // Securely erase key
    return 0;
}

// Function to create a directory if it doesn't exist
int create_directory(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
#ifdef _WIN32
        return mkdir(path);
#else
        return mkdir(path, 0755);
#endif
    }
    return 0;
}

// Function to encrypt a folder recursively
int encrypt_folder(const char *input_dir, const char *output_dir, const char *password) {
    DIR *dir = opendir(input_dir);
    if (!dir) {
        perror("Cannot open directory");
        return 1;
    }

    // Create output directory
    if (create_directory(output_dir) != 0) {
        fprintf(stderr, "Failed to create output directory: %s\n", output_dir);
        closedir(dir);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full input and output paths
        char input_path[1024], output_path[1024];
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, entry->d_name);
        snprintf(output_path, sizeof(output_path), "%s/%s.enc", output_dir, entry->d_name);

        // Check if it's a directory
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Cannot stat %s\n", input_path);
            continue;
        }

        if (S_ISDIR(path_stat.st_mode)) {
            // Recursively encrypt subdirectory
            char new_output_dir[1024];
            snprintf(new_output_dir, sizeof(new_output_dir), "%s/%s", output_dir, entry->d_name);
            if (encrypt_folder(input_path, new_output_dir, password) != 0) {
                closedir(dir);
                return 1;
            }
        } else if (S_ISREG(path_stat.st_mode)) {
            // Encrypt regular file
            if (encrypt_file(input_path, output_path, password) != 0) {
                fprintf(stderr, "Failed to encrypt %s\n", input_path);
                closedir(dir);
                return 1;
            }
            printf("Encrypted: %s -> %s\n", input_path, output_path);
        }
    }

    closedir(dir);
    return 0;
}

// Function to decrypt a folder recursively
int decrypt_folder(const char *input_dir, const char *output_dir, const char *password) {
    DIR *dir = opendir(input_dir);
    if (!dir) {
        perror("Cannot open directory");
        return 1;
    }

    // Create output directory
    if (create_directory(output_dir) != 0) {
        fprintf(stderr, "Failed to create output directory: %s\n", output_dir);
        closedir(dir);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full input path
        char input_path[1024];
        snprintf(input_path, sizeof(input_path), "%s/%s", input_dir, entry->d_name);

        // Check if it's a directory
        struct stat path_stat;
        if (stat(input_path, &path_stat) != 0) {
            fprintf(stderr, "Cannot stat %s\n", input_path);
            continue;
        }

        if (S_ISDIR(path_stat.st_mode)) {
            // Recursively decrypt subdirectory
            char new_output_dir[1024];
            snprintf(new_output_dir, sizeof(new_output_dir), "%s/%s", output_dir, entry->d_name);
            if (decrypt_folder(input_path, new_output_dir, password) != 0) {
                closedir(dir);
                return 1;
            }
        } else if (S_ISREG(path_stat.st_mode)) {
            // Check if file has .enc extension
            size_t len = strlen(entry->d_name);
            if (len > 4 && strcmp(entry->d_name + len - 4, ".enc") == 0) {
                // Construct output path without .enc
                char output_path[1024];
                snprintf(output_path, sizeof(output_path), "%s/%.*s", output_dir, (int)(len - 4), entry->d_name);

                // Decrypt file
                if (decrypt_file(input_path, output_path, password) != 0) {
                    fprintf(stderr, "Failed to decrypt %s\n", input_path);
                    closedir(dir);
                    return 1;
                }
                printf("Decrypted: %s -> %s\n", input_path, output_path);
            }
        }
    }

    closedir(dir);
    return 0;
}