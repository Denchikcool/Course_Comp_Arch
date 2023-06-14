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

int CU (void);
int cu_read (int);
int cu_write (int);
int cu_load (int);
int cu_store (int);
int cu_jump (int);
int cu_jneg (int);
int cu_jz (int);
