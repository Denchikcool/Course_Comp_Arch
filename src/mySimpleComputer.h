#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

extern uint16_t *memory;
extern uint8_t flag;
extern uint16_t ACCUM;
extern uint8_t IP;

enum flagMask
{
  flagP = 0b00000001,
  flag0 = 0b00000010,
  flagM = 0b00000100,
  flagT = 0b00001000,
  flagE = 0b00010000
};

int sc_memoryInit ();
int sc_memorySet (int, int);
int sc_memoryGet (int, int *);
int sc_memorySave (char *);
int sc_memoryLoad (char *);
int sc_regInit ();
int sc_regSet (int, int);
int sc_regGet (int, int *);
int sc_commandEncode (int, int, int *);
int sc_commandDecode (int, int *, int *);
