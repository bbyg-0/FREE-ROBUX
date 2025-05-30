#pragma once

typedef void (*pointPro)(void);

typedef struct {
	char * syntax;
	pointPro procedure;
} command;

typedef command box[10];

void inisialisasi(box cmd);

void proccess(char * syntax, box cmd);

void addCommand(char * syntax, void * procedure, box cmd);

void SHUTDOWN();

void REBOOT();

void EXIT();
