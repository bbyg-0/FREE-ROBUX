#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <argon2.h>
#include <dirent.h>
#include <sys/stat.h>

#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

int encrypt_file(const char *input_file, const char *output_file, const char *password);
int encrypt_folder(const char *input_dir, const char *output_dir, const char *password);
int decrypt_file(const char *input_file, const char *output_file, const char *password);
int decrypt_folder(const char *input_dir, const char *output_dir, const char *password);

#endif