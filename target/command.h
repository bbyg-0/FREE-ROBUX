#pragma once

typedef void (*pointPro)(void * param);

typedef struct {
	char * syntax;
	pointPro procedure;
} command;

typedef command box[10];

void inisialisasi(box cmd);

void proccess(char * syntax, void * param, box cmd);

void addCommand(char * syntax, void * procedure, box cmd);

void SHUTDOWN();

void REBOOT();

void EXIT();

void GETKEYLOG();
