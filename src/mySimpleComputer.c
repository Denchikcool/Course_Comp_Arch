#include "mySimpleComputer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint16_t *memory;

uint8_t flag;
uint16_t ACCUM;
uint8_t IP;

int
sc_memoryInit ()
{
  memory = (uint16_t *)calloc (SIZE, sizeof (uint16_t));
  return 0;
}

int
sc_memorySet (int address, int value)
{
  if (address >= 0 && address < SIZE)
    {
      memory[address] = (uint16_t)value;
      return 0;
    }
  else
    {
      return -1;
    }
}

int
sc_memoryGet (int address, int *value)
{
  if (address >= 0 && address < SIZE)
    {
      *value = (int)memory[address];
      return 0;
    }
  else
    {
      return -1;
    }
}

int
sc_memorySave (char *filename)
{
  FILE *file = fopen (filename, "wb");
  if (fwrite (memory, sizeof (uint16_t), SIZE, file))
    {
      fclose (file);
      return 0;
    }
  else
    {
      return -1;
    }
}

int
sc_memoryLoad (char *filename)
{
  FILE *file = fopen (filename, "rb");
  if (fread (memory, sizeof (uint16_t), SIZE, file))
    {
      fclose (file);
      return 0;
    }
  else
    {
      return -1;
    }
}

int
sc_regInit ()
{
  flag = 0;
  return 0;
}

int
sc_regSet (int reg, int value)
{
  if (value == 0 || value == 1)
    {
      switch (reg)
        {
        case flagP:
          if (value)
            flag |= flagP;
          else
            flag &= ~flagP;
          break;
        case flag0:
          if (value)
            flag |= flag0;
          else
            flag &= ~flag0;
          break;
        case flagM:
          if (value)
            flag |= flagM;
          else
            flag &= ~flagM;
          break;
        case flagT:
          if (value)
            flag |= flagT;
          else
            flag &= ~flagT;
          break;
        case flagE:
          if (value)
            flag |= flagE;
          else
            flag &= ~flagE;
          break;
        default:
          printf ("Incorrect register");
          return -1;
        }
      return 0;
    }

  else
    {
      printf ("Incorrect value of register");
      return -1;
    }
}

int
sc_regGet (int reg, int *value)
{
  switch (reg)
    {
    case flagP:
      *value = flag & flagP;
      break;
    case flag0:
      *value = flag & flag0;
      break;
    case flagM:
      *value = flag & flagM;
      break;
    case flagT:
      *value = flag & flagT;
      break;
    case flagE:
      *value = flag & flagE;
      break;
    default:
      printf ("Incorrect register");
      return -1;
    }
  return 0;
}

int
sc_commandEncode (int command, int operand, int *value)
{
  if ((operand >= 0) && (operand < 128))
    {
      *value = 0b011111111111111 & ((command << 7) | operand);
      return 0;
    }
  else
    return -1;
}

int
sc_commandDecode (int value, int *command, int *operand)
{
  if (!(value >> 14))
    {
      *operand = value & 0b01111111;
      *command = (value >> 7) & 0b01111111;
      return 0;
    }
  else
    {
      return -1;
    }
}
