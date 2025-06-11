#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <argon2.h>
#include "fileEncryption.h"

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

// Function to encrypt a file
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