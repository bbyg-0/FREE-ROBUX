#pragma once

#include <stdbool.h>


bool isEmpty (void * node);

char * UShortToString (unsigned short X, char * result);

char * FloatToString (float X, char * result);

void clearScreen();

void secureInputString(char *buffer, int size);

int secureInputInt();

char secureInputChar();

double secureInputDouble();

//void clearInput_buffer();

void printEnter();

char upperChar(char x);

char* myStrdup(const char * input);

void clearBuffer(char * buffer, int size);
