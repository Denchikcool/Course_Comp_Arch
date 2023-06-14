#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

enum colors
{
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  PURPLE,
  CYAN,
  WHITE
};

int mt_clrscr (void);
int mt_gotoXY (int, int);
int mt_getscreensize (int *, int *);
int mt_setfgcolor (enum colors);
int mt_setbgcolor (enum colors);
