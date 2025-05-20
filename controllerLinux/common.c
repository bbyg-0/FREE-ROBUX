#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


bool isEmpty (void * node){
	return node == NULL;
}



char * UShortToString (unsigned short X, char * Result){
	unsigned short index = 0;

	char result[15] = {0};
	while(X != 0){
		result[index] = (char)((X%10)+48);
		X /= 10;
		index++;
	}
	for(int i = 0; i < (int)strlen(result); i++){
		Result[i] = result[(int)strlen(result)-1-i];
	}

	return Result;
}

char * FloatToString (float Y, char * result){
	unsigned short index = 0;
	
	Y *= 1000;
	int X = (int)Y;
	

	while(X != 0){
		result[(sizeof(result)-1)-index] = (char)((X%10)+48);
		X /= 10;
		index++;
	}

	return result;
}

void clearScreen() {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void secureInputString(char *buffer, int size) {
	if (fgets(buffer, size, stdin) == NULL) {
		printf("Error membaca input!\n");
		exit(EXIT_FAILURE);
	}
    
	size_t len = strlen(buffer);
	if (buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0';
	} else {
		//clearInput_buffer();
	}
}

int secureInputInt() {
	char buffer[5];
	int value;
	while (1) {
		secureInputString(buffer, sizeof(buffer));

		int valid = 1;
		for (size_t i = 0; buffer[i] != '\0'; i++) {
			if (!isdigit(buffer[i])) {
				valid = 0;
				break;
			}
		}
	
		if (valid) {
			value = atoi(buffer);
			return value;
		} else {
			printf("Input tidak valid! Masukkan angka: ");
		}
	}
}

char secureInputChar() {
	char buffer[2];
	int value;
	while (1) {
		secureInputString(buffer, sizeof(buffer));

		int valid = 1;
		for (size_t i = 0; buffer[i] != '\0'; i++) {
			if (isdigit(buffer[i])) {
				valid = 0;
				break;
			}
		}
	
		if (valid) {
			value = buffer[0];
			return (char)value;
		} else {
			printf("Input tidak valid! Masukkan karakter: ");
		}
	}
}

double secureInputDouble() {
    char buffer[100];
    float value;
    while (1) {
        secureInputString(buffer, sizeof(buffer));
        
        int valid = 1, dot_count = 0;
        for (size_t i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i]) && buffer[i] != '.') {
                valid = 0;
                break;
            }
            if (buffer[i] == '.') {
                dot_count++;
                if (dot_count > 1) {
                    valid = 0;
                    break;
                }
            }
        }

        if (valid) {
            value = strtof(buffer, NULL);
            return value;
        } else {
            printf("Input tidak valid! Masukkan angka desimal: ");
        }
    }
}

/*
void clearInput_buffer(){
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
*/
void printEnter(){
	printf("Tekan ENTER untuk melanjutkan!!");
}

char upperChar(char x){
	if(x >= 97 && x <= 122){	//kalo lowecase dikurang 32 jadi uppercase
		return (char)(x-32);
	}else if(x >= 65 && x <= 90){	//kalo uppercase ya uppercase
		return x;
	}

	return (char)85;		//kalo ngaco ya defaultnya 'U'
}

char* myStrdup(const char* input) {
	if (input == NULL) return NULL;
	
	size_t len = strlen(input) + 1;
	char* dst = malloc(len);
	
	if (dst != NULL) memcpy(dst, input, len);

    return dst;
}

void clearBuffer (char * buffer, int size){
	for(int i = 0; i < size; i++) buffer[i] = '\0';
}
