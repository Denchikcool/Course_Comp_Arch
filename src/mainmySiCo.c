#include "mySimpleComputer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  int address, value, command, operand, reg;
  char filename;
  int menu_key;
  int pause, func_result;

  sc_regInit ();
  sc_memoryInit ();

  while (1)
    {
      system ("clear");
      printf ("1) Set value of certain RAM cell\n");
      printf ("2) Get value of certain RAM cell\n");
      printf ("3) Save RAM data as file\n");
      printf ("4) Load RAM data from file\n");
      printf ("5) Set value of certain flag register\n");
      printf ("6) Get value of cetrain flag register\n");
      printf ("7) Encode command and operand by certain value\n");
      printf ("8) Decode command and operand from certain value\n");
      printf ("0) Exit\n");

      scanf ("%d", &menu_key);

      switch (menu_key)
        {
        case 0:
          exit (EXIT_SUCCESS);

        case 1:
          system ("clear");
          printf ("Enter adress of RAM (0 - %d): ", SIZE);
          scanf ("%d", &address);
          printf ("\nEnter value you want to set: ");
          scanf ("%d", &value);
          func_result = sc_memorySet (address, value);
          if (func_result)
            {
              printf ("\nStack out of area\n");
              while (1)
                {
                  printf ("\nEnter number from 0 to 9 to continue: ");
                  scanf ("%d", &pause);
                  if (pause < 10 && pause >= 0)
                    break;
                }
            }
          break;

        case 2:
          system ("clear");
          printf ("Enter address of RAM you want to get: ");
          scanf ("%d", &address);
          func_result = sc_memoryGet (address, &value);
          if (!func_result)
            printf ("\nValue: %d\n", value);
          else
            printf ("\nStack out of area\n");
          while (1)
            {
              printf ("\nEnter number from 0 to 9 to continue: ");
              scanf ("%d", &pause);
              if (pause < 10 && pause >= 0)
                break;
            }
          break;

        case 3:
          system ("clear");
          printf ("Enter new file name: ");
          scanf ("%s", &filename);
          func_result = sc_memorySave (&filename);
          if (func_result)
            {
              printf ("\nCan't write into file\n");
              while (1)
                {
                  printf ("\nEnter number from 0 to 9 to continue: ");
                  scanf ("%d", &pause);
                  if (pause < 10 && pause >= 0)
                    break;
                }
            }
          break;

        case 4:
          system ("clear");
          printf ("Enter exist file name: ");
          scanf ("%s", &filename);
          func_result = sc_memoryLoad (&filename);
          if (func_result)
            {
              printf ("\nCan't read file\n");
              while (1)
                {
                  printf ("\nEnter number from 0 to 9 to continue: ");
                  scanf ("%d", &pause);
                  if (pause < 10 && pause >= 0)
                    break;
                }
            }
          break;

        case 5:
          system ("clear");
          printf ("Flag of overflow (1)\n");
          printf ("Flag of div by 0 (2)\n");
          printf ("Flag of stack out of area (4)\n");
          printf ("Flag of ignoring clock impulses (8)\n");
          printf ("Flag of incorrect command (16)\n");
          printf ("Enter flag register: ");
          scanf ("%d", &reg);
          printf ("\nEnter value of flag: ");
          scanf ("%d", &value);
          func_result = sc_regSet (reg, value);
          if (func_result)
            {
              printf ("\nIncorrect register/value of register\n");
              while (1)
                {
                  printf ("\nEnter number from 0 to 9 to continue: ");
                  scanf ("%d", &pause);
                  if (pause < 10 && pause >= 0)
                    break;
                }
            }
          break;

        case 6:
          system ("clear");
          printf ("Flag of overflow (1)\n");
          printf ("Flag of div by 0 (2)\n");
          printf ("Flag of stack out of area (4)\n");
          printf ("Flag of ignoring clock impulses (8)\n");
          printf ("Flag of incorrect command (16)\n");
          printf ("Enter register the value you want to get: ");
          scanf ("%d", &reg);
          func_result = sc_regGet (reg, &value);
          if (!func_result)
            printf ("\nValue = %d\n", value);
          else
            printf ("\nIncorrect register\n");
          while (1)
            {
              printf ("\nEnter number from 0 to 9 to continue: ");
              scanf ("%d", &pause);
              if (pause < 10 && pause >= 0)
                break;
            }
          break;

        case 7:
          system ("clear");
          printf ("Enter code of command: ");
          scanf ("%d", &command);
          printf ("\nEnter operand: ");
          scanf ("%d", &operand);
          func_result = sc_commandEncode (command, operand, &value);
          if (!func_result)
            printf ("\nEncoded value: %d\n", value);
          else
            printf ("\nIncorrect value of command/operand\n");
          while (1)
            {
              printf ("\nEnter number from 0 to 9 to continue: ");
              scanf ("%d", &pause);
              if (pause < 10 && pause >= 0)
                break;
            }
          break;

        case 8:
          system ("clear");
          printf ("Enter value you want to decode: ");
          scanf ("%d", &value);
          func_result = sc_commandDecode (value, &command, &operand);
          if (!func_result)
            {
              printf ("\nEncoded command: %d\n", command);
              printf ("\nEncoded operand: %d\n", operand);
            }
          else
            printf ("\nIncorrect encoded value\n");
          while (1)
            {
              printf ("\nEnter number from 0 to 9 to continue: ");
              scanf ("%d", &pause);
              if (pause < 10 && pause >= 0)
                break;
            }
          break;

        default:
          exit (EXIT_FAILURE);
        }
    }

  return 0;
}
