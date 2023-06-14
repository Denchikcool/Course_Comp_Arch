#include "myBigChars.h"
#include "graphics.h"
#include "mySimpleComputer.h"
#include "myTerm.h"

char EN_GRAPHIC_STR[] = "\E(0";
char DIS_GRAPHIC_STR[] = "\E(B";

int
bc_printA (char *str)
{
  EN_GRAPHIC;
  write (1, str, strlen (str));
  DIS_GRAPHIC;

  return 0;
}

int
bc_box (int x1, int y1, int x2, int y2)
{
  mt_gotoXY (x1, y1);

  bc_printA ("l");

  for (int i = x1 + 1; i < x2; ++i)
    {
      bc_printA ("q");
    }

  bc_printA ("k");

  for (int i = y1 + 1; i < y2; ++i)
    {
      mt_gotoXY (x1, i);
      bc_printA ("x");
      mt_gotoXY (x2, i);
      bc_printA ("x");
    }

  mt_gotoXY (x1, y2);

  bc_printA ("m");

  for (int i = x1 + 1; i < x2; ++i)
    {
      bc_printA ("q");
    }

  bc_printA ("j");

  return 0;
}

int
bc_printbigchar (int *big, int x, int y, int fg, int bg)
{
  int valuee;
  int x_shift = 1, y_shift = 1;

  mt_gotoXY (x, y);
  mt_setfgcolor (fg);
  mt_setbgcolor (bg);

  for (int i = 0; i < 32; i += 8)
    {
      for (int j = 0; j < 8; j++)
        {
          bc_getbigcharpos (big, x_shift + j, y_shift, &valuee);
          if (valuee)
            bc_printA ("a");
          else
            bc_printA (" ");
        }

      y_shift++;
      mt_gotoXY (x, ++y);
    }

  for (int i = 0; i < 32; i += 8)
    {
      for (int j = 0; j < 8; j++)
        {
          bc_getbigcharpos (big, x_shift + j, y_shift, &valuee);
          if (valuee)
            bc_printA ("a");
          else
            bc_printA (" ");
        }

      y_shift++;
      mt_gotoXY (x, ++y);
    }

  HIGHLIGHT_RESET;

  return 0;
}

int
bc_setbigcharpos (int *big, int x, int y, int value)
{
  int shift;

  if (value < 0 || value > 1 || x < 1 || x > 8 || y < 1 || y > 8)
    return -1;

  if (y <= 4)
    {
      shift = (y * 8) - (8 - x) - 1;

      if (value)
        big[0] |= (1 << shift);
      else
        big[0] &= ~(1 << shift);
    }
  else
    {
      shift = ((y - 4) * 8) - (8 - x) - 1;

      if (value)
        big[1] |= (1 << shift);
      else
        big[1] &= ~(1 << shift);
    }

  return 0;
}

int
bc_getbigcharpos (int *big, int x, int y, int *value)
{
  int shift;
  int temp;

  if (x < 1 || x > 8 || y < 1 || y > 8)
    return -1;

  if (y <= 4)
    {
      shift = (y * 8) - (8 - x) - 1;
      temp = big[0] & (1 << shift);
      if (temp != 0)
        *value = 1;
      else
        *value = 0;
    }
  else
    {
      shift = ((y - 4) * 8) - (8 - x) - 1;
      temp = big[1] & (1 << shift);
      if (temp != 0)
        *value = 1;
      else
        *value = 0;
    }

  return 0;
}

int
bc_bigcharwrite (int fd, int *big, int count)
{
  write (fd, big, sizeof (int) * count * 2);

  return 0;
}

int
bc_bigcharread (int fd, int *big, int need_count, int *count)
{
  *count = read (fd, big, sizeof (int) * need_count * 2);

  if (*count / sizeof (int) != need_count * 2)
    return -1;

  return 0;
}
