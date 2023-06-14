#include "myALU.h"
#include "mySimpleComputer.h"

int
ALU (int command, int operand)
{
  int value;

  switch (command)
    {
    case 0x30:
      if (sc_memoryGet (operand, &value))
        {
          sc_regSet (flagM, 1);
          return -1;
        }

      if (alu_add (value))
        {
          return -1;
        }

      break;
    case 0x31:
      if (sc_memoryGet (operand, &value))
        {
          sc_regSet (flagM, 1);
          return -1;
        }

      if (alu_sub (value))
        {
          return -1;
        }

      break;
    case 0x32:
      if (sc_memoryGet (operand, &value))
        {
          sc_regSet (flagM, 1);
          return -1;
        }

      if (alu_divide (value))
        {
          return -1;
        }

      break;
    case 0x33:
      if (sc_memoryGet (operand, &value))
        {
          sc_regSet (flagM, 1);
          return -1;
        }

      if (alu_mul (value))
        {
          return -1;
        }

      break;
    case 0x62:
      if (sc_memoryGet (operand, &value))
        {
          sc_regSet (flagM, 1);
          return -1;
        }

      if (rcl_ALU (value))
        {
          return -1;
        }

      break;
    default:
      sc_regSet (flagE, 1);
      return -1;
    }

  return 0;
}

int
alu_add (int value)
{
  int accum_copy;
  int value_copy;
  int res;

  accum_copy = ACCUM & 0x3FFF;

  if (ACCUM & 0x4000)
    {
      accum_copy = -accum_copy;
    }

  value_copy = value & 0x3FFF;

  if (value & 0x4000)
    {
      value_copy = -value_copy;
    }

  res = accum_copy + value_copy;

  if (res < 0)
    {
      res = -res;
      res |= 0x4000;
    }

  ACCUM = res & 0x7FFF;

  return 0;
}

int
alu_sub (int value)
{
  int accum_copy;
  int value_copy;
  int res;

  accum_copy = ACCUM & 0x3FFF;

  if (ACCUM & 0x4000)
    {
      accum_copy = -accum_copy;
    }

  value_copy = value & 0x3FFF;

  if (value & 0x4000)
    {
      value_copy = -value_copy;
    }

  res = accum_copy - value_copy;

  if (res < 0)
    {
      res = -res;
      res |= 0x4000;
    }

  ACCUM = res & 0x7FFF;

  return 0;
}

int
alu_divide (int value)
{
  int accum_copy;
  int value_copy;
  int res;

  if (value == 0)
    {
      sc_regSet (flag0, 1);
      return -1;
    }

  accum_copy = ACCUM & 0x3FFF;

  if (ACCUM & 0x4000)
    {
      accum_copy = -accum_copy;
    }

  value_copy = value & 0x3FFF;

  if (value & 0x4000)
    {
      value_copy = -value_copy;
    }

  res = accum_copy / value_copy;

  if (res < 0)
    {
      res = -res;
      res |= 0x4000;
    }

  ACCUM = res & 0x7FFF;

  return 0;
}

int
alu_mul (int value)
{
  int accum_copy;
  int value_copy;
  int res;

  accum_copy = ACCUM & 0x3FFF;

  if (ACCUM & 0x4000)
    {
      accum_copy = -accum_copy;
    }

  value_copy = value & 0x3FFF;

  if (value & 0x4000)
    {
      value_copy = -value_copy;
    }

  res = accum_copy * value_copy;

  if (res < 0)
    {
      res = -res;
      res |= 0x4000;
    }

  ACCUM = res & 0x7FFF;

  return 0;
}

int
rcl_ALU (int value)
{
  int res;

  res = value;

  res = (res << 1) | (res >> 14);

  ACCUM = res & 0x7FFF;

  return 0;
}
