#include "myReadkey.h"
#include "mySimpleComputer.h"

int
main (void)
{
  enum KEYS key = EMPTY_KEY;
  int value;

  sc_memoryInit ();
  sc_memorySet (0, 5);
  sc_memoryGet (0, &value);

  printf ("%d\n", value);

  rk_myTermSave ();

  rk_myTermRegime (0, 30, 0, 0, 0);
  rk_readKey (&key);
  rk_myTermRestore ();
  printf ("%d", key);

  return 0;
}
