#pragma once

typedef struct x{
	char * syntax;
	void * procedure;
} command;

void proccess(char * syntax, command box[]);

void addCommand(char * syntax, void * procedure);
