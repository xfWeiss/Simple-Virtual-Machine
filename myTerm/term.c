#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "myTerm.h"

int
mt_clrscr (void)
{
  const char *clear_screen = "\033[H\033[2J";
  write (STDOUT_FILENO, clear_screen, strlen (clear_screen));
  return 0;
}

int
mt_getscreensize (int *rows, int *cols)
{
  struct winsize w;
  if (!ioctl (STDOUT_FILENO, TIOCGWINSZ, &w))
    {
      *rows = w.ws_row;
      *cols = w.ws_col;
      return 0;
    }
  return -1;
}

int
mt_gotoXY (int x, int y)
{
  int rows, cols;
  mt_getscreensize (&rows, &cols);
  if ((y < rows) && (x < cols) && (x >= 0) && (y >= 0))
    {
      char buf[30];
      int len = snprintf (buf, sizeof (buf), "\033[%d;%dH", y, x);
      write (STDOUT_FILENO, buf, len);
      return 0;
    }
  return -1;
}

int
mt_setfgcolor (colors color)
{
  const char *color_code;
  switch (color)
    {
    case black:
      color_code = "\033[30m";
      break;
    case red:
      color_code = "\033[31m";
      break;
    case green:
      color_code = "\033[32m";
      break;
    case blue:
      color_code = "\033[34m";
      break;
    case brown:
      color_code = "\033[33m";
      break;
    case magenta:
      color_code = "\033[35m";
      break;
    case cyan:
      color_code = "\033[36m";
      break;
    case white:
      color_code = "\033[37m";
      break;
    case defclr:
      color_code = "\033[39m";
      break;
    default:
      return -1;
    }
  write (STDOUT_FILENO, color_code, strlen (color_code));
  return 0;
}

int
mt_setbgcolor (colors color)
{
  const char *color_code;
  switch (color)
    {
    case black:
      color_code = "\033[40m";
      break;
    case red:
      color_code = "\033[41m";
      break;
    case green:
      color_code = "\033[42m";
      break;
    case blue:
      color_code = "\033[44m";
      break;
    case brown:
      color_code = "\033[43m";
      break;
    case magenta:
      color_code = "\033[45m";
      break;
    case cyan:
      color_code = "\033[46m";
      break;
    case white:
      color_code = "\033[47m";
      break;
    case defclr:
      color_code = "\033[49m";
      break;
    default:
      return -1;
    }
  write (STDOUT_FILENO, color_code, strlen (color_code));
  return 0;
}

int
mt_setdefaultcolor (void)
{
  const char *reset_color = "\033[0m";
  write (STDOUT_FILENO, reset_color, strlen (reset_color));
  return 0;
}

int
mt_setcursorvisible (int value)
{
  const char *cursor_visibility = value ? "\033[?25h" : "\033[?25l";
  write (STDOUT_FILENO, cursor_visibility, strlen (cursor_visibility));
  return 0;
}

int
mt_delline (void)
{
  const char *clear_line = "\033[K";
  write (STDOUT_FILENO, clear_line, strlen (clear_line));
  return 0;
}
