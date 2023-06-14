#include "graphics.h"
#include "myBigChars.h"
#include "mySimpleComputer.h"
#include "myTerm.h"

int
main (void)
{
  int x = 1, y = 1;
  sc_memoryInit ();
  sc_regInit ();
  int fd;
  int count;
  int arr[18][2];
  int big_arr[18][2];
  int small_arr[36];

  for (int i = 0; i < 18; i++)
    for (int j = 0; j < 2; j++)
      arr[i][j] = 0;

  sc_memorySet (5, 0x7FFF);
  char buffer[6];
  SELECTED_ADDR = 5;
  getMemoryCell (buffer);

  mt_clrscr ();

  for (int i = 0; i < 5; i++)
    {

      drawFont (arr, 1, 1, '+', x, y);
      drawFont (arr, 1, 1, '-', x, y);
      drawFont (arr, 1, 1, '0', x, y);
      drawFont (arr, 1, 1, '1', x, y);
      drawFont (arr, 1, 1, '2', x, y);
      drawFont (arr, 1, 1, '3', x, y);
      drawFont (arr, 1, 1, '4', x, y);
      drawFont (arr, 1, 1, '5', x, y);
      drawFont (arr, 1, 1, '6', x, y);
      drawFont (arr, 1, 1, '7', x, y);
      drawFont (arr, 1, 1, '8', x, y);
      drawFont (arr, 1, 1, '9', x, y);
      drawFont (arr, 1, 1, 'A', x, y);
      drawFont (arr, 1, 1, 'B', x, y);
      drawFont (arr, 1, 1, 'C', x, y);
      drawFont (arr, 1, 1, 'D', x, y);
      drawFont (arr, 1, 1, 'E', x, y);
      drawFont (arr, 1, 1, 'F', x, y);
    }

  fd = open ("font.bin", O_RDONLY, S_IREAD);
  bc_bigcharread (fd, small_arr, 18, &count);
  close (fd);

  int k = 0;
  for (int i = 0; i < 18; i++)
    {
      for (int j = 0; j < 2; j++)
        {
          big_arr[i][j] = small_arr[k];
          k++;
        }
    }

  bc_printbigchar (big_arr[12], x, y, RED, BLACK);

  return 0;
}
