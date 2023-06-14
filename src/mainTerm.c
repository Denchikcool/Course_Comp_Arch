#include "myTerm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int
main ()
{
  int cursor_x = 0, cursor_y = 0;
  int row = 0, column = 0;
  int menu_key;
  int func_result;
  int inColor;

  while (1)
    {
      printf ("1) Clear screen\n");
      printf ("2) Move cursor to the certain point\n");
      printf ("3) Get size of screen\n");
      printf ("4) Set foreground color\n");
      printf ("5) Set background color\n");
      printf ("0) Exit\n");

      scanf ("%d", &menu_key);

      switch (menu_key)
        {
        case 0:
          exit (EXIT_SUCCESS);

        case 1:
          func_result = mt_clrscr ();
          if (func_result == -1)
            printf ("\nError while operation of cleaning screen\n");
          break;

        case 2:
          printf ("Enter coordinates to move cursor. (X) then (Y): ");
          scanf ("%d", &cursor_x);
          scanf ("%d", &cursor_y);
          func_result = mt_gotoXY (cursor_y, cursor_x);
          if (func_result == -1)
            printf ("Error while moving cursor");
          break;

        case 3:
          func_result = mt_getscreensize (&row, &column);
          if (func_result == -1)
            printf ("\nUnable to get screen size\n");
          break;

        case 4:
          printf ("Select color: ");
          printf ("Black - 0 ");
          printf ("Red - 1 ");
          printf ("Green - 2 ");
          printf ("Yellow - 3 ");
          printf ("Blue - 4 ");
          printf ("Purple - 5 ");
          printf ("Cyan - 6 ");
          printf ("White - 7 ");
          printf ("Enter foreground color: ");
          scanf ("%d", &inColor);
          func_result = mt_setfgcolor (inColor);
          if (func_result == -1)
            printf ("\nWrong color\n");
          break;

        case 5:
          printf ("Select color: ");
          printf ("Black - 0 ");
          printf ("Red - 1 ");
          printf ("Green - 2 ");
          printf ("Yellow - 3 ");
          printf ("Blue - 4 ");
          printf ("Purple - 5 ");
          printf ("Cyan - 6 ");
          printf ("White - 7 ");
          printf ("Enter background color: ");
          scanf ("%d", &inColor);
          func_result = mt_setbgcolor (inColor);
          if (func_result == -1)
            printf ("\nWrong color\n");
          break;

        default:
          exit (EXIT_FAILURE);
        }
    }
}
