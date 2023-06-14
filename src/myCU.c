#include "myCU.h"
#include "myALU.h"
#include "myBigChars.h"
#include "myReadkey.h"
#include "mySimpleComputer.h"
#include "myTerm.h"

int
CU ()
{
  int value;
  int command;
  int operand;

  sc_memoryGet (IP, &value);

  if (sc_commandDecode (value & 0x3FFF, &command, &operand))
    {
      sc_regSet (flagE, 1);
      return -1;
    }
  printf ("%x", command);
  switch (command)
    {
    case 0x10:
      if (cu_read (operand))
        {
          return -1;
        }
      break;
    case 0x11:
      if (cu_write (operand))
        {
          return -1;
        }
      break;
    case 0x20:
      if (cu_load (operand))
        {
          return -1;
        }
      break;
    case 0x21:
      if (cu_store (operand))
        {
          return -1;
        }
      break;
    case 0x40:
      if (cu_jump (operand))
        {
          return -1;
        }
      break;
    case 0x41:
      if (cu_jneg (operand))
        {
          return -1;
        }
      break;
    case 0x42:
      if (cu_jz (operand))
        {
          return -1;
        }
      break;
    case 0x43:
      return -1;
    default:
      if (ALU (command, operand))
        {
          return -1;
        }
      break;
    }
  return 0;
}

int
cu_read (int operand)
{
  char buffer[10] = { 0 };
  int number;

  rk_myTermRestore ();

  mt_gotoXY (14, 23);

  read (0, buffer, sizeof (buffer));

  mt_gotoXY (14, 23);
  WRITE_STR ("                 ");
  mt_gotoXY (14, 23);

  fflush (0);

  rk_myTermSave ();

  rk_myTermRegime (0, 30, 0, 0, 0);

  if (buffer[0] != '+' && buffer[0] != '-')
    {
      return -1;
    }

  number = (int)strtol (&buffer[1], NULL, 16);

  if (number > 0x3FFF)
    {
      return -1;
    }

  if (buffer[0] == '-')
    {
      number = number | 0x4000;
    }

  sc_memorySet (operand, number);

  return 0;
}

int
cu_write (int operand)
{
  int value;
  int command;
  char buf[6];

  if (sc_memoryGet (operand, &value)
      || sc_commandDecode (value & 0x3FFF, &command, &operand))
    {
      sc_regSet (flagE, 1);
      return -1;
    }

  snprintf (buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command,
            operand);

  mt_gotoXY (14, 23);
  WRITE_STR ("                                                                "
             "                                                                "
             "              ");
  mt_gotoXY (14, 23);

  write (1, buf, 5);
  fflush (0);
  mt_gotoXY (19, 23);
  WRITE_STR ("                                                                "
             "                                                                "
             "              ");
  mt_gotoXY (14, 23);
  mt_gotoXY (19, 30);
  rk_myTermSave ();

  rk_myTermRegime (0, 30, 0, 0, 0);

  char buffer[1] = { 0 };

  read (0, buffer, sizeof (buffer));

  rk_myTermRestore ();

  mt_gotoXY (14, 23);
  WRITE_STR ("                                                                "
             "                                                                "
             "              ");
  mt_gotoXY (14, 23);

  return 0;
}

int
cu_load (int operand)
{
  int value;

  if (sc_memoryGet (operand, &value))
    {
      sc_regSet (flagE, 1);
      return -1;
    }

  ACCUM = value;

  return 0;
}

int
cu_store (int operand)
{
  if (operand < 0 || operand > 99)
    {
      sc_regSet (flagM, 1);
      return -1;
    }

  if (sc_memorySet (operand, ACCUM))
    {
      return -1;
    }

  return 0;
}

int
cu_jump (int operand)
{
  if (operand < 0 || operand > 99)
    {
      sc_regSet (flagM, 1);
      return -1;
    }

  IP = operand - 1;

  return 0;
}

int
cu_jneg (int operand)
{
  if ((ACCUM >> 14) & 1)
    {
      if (operand < 0 || operand > 99)
        {
          sc_regSet (flagM, 1);
          return -1;
        }

      IP = operand - 1;
    }

  return 0;
}

int
cu_jz (int operand)
{
  if (ACCUM == 0)
    {
      if (operand < 0 || operand > 99)
        {
          sc_regSet (flagM, 1);
          return -1;
        }

      IP = operand - 1;
    }

  return 0;
}
