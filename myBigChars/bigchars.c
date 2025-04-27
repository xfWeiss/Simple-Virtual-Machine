#include <string.h>
#include <unistd.h>

#include "myBigChars.h"

int
bc_strlen (char *str)
{
  if (str == NULL)
    {
      return 0;
    }

  int count = 0;
  unsigned char *s = (unsigned char *)str;

  while (*s)
    {
      if ((*s & 0x80) == 0) // 1 байт
        {
          s += 1;
        }
      else if ((*s & 0xE0) == 0xC0) // 2 байта
        {
          s += 2;
        }
      else if ((*s & 0xF0) == 0xE0) // 3 байта
        {
          s += 3;
        }
      else if ((*s & 0xF8) == 0xF0) // 4 байта
        {
          s += 4;
        }
      else // Некорректный символ
        {
          return 0;
        }
      count++;
    }
  return count;
}

int
bc_printA (char *str)
{
  if (str == NULL)
    {
      return -1;
    }
  int len = strlen (str);
  write (STDOUT_FILENO, "\033(0", 3);
  if (write (STDOUT_FILENO, str, len) != len)
    {
      return -1;
    }
  write (STDOUT_FILENO, "\033(B", 3);
  return 0;
}

int
bc_box (int x1, int y1, int x2, int y2, colors box_fg, colors box_bg,
        char *header, colors header_fg, colors header_bg)
{
  if (header == NULL)
    {
      return -1;
    }

  int maxX, maxY;
  int len_utf8 = bc_strlen (header);
  int len_ascii = strlen (header);
  int i;

  if (x1 > x2)
    {
      x1 = x1 + x2;
      x2 = x1 - x2;
      x1 = x1 - x2;
    }
  if (y1 > y2)
    {
      y1 = y1 + y2;
      y2 = y1 - y2;
      y1 = y1 - y2;
    }

  mt_getscreensize (&maxY, &maxX);
  if (x1 < 0 || y1 < 0 || x2 > maxX || y2 > maxY || x2 - x1 < 2 || y2 - y1 < 2)
    {
      return -1;
    }

  mt_setfgcolor (box_fg);
  mt_setbgcolor (box_bg);

  mt_gotoXY (x1, y1);
  bc_printA (BOXCH_TL);
  for (i = x1 + 1; i < x2; i++)
    {
      bc_printA (BOXCH_HOR);
    }
  bc_printA (BOXCH_TR);
  for (i = y1 + 1; i < y2; i++)
    {
      mt_gotoXY (x1, i);
      bc_printA (BOXCH_VERT);
      mt_gotoXY (x2, i);
      bc_printA (BOXCH_VERT);
    }
  mt_gotoXY (x1, y2);
  bc_printA (BOXCH_BL);
  for (i = x1 + 1; i < x2; i++)
    {
      bc_printA (BOXCH_HOR);
    }
  bc_printA (BOXCH_BR);

  mt_setfgcolor (header_fg);
  mt_setbgcolor (header_bg);

  if (len_ascii == len_utf8)
    {
      mt_gotoXY (x1 + 1 + ((x2 - x1) / 2) - (len_ascii + 1) / 2, y1);
    }
  else
    {
      mt_gotoXY (x1 + 1 + ((x2 - x1) / 2) - (len_utf8 + 1) / 2, y1);
    }
  if (write (STDOUT_FILENO, header, len_ascii) != len_ascii)
    {
      mt_setdefaultcolor ();
      return -1;
    }
  mt_setdefaultcolor ();
  return 0;
}

int
bc_setbigcharpos (int *big, int x, int y, int value)
{
  if (big == NULL)
    {
      return -1;
    }
  if (value == 0)
    {
      big[y / 4] = big[y / 4] & ~(1 << (x + ((y % 4) * 8)));
    }
  else
    {
      big[y / 4] = big[y / 4] | (1 << (x + ((y % 4) * 8)));
    }
  return 0;
}

int
bc_getbigcharpos (int *big, int x, int y, int *value)
{
  if (value == NULL || big == NULL)
    {
      return -1;
    }
  *value = (big[y / sizeof (int)] >> (x + ((y % 4) * 8))) & 1;
  return 0;
}

int
bc_printbigchar (int big[2], int x, int y, colors fg, colors bg)
{
  int rows;
  int cols;
  mt_getscreensize (&rows, &cols);

  if (x + 8 > cols || y + 8 > rows || big == NULL)
    {
      return -1;
    }

  mt_gotoXY (x, y);
  mt_setfgcolor (fg);
  mt_setbgcolor (bg);

  for (int i = 0; i < 2 * sizeof (int); i++)
    {
      for (int j = 0; j < 8; j++)
        {
          int flag = 0;
          bc_getbigcharpos (big, j, i, &flag);

          if (flag == 1)
            {
              mt_gotoXY (x + j, y + i);
              bc_printA (BOXCH_REC);
            }
          else
            {
              mt_gotoXY (x + j, y + i);
              bc_printA (" ");
            }
        }
    }
  mt_setdefaultcolor ();
  return 0;
}

int
bc_bigcharwrite (int fd, int *big, int count)
{
  if (big == NULL)
    {
      return -1;
    }
  int n = write (fd, big, count * sizeof (int) * 2);
  return (n == -1) ? -1 : 0;
}

int
bc_bigcharread (int fd, int *big, int need_count, int *count)
{
  if (big == NULL || count == NULL)
    {
      return -1;
    }

  *count = read (fd, big, need_count * sizeof (int) * 2) / (sizeof (int) * 2);
  return 0;
}
