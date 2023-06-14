#include "../src/mySimpleComputer.h"
#include <string.h>

FILE *input = NULL;

void
load (char *filename)
{
  input = fopen (filename, "r");
}

int
Command (char *command)
{
  int result = 0;
  if (strcmp (command, "READ") == 0)
    result = 0x10;
  else if (strcmp (command, "WRITE") == 0)
    result = 0x11;
  else if (strcmp (command, "LOAD") == 0)
    result = 0x20;
  else if (strcmp (command, "STORE") == 0)
    result = 0x21;
  else if (strcmp (command, "ADD") == 0)
    result = 0x30;
  else if (strcmp (command, "SUB") == 0)
    result = 0x31;
  else if (strcmp (command, "DIVIDE") == 0)
    result = 0x32;
  else if (strcmp (command, "MUL") == 0)
    result = 0x33;
  else if (strcmp (command, "JUMP") == 0)
    result = 0x40;
  else if (strcmp (command, "JNEG") == 0)
    result = 0x41;
  else if (strcmp (command, "JZ") == 0)
    result = 0x42;
  else if (strcmp (command, "HALT") == 0)
    result = 0x43;
  else if (strcmp (command, "AND") == 0)
    result = 0x52;
  else if (strcmp (command, "LOGRC") == 0)
    result = 0x68;
  else
    printf ("Wrong command: %s\n", command);
  return result;
}

void
translation (char *filename)
{
  for (int i = 0; !feof (input); i++)
    {
      char line[255];
      if (!fgets (line, 255, input))
        {
          if (feof (input))
            break;
          else
            printf ("Error while reading string %d\n", i);
        }

      char *ptr = strtok (line, " ");
      char *str_addr = ptr;
      int addr = atoi (str_addr);
      if (addr < 0 || addr > 99)
        printf ("Error of address in string %d\n", i);
      ptr = strtok (NULL, " ");
      char *command = ptr;

      ptr = strtok (NULL, " +");
      char *str_operand = ptr;

      ptr = strtok (NULL, " ");

      if (ptr != NULL && ptr[0] != ';')
        printf ("Wrong symbol in string %d\n", i);

      int operand = atoi (str_operand);
      char buffer[255];
      sprintf (buffer, "%02x", operand);
      sscanf (buffer, "%02x", &operand);

      if (command[0] == '=')
        {
          sscanf (str_operand, "%x", &operand);
          sc_memorySet (addr, operand);
          continue;
        }
      int num_of_cmd = Command (command);

      int value = 0;
      if (sc_commandEncode (num_of_cmd, operand, &value) < 0)
        printf ("Error while encode in string %d\n", i);
      sc_memorySet (addr, value);
    }

  sc_memorySave (strdup (filename));
}

int
main (int argc, char **argv)
{
  if (argc < 3)
    {
      printf (
          "Example of the correct launch: ./tsa <input.txt> <output.txt>\n");
      printf ("Creating default files like: './translators/AsCode.sa' and "
              "'./memory/memory.bin'\n");
      sc_memoryInit ();
      load ("./translators/AsCode.sa");
      translation ("./Saved_RAM.bin");
      return 0;
    }
  sc_memoryInit ();
  load (argv[1]);
  translation (argv[2]);
  return 0;
}
