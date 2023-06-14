#include "graphics.h"
#include "myALU.h"
#include "myBigChars.h"
#include "myCU.h"
#include "myReadkey.h"
#include "mySignal.h"
#include "mySimpleComputer.h"
#include "myTerm.h"

int
main (void)
{
  enum KEYS key;

  int ignore;
  struct itimerval nval, oval;

  int x = 1, y = 13;
  sc_memoryInit ();
  sc_regInit ();

  signal (SIGALRM, ms_defineSignal);
  signal (SIGUSR1, ms_defineSignal);
  signal (SIGUSR2, ms_defineSignal);

  nval.it_interval.tv_sec = 0;
  nval.it_interval.tv_usec = 50000;
  nval.it_value.tv_sec = 0;
  nval.it_value.tv_usec = 50000;

  HIGHLIGHT_RESET;

  int arr[18][2];
  int small_arr[36];
  int count;
  int fd;

  fd = open ("font.bin", O_RDONLY);
  bc_bigcharread (fd, small_arr, 18, &count);
  close (fd);

  int k = 0;
  for (int i = 0; i < 18; i++)
    {
      for (int j = 0; j < 2; j++)
        {
          arr[i][j] = small_arr[k];
          k++;
        }
    }

  ACCUM = 0x7FFF;
  IP = 0;
  SELECTED_ADDR = 5;

  sc_regSet (flagM, 1);
  sc_regSet (flagE, 1);
  sc_memorySet (4, 0x3FFF);
  sc_memorySet (5, 0x7FFF);
  char buffer[6];
  sc_regSet (flagT, 1);
  getMemoryCell (buffer);

  HIGHLIGHT_RESET;

  mt_clrscr ();

  drawMemoryBox ();
  drawMemoryContent ();

  drawAccumBox ();
  drawAccumContent ();

  drawIPBox ();
  drawIPContent ();

  drawOperationBox ();
  drawOperationContent ();

  drawFlagsBox ();
  drawFlagsContent ();

  drawBigCharsBox ();

  drawKeys ();

  drawBottom ();

  mt_gotoXY (14, 23);

  while (1)
    {

      drawMemoryContent ();
      drawAccumContent ();
      drawIPContent ();
      drawOperationContent ();
      drawFlagsContent ();

      getMemoryCell (buffer);

      mt_gotoXY (1, 13);
      x = 1;

      int symbol;
      for (int i = 0; i < 5; i++)
        {
          if (buffer[i] == '+')
            symbol = 0;
          else if (buffer[i] == '-')
            symbol = 1;
          else if (buffer[i] == '0')
            symbol = 2;
          else if (buffer[i] == '1')
            symbol = 3;
          else if (buffer[i] == '2')
            symbol = 4;
          else if (buffer[i] == '3')
            symbol = 5;
          else if (buffer[i] == '4')
            symbol = 6;
          else if (buffer[i] == '5')
            symbol = 7;
          else if (buffer[i] == '6')
            symbol = 8;
          else if (buffer[i] == '7')
            symbol = 9;
          else if (buffer[i] == '8')
            symbol = 10;
          else if (buffer[i] == '9')
            symbol = 11;
          else if (buffer[i] == 'A')
            symbol = 12;
          else if (buffer[i] == 'B')
            symbol = 13;
          else if (buffer[i] == 'C')
            symbol = 14;
          else if (buffer[i] == 'D')
            symbol = 15;
          else if (buffer[i] == 'E')
            symbol = 16;
          else if (buffer[i] == 'F')
            symbol = 17;
          bc_printbigchar (arr[symbol], x, y, WHITE, BLACK);
          x += 9;
        }

      mt_gotoXY (14, 23);

      mt_gotoXY (14, 23);

      rk_myTermSave ();

      key = EMPTY_KEY;
      rk_myTermRegime (0, 30, 0, 0, 0);
      rk_readKey (&key);
      rk_myTermRestore ();

      sc_regGet (flagT, &ignore);

      if (ignore)
        {
          switch (key)
            {
            case UP_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              (SELECTED_ADDR <= 9) ? (SELECTED_ADDR += 90)
                                   : (SELECTED_ADDR -= 10);
              break;
            case RIGHT_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              (!((SELECTED_ADDR + 1) % 10)) ? (SELECTED_ADDR -= 9)
                                            : (SELECTED_ADDR += 1);
              break;
            case DOWN_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");

              (SELECTED_ADDR >= 90) ? (SELECTED_ADDR -= 90)
                                    : (SELECTED_ADDR += 10);
              break;
            case LEFT_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              (!(SELECTED_ADDR % 10)) ? (SELECTED_ADDR += 9)
                                      : (SELECTED_ADDR -= 1);
              break;
            case L_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              mt_gotoXY (14, 23);
              rk_myTermRegime (1, 30, 0, 0, 0);
              loadProgram ();
              break;
            case S_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              mt_gotoXY (14, 23);
              rk_myTermRegime (1, 30, 0, 0, 0);
              saveProgram ();
              break;
            case R_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              mt_gotoXY (14, 23);
              sc_regSet (flagT, 0);
              setitimer (ITIMER_REAL, &nval, &oval);
              break;
            case T_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              mt_gotoXY (14, 23);
              rk_myTermRegime (1, 30, 0, 0, 0);
              sc_regSet (flagT, 0);
              raise (SIGUSR2);
              sc_regSet (flagT, 1);

              break;
            case I_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              mt_gotoXY (14, 23);
              rk_myTermRegime (1, 30, 0, 0, 0);
              raise (SIGUSR1);
              break;
            case F5_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");

              mt_gotoXY (14, 23);
              rk_myTermRegime (1, 30, 0, 0, 0);
              setAccum ();
              break;
            case F6_KEY:
              mt_gotoXY (14, 23);
              WRITE_STR ("                   ");
              mt_gotoXY (14, 23);
              rk_myTermRegime (1, 30, 0, 0, 0);
              setIP ();
              break;
            case ENTER_KEY:
              rk_myTermRegime (1, 30, 0, 0, 0);
              setValue ();
              break;
            case ESC_KEY:
              mt_clrscr ();
              rk_myTermRegime (1, 30, 0, 0, 0);
              free (memory);
              exit (EXIT_SUCCESS);
              break;
            case EMPTY_KEY:
              WRITE_STR ("Time out");
              break;
            }
        }
    }

  return 0;
}
