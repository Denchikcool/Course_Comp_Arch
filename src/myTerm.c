#include "myTerm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int
mt_clrscr (void)
{
  char *command = "\E[H\E[2J";
  if (write (1, command, sizeof (command)))
    return 0;
  else
    return -1;
}

int
mt_gotoXY (int line, int row)
{
  int size;
  char *command = calloc (64, sizeof (char));

  size = snprintf (command, sizeof (command) * 64, "\E[%d;%dH", row + 1,
                   line + 1);
  if (write (1, command, size))
    {
      free (command);
      return 0;
    }
  else
    return -1;
}

int
mt_getscreensize (int *rows, int *cols)
{
  struct winsize ws;

  if (ioctl (1, TIOCGWINSZ, &ws))
    {
      return -1;
    }

  *rows = ws.ws_row;
  *cols = ws.ws_col;
  return 0;
}

int
mt_setfgcolor (enum colors color)
{
  int size;
  char *command = calloc (64, sizeof (char));

  size = snprintf (command, sizeof (command) * 64, "\E[3%dm", color);
  if (write (1, command, size))
    {
      free (command);
      return 0;
    }
  else
    return -1;
}

int
mt_setbgcolor (enum colors color)
{
  int size;
  char *command = calloc (64, sizeof (char));

  size = snprintf (command, sizeof (command) * 64, "\E[4%dm", color);
  if (write (1, command, size))
    {
      free (command);
      return 0;
    }
  else
    return -1;
}
