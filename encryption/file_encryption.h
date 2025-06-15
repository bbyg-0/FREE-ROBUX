#ifndef FILE_ENCRYPTION_H
#define FILE_ENCRYPTION_H

int encrypt_file(const char *input_file, const char *output_file, const char *password);
int encrypt_folder(const char *input_dir, const char *output_dir, const char *password);
int decrypt_file(const char *input_file, const char *output_file, const char *password);
int decrypt_folder(const char *input_dir, const char *output_dir, const char *password);

#endif