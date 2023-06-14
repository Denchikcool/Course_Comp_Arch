#pragma once

#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

extern struct termios SAVE_TERM;

enum KEYS
{
  ESC_KEY,
  L_KEY,
  S_KEY,
  R_KEY,
  T_KEY,
  I_KEY,
  F5_KEY,
  F6_KEY,
  UP_KEY,
  DOWN_KEY,
  LEFT_KEY,
  RIGHT_KEY,
  ENTER_KEY,
  EMPTY_KEY,
};

int rk_readKey (enum KEYS *);
int rk_myTermSave (void);
int rk_myTermRestore (void);
int rk_myTermRegime (int, int, int, int, int);
