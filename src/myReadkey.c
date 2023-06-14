#include "myReadkey.h"

struct termios SAVE_TERM;

int
rk_readKey (enum KEYS *key)
{
  char buffer[10];
  memset (buffer, 0, sizeof (char) * 2);
  fflush (stdin);

  read (fileno (stdin), buffer, sizeof (buffer));

  if (buffer[0] == '\E')
    {
      switch (buffer[1])
        {
        case '\0':
          *key = ESC_KEY;
          break;
        case '[':
          switch (buffer[2])
            {
            case 'A':
              *key = UP_KEY;
              break;
            case 'B':
              *key = DOWN_KEY;
              break;
            case 'C':
              *key = RIGHT_KEY;
              break;
            case 'D':
              *key = LEFT_KEY;
              break;
            case '1':
              if (buffer[3] == '7' && buffer[4] == '~')
                *key = F6_KEY;
              else if (buffer[3] == '5' && buffer[4] == '~')
                *key = F5_KEY;
              break;
            default:
              break;
            }
          break;
        default:
          break;
        }
    }
  else if (buffer[0] == '\n' && buffer[1] == '\0')
    {
      *key = ENTER_KEY;
    }
  else
    {
      switch (buffer[0])
        {
        case 'l':
          *key = L_KEY;
          break;
        case 's':
          *key = S_KEY;
          break;
        case 'r':
          *key = R_KEY;
          break;
        case 't':
          *key = T_KEY;
          break;
        case 'i':
          *key = I_KEY;
          break;
        default:
          break;
        }
    }

  return 0;
}

int
rk_myTermSave ()
{
  if (tcgetattr (fileno (stdin), &SAVE_TERM) == -1)
    return -1;
  return 0;
}

int
rk_myTermRestore ()
{
  if (tcsetattr (fileno (stdin), TCSAFLUSH, &SAVE_TERM) == -1)
    return -1;
  return 0;
}

int
rk_myTermRegime (int regime, int vtime, int vmin, int echo, int sigint)
{

  struct termios current;

  if (tcgetattr (fileno (stdin), &current) == -1)
    {
      perror ("tcgetattr");
      return -1;
    }

  if (regime == 0)
    {
      current.c_lflag &= ~ICANON;
      if (echo == 0)
        current.c_lflag &= ~ECHO;
      else
        current.c_lflag |= ECHO;
      if (sigint == 0)
        current.c_lflag &= ~ISIG;
      else
        current.c_lflag |= ISIG;
      current.c_cc[VMIN] = vmin;
      current.c_cc[VTIME] = vtime;
    }
  else
    {
      current.c_lflag |= ECHO;
      current.c_lflag |= ISIG;
      current.c_lflag |= ICANON;
    }

  if (tcsetattr (fileno (stdin), TCSAFLUSH, &current) == -1)
    {
      perror ("tcsetattr");
      return -1;
    }

  return 0;
}
