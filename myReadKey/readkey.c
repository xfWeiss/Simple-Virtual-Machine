#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "../include/myReadKey.h"
#include "../include/myTerm.h"

int
rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint)
{
  struct termios term;
  if (tcgetattr (STDIN_FILENO, &term) != 0)
    {
      return -1;
    }

  if (regime)
    {
      term.c_lflag |= ICANON;
    }
  else if (!regime)
    {
      term.c_lflag &= ~ICANON;
    }
  else
    {
      return -1;
    }

  if (!regime)
    {
      term.c_cc[VTIME] = vtime;
      term.c_cc[VMIN] = vmin;
    }

  if (echo == 1)
    {
      term.c_lflag |= ECHO;
    }
  else if (!echo)
    {
      term.c_lflag &= ~ECHO;
    }
  else
    {
      return -1;
    }

  if (sigint)
    {
      term.c_lflag |= ISIG;
    }
  else if (!sigint)
    {
      term.c_lflag &= ~ISIG;
    }
  else
    {
      return -1;
    }

  if (tcsetattr (STDIN_FILENO, TCSANOW, &term) != 0)
    {
      return -1;
    }
  return 0;
}

int
rk_readkey (keys *key)
{
  if (rk_mytermregime (0, 0, 1, 0, 1) != 0)
    {
      return -1;
    }

  char buf[16];
  int numRead;

  numRead = read (STDIN_FILENO, buf, sizeof (buf) - 1);
  buf[numRead] = '\0';

  if (strcmp (buf, "\033") == 0)
    {
      *key = KEY_ESC;
    }
  else if ((strcmp (buf, "\033[15~") == 0) || (strcmp (buf, "\033[[E") == 0))
    {
      *key = KEY_F5;
    }
  else if (strcmp (buf, "\033[17~") == 0)
    {
      *key = KEY_F6;
    }
  else if (strcmp (buf, "\033[A") == 0)
    {
      *key = KEY_UP;
    }
  else if (strcmp (buf, "\033[B") == 0)
    {
      *key = KEY_DOWN;
    }
  else if (strcmp (buf, "\033[C") == 0)
    {
      *key = KEY_RIGHT;
    }
  else if (strcmp (buf, "\033[D") == 0)
    {
      *key = KEY_LEFT;
    }
  else if ((strcmp (buf, "\n") == 0) || (strcmp (buf, "\r") == 0))
    {
      *key = KEY_ENTER;
    }

  else if (strcmp (buf, "l") == 0 || strcmp (buf, "L") == 0)
    {
      *key = KEY_L;
    }
  else if (strcmp (buf, "s") == 0 || strcmp (buf, "S") == 0)
    {
      *key = KEY_S;
    }
  else if (strcmp (buf, "i") == 0 || strcmp (buf, "I") == 0)
    {
      *key = KEY_I;
    }
  else if (strcmp (buf, "r") == 0 || strcmp (buf, "R") == 0)
    {
      *key = KEY_R;
    }
  else if (strcmp (buf, "t") == 0 || strcmp (buf, "T") == 0)
    {
      *key = KEY_T;
    }
  else if (strcmp (buf, "+") == 0)
    {
      *key = KEY_PLUS;
    }
  else if (strcmp (buf, "-") == 0)
    {
      *key = KEY_MINUS;
    }
  else if (strcmp (buf, "0") == 0)
    {
      *key = KEY_0;
    }
  else if (strcmp (buf, "1") == 0)
    {
      *key = KEY_1;
    }
  else if (strcmp (buf, "2") == 0)
    {
      *key = KEY_2;
    }
  else if (strcmp (buf, "3") == 0)
    {
      *key = KEY_3;
    }
  else if (strcmp (buf, "4") == 0)
    {
      *key = KEY_4;
    }
  else if (strcmp (buf, "5") == 0)
    {
      *key = KEY_5;
    }
  else if (strcmp (buf, "6") == 0)
    {
      *key = KEY_6;
    }
  else if (strcmp (buf, "7") == 0)
    {
      *key = KEY_7;
    }
  else if (strcmp (buf, "8") == 0)
    {
      *key = KEY_8;
    }
  else if (strcmp (buf, "9") == 0)
    {
      *key = KEY_9;
    }
  else if (strcmp (buf, "a") == 0 || strcmp (buf, "A") == 0)
    {
      *key = KEY_A;
    }
  else if (strcmp (buf, "b") == 0 || strcmp (buf, "B") == 0)
    {
      *key = KEY_B;
    }
  else if (strcmp (buf, "c") == 0 || strcmp (buf, "C") == 0)
    {
      *key = KEY_C;
    }
  else if (strcmp (buf, "d") == 0 || strcmp (buf, "D") == 0)
    {
      *key = KEY_D;
    }
  else if (strcmp (buf, "e") == 0 || strcmp (buf, "E") == 0)
    {
      *key = KEY_E;
    }
  else if (strcmp (buf, "f") == 0 || strcmp (buf, "F") == 0)
    {
      *key = KEY_F;
    }
  else
    {
      *key = KEY_UNKNOWN;
    }

  if (rk_mytermregime (1, 0, 1, 1, 1) != 0)
    {
      return -1;
    }

  return 0;
}

int
rk_mytermsave (void)
{
  struct termios opt;
  FILE *toSave;
  if (tcgetattr (STDIN_FILENO, &opt) != 0)
    {
      return -1;
    }
  if ((toSave = fopen ("termSettings", "wb")) == NULL)
    {
      return -1;
    }
  fwrite (&opt, sizeof (opt), 1, toSave);
  fclose (toSave);
  return 0;
}

int
rk_mytermrestore (void)
{
  struct termios opt;
  FILE *toSave;
  if ((toSave = fopen ("termsettings", "rb")) == NULL)
    {
      return -1;
    }
  if (fread (&opt, sizeof (opt), 1, toSave) > 0)
    {
      if (tcsetattr (STDIN_FILENO, TCSAFLUSH, &opt) != 0)
        {
          return -1;
        }
    }
  else
    {
      return -1;
    }
  return 0;
}

int
rk_readvalue (int *value, int timeout)
{
  mt_setcursorvisible (1);

  keys key;
  char input[6];
  int index = 0;

  while (index < sizeof (input) - 1)
    {
      rk_readkey (&key);

      switch (key)
        {
        case KEY_ENTER:
          input[index] = '\0';
          break;
        case KEY_ESC:
          input[0] = '\0';
          break;
        case KEY_PLUS:
          if (index == 0)
            {
              input[index++] = '+';
            }
          break;
        case KEY_MINUS:
          if (index == 0)
            {
              input[index++] = '-';
            }
          break;
        case KEY_0:
        case KEY_1:
        case KEY_2:
        case KEY_3:
        case KEY_4:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_8:
        case KEY_9:
          if (index > 0)
            {
              input[index++] = '0' + (key - KEY_0);
            }
          break;
        case KEY_A:
        case KEY_B:
        case KEY_C:
        case KEY_D:
        case KEY_E:
        case KEY_F:
          if (index > 0)
            {
              input[index++] = 'A' + (key - KEY_A);
            }
          break;
        default:
          break;
        }

      if (key == KEY_ENTER || key == KEY_ESC)
        {
          break;
        }

      write (STDOUT_FILENO, &input[index - 1], 1);

      if (timeout > 0)
        {
          sleep (60);
          timeout -= 100;
        }
    }

  if (input[0] == '\0' || index < 2)
    {
      return -1;
    }

  *value = (input[0] == '+') ? strtol (input + 1, NULL, 16)
                             : -strtol (input + 1, NULL, 16);

  mt_setcursorvisible (0);
  return 0;
}
