#pragma once

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

extern int SELECTED_ADDR;

void drawMemoryBox ();
void drawMemoryContent ();
void drawAccumBox ();
void drawAccumContent ();
void drawIPBox ();
void drawIPContent ();
void drawOperationBox ();
void drawOperationContent ();
void drawFlagsBox ();
void drawFlagsContent ();
void drawBigCharsBox ();
void drawBigCharsContent (int, int (*)[2]);
void drawKeys ();
void drawBottom ();
int drawFont (int (*)[2], int, int, char, int, int);
void getMemoryCell (char *);
void setValue (void);
void setAccum (void);
void setIP (void);
void saveProgram (void);
void loadProgram (void);
