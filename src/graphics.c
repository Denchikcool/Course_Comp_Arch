#include "graphics.h"
#include "myBigChars.h"
#include "myReadkey.h"
#include "mySimpleComputer.h"
#include "myTerm.h"

int SELECTED_ADDR;

void
drawMemoryBox (void)
{
  bc_box (0, 0, 60, 11);
  mt_gotoXY (27, 0);
  WRITE_STR (" Memory ");
}

void
drawMemoryContent (void)
{
  int line = 1;
  int value;
  int command;
  int operand;
  char buf[6];
  int ignore;

  sc_regGet (flagT, &ignore);

  for (int address = 0; address < 100; ++address)
    {
      if (address % 10 == 0)
        {
          mt_gotoXY (1, line);
          line++;
        }

      if (sc_memoryGet (address, &value) < 0
          || sc_commandDecode (value & 0x3FFF, &command, &operand) < 0)
        {
          sc_regSet (flagE, 1);
          WRITE_STR ("ERROR");
          return;
        }

      snprintf (buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command,
                operand);

      if (ignore == 0)
        {
          if (address == IP)
            {
              HIGHLIGHT_SET;

              write (1, buf, 5);

              HIGHLIGHT_RESET;
            }
          else
            {
              write (1, buf, 5);
            }
        }
      else
        {
          if (address == SELECTED_ADDR)
            {
              HIGHLIGHT_SET;

              write (1, buf, 5);

              HIGHLIGHT_RESET;
            }
          else
            {
              write (1, buf, 5);
            }
        }

      if ((address + 1) % 10 != 0)
        WRITE_STR (" ");
    }
}

void
drawAccumBox (void)
{
  bc_box (61, 0, 82, 2);
  mt_gotoXY (65, 0);
  WRITE_STR (" Accumulator ");
}

void
drawAccumContent (void)
{
  int command;
  int operand;
  char buf[6];

  mt_gotoXY (69, 1);

  if (sc_commandDecode (ACCUM & 0x3FFF, &command, &operand) < 0)
    {
      sc_regSet (flagE, 1);
      return;
    }

  snprintf (buf, 6, "%c%02X%02X", (ACCUM & 0x4000) ? '-' : '+', command,
            operand);

  write (1, buf, 5);
}

void
drawIPBox (void)
{
  bc_box (61, 3, 82, 5);
  mt_gotoXY (62, 3);
  WRITE_STR (" InstructionCounter ");
}

void
drawIPContent (void)
{
  char buf[6];

  mt_gotoXY (69, 4);

  WRITE_STR ("     ");

  mt_gotoXY (69, 4);

  snprintf (buf, 6, "%c%04d", (IP & 0x80) ? '-' : '+', IP);

  write (1, buf, 5);
}

void
drawOperationBox (void)
{
  bc_box (61, 6, 82, 8);
  mt_gotoXY (66, 6);
  WRITE_STR (" Operation ");
}

void
drawOperationContent (void)
{
  int value;
  int command;
  int operand;
  char buf[10];
  int ignore;

  sc_regGet (flagT, &ignore);

  if (ignore == 0)
    {
      if (sc_memoryGet (IP, &value) < 0
          || sc_commandDecode (value & 0x3FFF, &command, &operand) < 0)
        {
          sc_regSet (flagE, 1);
          return;
        }
    }
  else
    {
      if (sc_memoryGet (SELECTED_ADDR, &value) < 0
          || sc_commandDecode (value & 0x3FFF, &command, &operand) < 0)
        {
          sc_regSet (flagE, 1);
          return;
        }
    }

  mt_gotoXY (67, 7);

  snprintf (buf, 10, "%c%02X : %02X", (value & 0x4000) ? '-' : '+', command,
            operand);

  write (1, buf, 8);
}

void
drawFlagsBox (void)
{
  bc_box (61, 9, 82, 11);
  mt_gotoXY (68, 9);
  WRITE_STR (" Flags ");
}

void
drawFlagsContent (void)
{
  mt_gotoXY (67, 10);

  for (int i = 1; i < 6; ++i)
    {
      if (i == 1)
        {
          if (flag & flagP)
            {
              HIGHLIGHT_SET;
              WRITE_STR ("O");
              HIGHLIGHT_RESET;
            }
          else
            {
              WRITE_STR ("O");
            }
        }
      else if (i == 2)
        {
          if (flag & flag0)
            {
              HIGHLIGHT_SET;
              WRITE_STR ("E");
              HIGHLIGHT_RESET;
            }
          else
            {
              WRITE_STR ("E");
            }
        }
      else if (i == 3)
        {
          if (flag & flagM)
            {
              HIGHLIGHT_SET;
              WRITE_STR ("V");
              HIGHLIGHT_RESET;
            }
          else
            {
              WRITE_STR ("V");
            }
        }
      else if (i == 4)
        {
          if (flag & flagT)
            {
              HIGHLIGHT_SET;
              WRITE_STR ("M");
              HIGHLIGHT_RESET;
            }
          else
            {
              WRITE_STR ("M");
            }
        }
      else if (i == 5)
        {
          if (flag & flagE)
            {
              HIGHLIGHT_SET;
              WRITE_STR ("T");
              HIGHLIGHT_RESET;
            }
          else
            {
              WRITE_STR ("T");
            }
        }

      write (1, " ", 1);
    }
}

void
drawBigCharsBox (void)
{
  bc_box (0, 12, 45, 21);
}

void
drawKeys (void)
{
  bc_box (46, 12, 82, 21);
  mt_gotoXY (61, 12);
  WRITE_STR (" Keys: ");

  mt_gotoXY (47, 13);
  WRITE_STR (" l - load ");
  mt_gotoXY (47, 14);
  WRITE_STR (" s - save ");
  mt_gotoXY (47, 15);
  WRITE_STR (" r - run ");
  mt_gotoXY (47, 16);
  WRITE_STR (" t - step ");
  mt_gotoXY (47, 17);
  WRITE_STR (" i - reset ");
  mt_gotoXY (47, 18);
  WRITE_STR (" f5 - accumulator ");
  mt_gotoXY (47, 19);
  WRITE_STR (" f6 - IP ");
}

void
drawBottom (void)
{
  mt_gotoXY (0, 23);
  WRITE_STR ("Input\\Output:");
  mt_gotoXY (0, 24);
  WRITE_STR ("35< +1F1F");
  mt_gotoXY (0, 25);
  WRITE_STR ("35> +1F1F");
}

int
drawFont (int (*arr)[2], int x, int y, char symbol, int coordinate_x,
          int coordinate_y)
{
  int fd;
  switch (symbol)
    {
    case '+':
      bc_setbigcharpos (arr[0], x + 0, y + 0, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 0, 0);
      bc_setbigcharpos (arr[0], x + 2, y + 0, 0);
      bc_setbigcharpos (arr[0], x + 3, y + 0, 0);
      bc_setbigcharpos (arr[0], x + 4, y + 0, 0);
      bc_setbigcharpos (arr[0], x + 5, y + 0, 0);
      bc_setbigcharpos (arr[0], x + 6, y + 0, 0);
      bc_setbigcharpos (arr[0], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[0], x + 0, y + 1, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 1, 0);
      bc_setbigcharpos (arr[0], x + 2, y + 1, 0);
      bc_setbigcharpos (arr[0], x + 3, y + 1, 0);
      bc_setbigcharpos (arr[0], x + 4, y + 1, 0);
      bc_setbigcharpos (arr[0], x + 5, y + 1, 0);
      bc_setbigcharpos (arr[0], x + 6, y + 1, 0);
      bc_setbigcharpos (arr[0], x + 7, y + 1, 0);

      bc_setbigcharpos (arr[0], x + 0, y + 2, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 2, 0);
      bc_setbigcharpos (arr[0], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[0], x + 3, y + 2, 1);
      bc_setbigcharpos (arr[0], x + 4, y + 2, 1);
      bc_setbigcharpos (arr[0], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[0], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[0], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[0], x + 0, y + 3, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[0], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[0], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[0], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[0], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[0], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[0], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[0], x + 0, y + 4, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 4, 0);
      bc_setbigcharpos (arr[0], x + 2, y + 4, 0);
      bc_setbigcharpos (arr[0], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[0], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[0], x + 5, y + 4, 0);
      bc_setbigcharpos (arr[0], x + 6, y + 4, 0);
      bc_setbigcharpos (arr[0], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[0], x + 0, y + 5, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 5, 0);
      bc_setbigcharpos (arr[0], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[0], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[0], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[0], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[0], x + 6, y + 5, 0);
      bc_setbigcharpos (arr[0], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[0], x + 0, y + 6, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 6, 0);
      bc_setbigcharpos (arr[0], x + 2, y + 6, 0);
      bc_setbigcharpos (arr[0], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[0], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[0], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[0], x + 6, y + 6, 0);
      bc_setbigcharpos (arr[0], x + 7, y + 6, 0);

      bc_setbigcharpos (arr[0], x + 0, y + 7, 0);
      bc_setbigcharpos (arr[0], x + 1, y + 7, 0);
      bc_setbigcharpos (arr[0], x + 2, y + 7, 0);
      bc_setbigcharpos (arr[0], x + 3, y + 7, 0);
      bc_setbigcharpos (arr[0], x + 4, y + 7, 0);
      bc_setbigcharpos (arr[0], x + 5, y + 7, 0);
      bc_setbigcharpos (arr[0], x + 6, y + 7, 0);
      bc_setbigcharpos (arr[0], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[0], 1);
      close (fd);

      break;

    case '-':
      bc_setbigcharpos (arr[1], x + 0, y + 0, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 0, 0);
      bc_setbigcharpos (arr[1], x + 2, y + 0, 0);
      bc_setbigcharpos (arr[1], x + 3, y + 0, 0);
      bc_setbigcharpos (arr[1], x + 4, y + 0, 0);
      bc_setbigcharpos (arr[1], x + 5, y + 0, 0);
      bc_setbigcharpos (arr[1], x + 6, y + 0, 0);
      bc_setbigcharpos (arr[1], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[1], x + 0, y + 1, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 1, 0);
      bc_setbigcharpos (arr[1], x + 2, y + 1, 0);
      bc_setbigcharpos (arr[1], x + 3, y + 1, 0);
      bc_setbigcharpos (arr[1], x + 4, y + 1, 0);
      bc_setbigcharpos (arr[1], x + 5, y + 1, 0);
      bc_setbigcharpos (arr[1], x + 6, y + 1, 0);
      bc_setbigcharpos (arr[1], x + 7, y + 1, 0);

      bc_setbigcharpos (arr[1], x + 0, y + 2, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 2, 0);
      bc_setbigcharpos (arr[1], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[1], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[1], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[1], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[1], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[1], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[1], x + 0, y + 3, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[1], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[1], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[1], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[1], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[1], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[1], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[1], x + 0, y + 4, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 4, 0);
      bc_setbigcharpos (arr[1], x + 2, y + 4, 0);
      bc_setbigcharpos (arr[1], x + 3, y + 4, 0);
      bc_setbigcharpos (arr[1], x + 4, y + 4, 0);
      bc_setbigcharpos (arr[1], x + 5, y + 4, 0);
      bc_setbigcharpos (arr[1], x + 6, y + 4, 0);
      bc_setbigcharpos (arr[1], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[1], x + 0, y + 5, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 5, 0);
      bc_setbigcharpos (arr[1], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[1], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[1], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[1], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[1], x + 6, y + 5, 0);
      bc_setbigcharpos (arr[1], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[1], x + 0, y + 6, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 6, 0);
      bc_setbigcharpos (arr[1], x + 2, y + 6, 0);
      bc_setbigcharpos (arr[1], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[1], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[1], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[1], x + 6, y + 6, 0);
      bc_setbigcharpos (arr[1], x + 7, y + 6, 0);

      bc_setbigcharpos (arr[1], x + 0, y + 7, 0);
      bc_setbigcharpos (arr[1], x + 1, y + 7, 0);
      bc_setbigcharpos (arr[1], x + 2, y + 7, 0);
      bc_setbigcharpos (arr[1], x + 3, y + 7, 0);
      bc_setbigcharpos (arr[1], x + 4, y + 7, 0);
      bc_setbigcharpos (arr[1], x + 5, y + 7, 0);
      bc_setbigcharpos (arr[1], x + 6, y + 7, 0);
      bc_setbigcharpos (arr[1], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[1], 1);
      close (fd);

      break;

    case '0':
      bc_setbigcharpos (arr[2], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[2], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[2], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[2], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[2], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[2], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[2], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[2], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[2], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[2], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 3, 0);
      bc_setbigcharpos (arr[2], x + 4, y + 3, 0);
      bc_setbigcharpos (arr[2], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[2], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 4, 0);
      bc_setbigcharpos (arr[2], x + 4, y + 4, 0);
      bc_setbigcharpos (arr[2], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[2], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[2], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[2], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[2], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[2], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[2], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[2], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[2], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[2], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[2], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[2], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[2], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[2], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[2], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[2], 1);
      close (fd);

      break;

    case '1':
      bc_setbigcharpos (arr[3], x + 0, y + 0, 0);
      bc_setbigcharpos (arr[3], x + 1, y + 0, 0);
      bc_setbigcharpos (arr[3], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 0, 0);
      bc_setbigcharpos (arr[3], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[3], x + 0, y + 1, 0);
      bc_setbigcharpos (arr[3], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[3], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 1, 0);
      bc_setbigcharpos (arr[3], x + 7, y + 1, 0);

      bc_setbigcharpos (arr[3], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[3], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[3], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 2, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 2, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[3], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[3], x + 0, y + 3, 0);
      bc_setbigcharpos (arr[3], x + 1, y + 3, 0);
      bc_setbigcharpos (arr[3], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 3, 0);
      bc_setbigcharpos (arr[3], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[3], x + 0, y + 4, 0);
      bc_setbigcharpos (arr[3], x + 1, y + 4, 0);
      bc_setbigcharpos (arr[3], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 4, 0);
      bc_setbigcharpos (arr[3], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[3], x + 0, y + 5, 0);
      bc_setbigcharpos (arr[3], x + 1, y + 5, 0);
      bc_setbigcharpos (arr[3], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 5, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 5, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 5, 0);
      bc_setbigcharpos (arr[3], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[3], x + 0, y + 6, 0);
      bc_setbigcharpos (arr[3], x + 1, y + 6, 0);
      bc_setbigcharpos (arr[3], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 6, 0);
      bc_setbigcharpos (arr[3], x + 7, y + 6, 0);

      bc_setbigcharpos (arr[3], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[3], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[3], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[3], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[3], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[3], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[3], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[3], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[3], 1);
      close (fd);

      break;

    case '2':
      bc_setbigcharpos (arr[4], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[4], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[4], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[4], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[4], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[4], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[4], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[4], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[4], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[4], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[4], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[4], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[4], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[4], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[4], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[4], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[4], x + 0, y + 2, 0);
      bc_setbigcharpos (arr[4], x + 1, y + 2, 0);
      bc_setbigcharpos (arr[4], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[4], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[4], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[4], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[4], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[4], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[4], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[4], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[4], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[4], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[4], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[4], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[4], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[4], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[4], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[4], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[4], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[1], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[4], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[4], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[4], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[4], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[4], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[4], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[4], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[4], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[4], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[4], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[4], x + 6, y + 5, 0);
      bc_setbigcharpos (arr[4], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[4], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[4], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[4], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[4], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[4], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[4], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[4], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[4], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[4], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[4], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[4], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[4], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[4], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[4], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[4], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[4], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[4], 1);
      close (fd);

      break;

    case '3':
      bc_setbigcharpos (arr[5], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[5], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[5], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[5], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[5], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[5], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[5], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[5], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[5], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[5], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[5], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[5], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[5], x + 0, y + 2, 0);
      bc_setbigcharpos (arr[5], x + 1, y + 2, 0);
      bc_setbigcharpos (arr[5], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[5], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[5], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[5], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[5], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[5], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[5], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[5], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[5], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[5], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[5], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[5], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[5], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[5], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[5], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[5], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[5], x + 0, y + 5, 0);
      bc_setbigcharpos (arr[5], x + 1, y + 5, 0);
      bc_setbigcharpos (arr[5], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[5], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[5], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[5], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[5], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[5], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[5], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[5], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[5], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[5], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[5], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[5], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[5], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[5], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[5], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[5], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[5], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[5], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[5], 1);
      close (fd);

      break;

    case '4':
      bc_setbigcharpos (arr[6], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[6], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[6], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[6], x + 3, y + 0, 0);
      bc_setbigcharpos (arr[6], x + 4, y + 0, 0);
      bc_setbigcharpos (arr[6], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[6], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[6], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[6], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[6], x + 3, y + 1, 0);
      bc_setbigcharpos (arr[6], x + 4, y + 1, 0);
      bc_setbigcharpos (arr[6], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[6], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[6], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[6], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[6], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[6], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[6], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[6], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[6], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[6], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[6], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[6], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[6], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[6], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[6], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[6], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[6], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[6], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[6], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[6], x + 0, y + 5, 0);
      bc_setbigcharpos (arr[6], x + 1, y + 5, 0);
      bc_setbigcharpos (arr[6], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[6], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[6], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[6], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[6], x + 0, y + 6, 0);
      bc_setbigcharpos (arr[6], x + 1, y + 6, 0);
      bc_setbigcharpos (arr[6], x + 2, y + 6, 0);
      bc_setbigcharpos (arr[6], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[6], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[6], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[6], x + 0, y + 7, 0);
      bc_setbigcharpos (arr[6], x + 1, y + 7, 0);
      bc_setbigcharpos (arr[6], x + 2, y + 7, 0);
      bc_setbigcharpos (arr[6], x + 3, y + 7, 0);
      bc_setbigcharpos (arr[6], x + 4, y + 7, 0);
      bc_setbigcharpos (arr[6], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[6], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[6], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[6], 1);
      close (fd);

      break;

    case '5':
      bc_setbigcharpos (arr[7], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[7], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[7], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[7], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[7], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[7], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[7], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[7], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[7], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[7], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[7], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[7], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[7], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[7], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[7], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[7], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[7], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[7], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[7], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[7], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[7], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[7], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[7], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[7], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[7], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[7], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[7], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[7], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[7], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[7], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[7], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[7], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[7], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[7], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[7], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[7], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[7], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[7], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[7], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[7], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[7], x + 0, y + 5, 0);
      bc_setbigcharpos (arr[7], x + 1, y + 5, 0);
      bc_setbigcharpos (arr[7], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[7], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[7], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[7], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[7], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[7], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[7], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[7], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[7], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[7], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[7], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[7], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[7], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[7], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[7], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[7], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[7], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[7], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[7], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[7], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[7], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[7], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[7], 1);
      close (fd);

      break;

    case '6':
      bc_setbigcharpos (arr[8], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[8], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[8], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[8], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[8], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[8], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[8], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[8], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[8], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[8], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[8], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[8], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[8], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[8], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[8], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[8], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[8], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[8], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[8], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[8], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[8], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[8], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[8], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[8], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[8], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[8], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[8], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[8], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[8], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[8], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[8], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[8], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[8], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[8], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[8], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[8], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[8], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[8], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[8], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[8], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[8], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[8], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[8], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[8], 1);
      close (fd);

      break;

    case '7':
      bc_setbigcharpos (arr[9], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[9], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[9], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[9], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[9], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[9], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[9], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[9], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[9], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[9], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[9], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[9], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[9], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[9], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[9], x + 0, y + 2, 0);
      bc_setbigcharpos (arr[9], x + 1, y + 2, 0);
      bc_setbigcharpos (arr[9], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[9], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[9], x + 4, y + 2, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[9], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[9], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[9], x + 0, y + 3, 0);
      bc_setbigcharpos (arr[9], x + 1, y + 3, 0);
      bc_setbigcharpos (arr[9], x + 2, y + 3, 0);
      bc_setbigcharpos (arr[9], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[9], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[9], x + 6, y + 3, 0);
      bc_setbigcharpos (arr[9], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[9], x + 0, y + 4, 0);
      bc_setbigcharpos (arr[9], x + 1, y + 4, 0);
      bc_setbigcharpos (arr[9], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[9], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[9], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 4, 0);
      bc_setbigcharpos (arr[9], x + 6, y + 4, 0);
      bc_setbigcharpos (arr[9], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[9], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[9], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[9], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[9], x + 3, y + 5, 1);
      bc_setbigcharpos (arr[9], x + 4, y + 5, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[9], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[9], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[9], x + 0, y + 6, 0);
      bc_setbigcharpos (arr[9], x + 1, y + 6, 0);
      bc_setbigcharpos (arr[9], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[9], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[9], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[9], x + 6, y + 6, 0);
      bc_setbigcharpos (arr[9], x + 7, y + 6, 0);

      bc_setbigcharpos (arr[9], x + 0, y + 7, 0);
      bc_setbigcharpos (arr[9], x + 1, y + 7, 0);
      bc_setbigcharpos (arr[9], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[9], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[9], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[9], x + 5, y + 7, 0);
      bc_setbigcharpos (arr[9], x + 6, y + 7, 0);
      bc_setbigcharpos (arr[9], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[9], 1);
      close (fd);

      break;

    case '8':
      bc_setbigcharpos (arr[10], x + 0, y + 0, 0);
      bc_setbigcharpos (arr[10], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[10], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[10], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[10], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[10], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[10], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[10], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[10], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[10], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[10], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[10], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[10], x + 0, y + 3, 0);
      bc_setbigcharpos (arr[10], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[10], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[10], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[10], x + 0, y + 4, 0);
      bc_setbigcharpos (arr[10], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[10], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[10], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[10], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[10], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[10], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[10], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[10], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[10], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[10], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[10], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[10], x + 0, y + 7, 0);
      bc_setbigcharpos (arr[10], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[10], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[10], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[10], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[10], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[10], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[10], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[10], 1);
      close (fd);

      break;

    case '9':
      bc_setbigcharpos (arr[11], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[11], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[11], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[11], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[11], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[11], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[11], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[11], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[11], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[11], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[11], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[11], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[11], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[11], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[11], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[11], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[11], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[11], x + 5, y + 2, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[11], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[11], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[11], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[11], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[11], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[11], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[11], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[11], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[11], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[11], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[11], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[11], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[11], x + 0, y + 5, 0);
      bc_setbigcharpos (arr[11], x + 1, y + 5, 0);
      bc_setbigcharpos (arr[11], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[11], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[11], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[11], x + 5, y + 5, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[11], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[11], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[11], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[11], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[11], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[11], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[11], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[11], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[11], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[11], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[11], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[11], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[11], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[11], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[11], 1);
      close (fd);

      break;

    case 'A':
      bc_setbigcharpos (arr[12], x + 0, y + 0, 0);
      bc_setbigcharpos (arr[12], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[12], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[12], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[12], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[12], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[12], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[12], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 1, 0);
      bc_setbigcharpos (arr[12], x + 3, y + 1, 0);
      bc_setbigcharpos (arr[12], x + 4, y + 1, 0);
      bc_setbigcharpos (arr[12], x + 5, y + 1, 0);
      bc_setbigcharpos (arr[12], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[12], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[12], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[12], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[12], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[12], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[12], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[12], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[12], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[12], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[12], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[12], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[12], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[12], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[12], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 4, 0);
      bc_setbigcharpos (arr[12], x + 3, y + 4, 0);
      bc_setbigcharpos (arr[12], x + 4, y + 4, 0);
      bc_setbigcharpos (arr[12], x + 5, y + 4, 0);
      bc_setbigcharpos (arr[12], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[12], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[12], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[12], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[12], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[12], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[12], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[12], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[12], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 6, 0);
      bc_setbigcharpos (arr[12], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[12], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[12], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[12], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[12], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[12], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[12], x + 2, y + 7, 0);
      bc_setbigcharpos (arr[12], x + 3, y + 7, 0);
      bc_setbigcharpos (arr[12], x + 4, y + 7, 0);
      bc_setbigcharpos (arr[12], x + 5, y + 7, 0);
      bc_setbigcharpos (arr[12], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[12], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[12], 1);
      close (fd);

      break;

    case 'B':
      bc_setbigcharpos (arr[13], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[13], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[13], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[13], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[13], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[13], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 1, 0);
      bc_setbigcharpos (arr[13], x + 3, y + 1, 0);
      bc_setbigcharpos (arr[13], x + 4, y + 1, 0);
      bc_setbigcharpos (arr[13], x + 5, y + 1, 0);
      bc_setbigcharpos (arr[13], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[13], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[13], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[13], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[13], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[13], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[13], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[13], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[13], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[13], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[13], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[13], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 4, 0);
      bc_setbigcharpos (arr[13], x + 3, y + 4, 0);
      bc_setbigcharpos (arr[13], x + 4, y + 4, 0);
      bc_setbigcharpos (arr[13], x + 5, y + 4, 0);
      bc_setbigcharpos (arr[13], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[13], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[13], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[13], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[13], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[13], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[13], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 6, 0);
      bc_setbigcharpos (arr[13], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[13], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[13], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[13], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[13], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[13], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[13], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[13], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[13], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[13], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[13], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[13], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[13], 1);
      close (fd);

      break;

    case 'C':
      bc_setbigcharpos (arr[14], x + 0, y + 0, 0);
      bc_setbigcharpos (arr[14], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[14], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[14], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[14], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[14], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[14], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[14], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[14], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 1, 0);
      bc_setbigcharpos (arr[14], x + 3, y + 1, 0);
      bc_setbigcharpos (arr[14], x + 4, y + 1, 0);
      bc_setbigcharpos (arr[14], x + 5, y + 1, 0);
      bc_setbigcharpos (arr[14], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[14], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[14], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[14], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[14], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[14], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[14], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[14], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[14], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[14], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[14], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 3, 0);
      bc_setbigcharpos (arr[14], x + 3, y + 3, 0);
      bc_setbigcharpos (arr[14], x + 4, y + 3, 0);
      bc_setbigcharpos (arr[14], x + 5, y + 3, 0);
      bc_setbigcharpos (arr[14], x + 6, y + 3, 0);
      bc_setbigcharpos (arr[14], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[14], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[14], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 4, 0);
      bc_setbigcharpos (arr[14], x + 3, y + 4, 0);
      bc_setbigcharpos (arr[14], x + 4, y + 4, 0);
      bc_setbigcharpos (arr[14], x + 5, y + 4, 0);
      bc_setbigcharpos (arr[14], x + 6, y + 4, 0);
      bc_setbigcharpos (arr[14], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[14], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[14], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[14], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[14], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[14], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[14], x + 6, y + 5, 0);
      bc_setbigcharpos (arr[14], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[14], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[14], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 6, 0);
      bc_setbigcharpos (arr[14], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[14], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[14], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[14], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[14], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[14], x + 0, y + 7, 0);
      bc_setbigcharpos (arr[14], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[14], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[14], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[14], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[14], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[14], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[14], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[14], 1);
      close (fd);

      break;

    case 'D':
      bc_setbigcharpos (arr[15], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[15], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[15], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[15], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[15], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 0, 0);

      bc_setbigcharpos (arr[15], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 1, 0);
      bc_setbigcharpos (arr[15], x + 3, y + 1, 0);
      bc_setbigcharpos (arr[15], x + 4, y + 1, 0);
      bc_setbigcharpos (arr[15], x + 5, y + 1, 0);
      bc_setbigcharpos (arr[15], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[15], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 2, 0);
      bc_setbigcharpos (arr[15], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[15], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[15], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[15], x + 6, y + 2, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 2, 1);

      bc_setbigcharpos (arr[15], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 3, 0);
      bc_setbigcharpos (arr[15], x + 3, y + 3, 0);
      bc_setbigcharpos (arr[15], x + 4, y + 3, 0);
      bc_setbigcharpos (arr[15], x + 5, y + 3, 0);
      bc_setbigcharpos (arr[15], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 3, 1);

      bc_setbigcharpos (arr[15], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 4, 0);
      bc_setbigcharpos (arr[15], x + 3, y + 4, 0);
      bc_setbigcharpos (arr[15], x + 4, y + 4, 0);
      bc_setbigcharpos (arr[15], x + 5, y + 4, 0);
      bc_setbigcharpos (arr[15], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 4, 1);

      bc_setbigcharpos (arr[15], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 5, 0);
      bc_setbigcharpos (arr[15], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[15], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[15], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[15], x + 6, y + 5, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 5, 1);

      bc_setbigcharpos (arr[15], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 6, 0);
      bc_setbigcharpos (arr[15], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[15], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[15], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[15], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[15], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[15], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[15], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[15], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[15], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[15], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[15], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[15], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[15], 1);
      close (fd);

      break;

    case 'E':
      bc_setbigcharpos (arr[16], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[16], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[16], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[16], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[16], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[16], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[16], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[16], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[16], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[16], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[16], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[16], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[16], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[16], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[16], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[16], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[16], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[16], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[16], x + 6, y + 3, 1);
      bc_setbigcharpos (arr[16], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[16], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[16], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[16], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[16], x + 6, y + 4, 1);
      bc_setbigcharpos (arr[16], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[16], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[16], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[16], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[16], x + 6, y + 5, 0);
      bc_setbigcharpos (arr[16], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[16], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 6, 1);
      bc_setbigcharpos (arr[16], x + 4, y + 6, 1);
      bc_setbigcharpos (arr[16], x + 5, y + 6, 1);
      bc_setbigcharpos (arr[16], x + 6, y + 6, 1);
      bc_setbigcharpos (arr[16], x + 7, y + 6, 1);

      bc_setbigcharpos (arr[16], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[16], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[16], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[16], x + 3, y + 7, 1);
      bc_setbigcharpos (arr[16], x + 4, y + 7, 1);
      bc_setbigcharpos (arr[16], x + 5, y + 7, 1);
      bc_setbigcharpos (arr[16], x + 6, y + 7, 1);
      bc_setbigcharpos (arr[16], x + 7, y + 7, 1);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[16], 1);
      close (fd);

      break;

    case 'F':
      bc_setbigcharpos (arr[17], x + 0, y + 0, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 0, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 0, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 0, 1);
      bc_setbigcharpos (arr[17], x + 4, y + 0, 1);
      bc_setbigcharpos (arr[17], x + 5, y + 0, 1);
      bc_setbigcharpos (arr[17], x + 6, y + 0, 1);
      bc_setbigcharpos (arr[17], x + 7, y + 0, 1);

      bc_setbigcharpos (arr[17], x + 0, y + 1, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 1, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 1, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 1, 1);
      bc_setbigcharpos (arr[17], x + 4, y + 1, 1);
      bc_setbigcharpos (arr[17], x + 5, y + 1, 1);
      bc_setbigcharpos (arr[17], x + 6, y + 1, 1);
      bc_setbigcharpos (arr[17], x + 7, y + 1, 1);

      bc_setbigcharpos (arr[17], x + 0, y + 2, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 2, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 2, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 2, 0);
      bc_setbigcharpos (arr[17], x + 4, y + 2, 0);
      bc_setbigcharpos (arr[17], x + 5, y + 2, 0);
      bc_setbigcharpos (arr[17], x + 6, y + 2, 0);
      bc_setbigcharpos (arr[17], x + 7, y + 2, 0);

      bc_setbigcharpos (arr[17], x + 0, y + 3, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 3, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 3, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 3, 1);
      bc_setbigcharpos (arr[17], x + 4, y + 3, 1);
      bc_setbigcharpos (arr[17], x + 5, y + 3, 1);
      bc_setbigcharpos (arr[17], x + 6, y + 3, 0);
      bc_setbigcharpos (arr[17], x + 7, y + 3, 0);

      bc_setbigcharpos (arr[17], x + 0, y + 4, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 4, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 4, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 4, 1);
      bc_setbigcharpos (arr[17], x + 4, y + 4, 1);
      bc_setbigcharpos (arr[17], x + 5, y + 4, 1);
      bc_setbigcharpos (arr[17], x + 6, y + 4, 0);
      bc_setbigcharpos (arr[17], x + 7, y + 4, 0);

      bc_setbigcharpos (arr[17], x + 0, y + 5, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 5, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 5, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 5, 0);
      bc_setbigcharpos (arr[17], x + 4, y + 5, 0);
      bc_setbigcharpos (arr[17], x + 5, y + 5, 0);
      bc_setbigcharpos (arr[17], x + 6, y + 5, 0);
      bc_setbigcharpos (arr[17], x + 7, y + 5, 0);

      bc_setbigcharpos (arr[17], x + 0, y + 6, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 6, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 6, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 6, 0);
      bc_setbigcharpos (arr[17], x + 4, y + 6, 0);
      bc_setbigcharpos (arr[17], x + 5, y + 6, 0);
      bc_setbigcharpos (arr[17], x + 6, y + 6, 0);
      bc_setbigcharpos (arr[17], x + 7, y + 6, 0);

      bc_setbigcharpos (arr[17], x + 0, y + 7, 1);
      bc_setbigcharpos (arr[17], x + 1, y + 7, 1);
      bc_setbigcharpos (arr[17], x + 2, y + 7, 1);
      bc_setbigcharpos (arr[17], x + 3, y + 7, 0);
      bc_setbigcharpos (arr[17], x + 4, y + 7, 0);
      bc_setbigcharpos (arr[17], x + 5, y + 7, 0);
      bc_setbigcharpos (arr[17], x + 6, y + 7, 0);
      bc_setbigcharpos (arr[17], x + 7, y + 7, 0);

      fd = open ("font.bin", O_WRONLY | O_CREAT | O_APPEND, 0600);
      bc_bigcharwrite (fd, arr[17], 1);
      close (fd);

      break;

    default:
      return -1;
      break;
    }

  return 0;
}

void
getMemoryCell (char *buf)
{
  int value;
  int command;
  int operand;
  int ignore;

  sc_regGet (flagT, &ignore);

  if (ignore == 0)
    {
      sc_memoryGet (IP, &value);
      sc_commandDecode (value & 0x3FFF, &command, &operand);
    }
  else
    {
      sc_memoryGet (SELECTED_ADDR, &value);
      sc_commandDecode (value & 0x3FFF, &command, &operand);
    }

  snprintf (buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command,
            operand);
}

void
loadProgram (void)
{
  char buffer[64] = { 0 };
  int size;

  size = read (0, buffer, sizeof (buffer));

  fflush (0);

  buffer[size - 1] = '\0';

  sc_memoryLoad (buffer);
}

void
saveProgram (void)
{
  char buffer[64] = { 0 };
  int size;

  size = read (0, buffer, sizeof (buffer));

  fflush (0);

  buffer[size - 1] = '\0';

  sc_memorySave (buffer);
}

void
setIP (void)
{
  char buffer[10] = { 0 };
  int number;

  read (0, buffer, sizeof (buffer));

  fflush (0);

  number = (int)strtol (&buffer[0], NULL, 10);

  IP = (uint16_t)number;
}

void
setAccum (void)
{
  char buffer[10] = { 0 };
  int number;

  read (0, buffer, sizeof (buffer));

  fflush (0);

  if (buffer[0] != '+' && buffer[0] != '-')
    {
      write (STDOUT_FILENO, "Wrong input", 11);
      return;
    }

  number = (int)strtol (&buffer[1], NULL, 16);

  if (number > 0x3FFF)
    {
      write (STDOUT_FILENO, "Incorrect size of value", 23);
      return;
    }

  if (buffer[0] == '-')
    {
      number = number | 0x4000;
    }

  ACCUM = number;
}

void
setValue (void)
{
  char buffer[10] = { 0 };
  int number;
  int ignore;

  sc_regGet (flagT, &ignore);

  read (0, buffer, sizeof (buffer));

  fflush (0);

  if (buffer[0] != '+' && buffer[0] != '-')
    {
      write (STDOUT_FILENO, "Wrong input", 11);
      return;
    }

  number = (int)strtol (&buffer[1], NULL, 16);

  if (number > 0x3FFF)
    {
      write (STDOUT_FILENO, "Incorrect size of value", 23);
      return;
    }

  if (buffer[0] == '-')
    {
      number = number | 0x4000;
    }

  if (ignore == 0)
    {
      sc_memorySet (IP, number);
    }
  else
    {
      sc_memorySet (SELECTED_ADDR, number);
    }
}
