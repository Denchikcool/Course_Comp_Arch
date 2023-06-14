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

#define EN_GRAPHIC write (1, EN_GRAPHIC_STR, sizeof (EN_GRAPHIC_STR))
#define DIS_GRAPHIC write (1, DIS_GRAPHIC_STR, sizeof (DIS_GRAPHIC_STR))
#define WRITE_STR(str) write (1, str, sizeof (str))
#define HIGHLIGHT_RESET                                                       \
  do                                                                          \
    {                                                                         \
      mt_setfgcolor (WHITE);                                                  \
      mt_setbgcolor (BLACK);                                                  \
    }                                                                         \
  while (0)
#define HIGHLIGHT_SET                                                         \
  do                                                                          \
    {                                                                         \
      mt_setfgcolor (BLACK);                                                  \
      mt_setbgcolor (WHITE);                                                  \
    }                                                                         \
  while (0)
#define RAM_BACKUP "RAM_BACKUP.bin"

extern char EN_GRAPHIC_STR[];
extern char DIS_GRAPHIC_STR[];

int bc_printA (char *);
int bc_box (int, int, int, int);
int bc_printbigchar (int *, int, int, int, int);
int bc_setbigcharpos (int *, int, int, int);
int bc_getbigcharpos (int *, int, int, int *);
int bc_bigcharwrite (int, int *, int);
int bc_bigcharread (int, int *, int, int *);
