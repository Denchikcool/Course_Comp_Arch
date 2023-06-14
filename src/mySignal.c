#include "mySignal.h"
#include "graphics.h"
#include "myALU.h"
#include "myBigChars.h"
#include "myCU.h"
#include "mySimpleComputer.h"
#include "myTerm.h"

void
ms_defineSignal (int signal)
{
  int ignore;
  int CUresult;

  switch (signal)
    {
    case SIGALRM:
      sc_regGet (flagT, &ignore);

      if (ignore == 0)
        {
          CUresult = CU ();

          if (CUresult)
            {
              ms_disableSignal ();
            }
          else
            {
              if (++IP == 100)
                {
                  IP--;
                  ms_disableSignal ();
                }
            }
          drawMemoryContent ();
          drawAccumContent ();
          drawIPContent ();
          drawOperationContent ();
          drawFlagsContent ();
          mt_gotoXY (14, 23);
        }
      break;
    case SIGUSR1:
      free (memory);
      sc_memoryInit ();
      IP = 0;
      ACCUM = 0;
      flag = 0;
      sc_regSet (flagT, 1);
      break;
    case SIGUSR2:
      sc_regGet (flagT, &ignore);

      if (ignore == 0)
        {
          CUresult = CU ();

          if (CUresult)
            {
              sc_regSet (flagT, 1);
            }
          else
            {
              if (++IP == 100)
                {
                  IP--;
                  sc_regSet (flagT, 1);
                }
            }
        }
      break;
    }
}

void
ms_disableSignal ()
{
  sc_regSet (flagT, 1);
  alarm (0);
}
