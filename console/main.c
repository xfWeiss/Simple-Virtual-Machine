#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "myBigChars.h"
#include "myReadKey.h"
#include "mySimpleComputer.h"
#include "myTerm.h"

void printCell (int address, colors fg, colors bg);
void printBigCell (int address, int bigchars[18][2]);
void printfRAM (void);
void printFlags (void);
void printDecodedCommand (int value);
void printAccumulator (void);
void printCounters (void);
void printTerm (int address, int input);
void printCommand (void);
void printBox (void);

void memoryInput (int address);
void memorySave (void);
void memoryLoad (void);
void reset (void);
void accumulatorInput (void);
void icounterInput (void);

int
main (int argc, char const *argv[])
{
  if (argc > 2)
    {
      fprintf (stderr,
               "\033[31m[Ошибка]\033[39m Допустимо не более 1-го аргумента "
               "командной строки!\n"
               "Используйте: %s [filename_font.bin]\n",
               argv[0]);
      return 1;
    }

  if (!isatty (STDOUT_FILENO))
    {
      fprintf (stderr,
               "\033[31m[Ошибка]\033[39m Стандартный поток ввода-вывода не "
               "соответствует терминалу!\n");
      return 1;
    }

  int rows, cols;
  mt_getscreensize (&rows, &cols);
  if (rows < 27 || cols < 110)
    {
      fprintf (stderr,
               "\033[31m[Ошибка]\033[39m Размер терминала слишком мал!\n");
      return 1;
    }

  FILE *fp;
  if (argc > 1)
    {
      fp = fopen (argv[1], "rb");
    }
  else
    {
      fp = fopen ("console/fonts/font.bin", "rb"); // Шрифт по умолчанию
    }
  if (fp == NULL)
    {
      fprintf (stderr, "\033[31m[Ошибка]\033[39m Не удалось открыть файл с "
                       "шрифтом больших символов!\n");
      return 1;
    }

  int bigchars[18][BIGCH_SIZE];
  fread (bigchars, sizeof (int), 18 * BIGCH_SIZE, fp);
  fclose (fp);

  sc_memoryInit ();
  sc_regInit ();
  sc_accumulatorInit ();
  sc_icounterInit ();

  mt_clrscr ();
  mt_setcursorvisible (0);
  printBox ();
  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      printCell (i, defclr, defclr);
    }

  int value;
  int curCell = 0, prevCell = -1;
  keys key;
  do
    {
      if (prevCell != -1)
        {
          printCell (prevCell, defclr, defclr);
        }
      printCell (curCell, black, white);
      printBigCell (curCell, bigchars);
      sc_memoryGet (curCell, &value);
      printDecodedCommand (value);
      printAccumulator ();
      printFlags ();
      printCounters ();
      printCommand ();
      prevCell = curCell;

      rk_readkey (&key);

      switch (key)
        {
        case KEY_UP:
          if (curCell >= 10)
            {
              curCell -= 10;
            }
          break;
        case KEY_DOWN:
          if (curCell < 118)
            {
              curCell += 10;
            }
          break;
        case KEY_LEFT:
          if (curCell % 10 > 0)
            {
              curCell -= 1;
            }
          break;
        case KEY_RIGHT:
          if (curCell % 10 < 9 && curCell < 120)
            {
              curCell += 1;
            }
          else if (curCell > 119 && curCell < 127)
            {
              curCell += 1;
            }
          break;
        case KEY_ENTER:
          memoryInput (curCell);
          break;
        case KEY_S:
          memorySave ();
          break;
        case KEY_L:
          memoryLoad ();
          break;
        case KEY_I:
          reset ();
          curCell = 0;
          break;
        case KEY_R:
          break;
        case KEY_T:
          break;
        case KEY_F5:
          printCell (curCell, defclr, defclr);
          accumulatorInput ();
          break;
        case KEY_F6:
          printCell (curCell, defclr, defclr);
          icounterInput ();
          break;
        default:
          break;
        }
    }
  while (key != KEY_ESC);

  mt_setcursorvisible (1);
  mt_gotoXY (0, 27);
  return 0;
}

void
printCell (int address, colors fg, colors bg)
{
  int value;
  int row = address / 10;
  int col = address % 10 * 6;
  char num[10];

  if (sc_memoryGet (address, &value) == 0)
    {
      if (value >= 0)
        {
          sprintf (num, "+%.4x", value);
        }
      else
        {
          sprintf (num, "-%.4x", -(value));
        }
    }
  else
    {
      return;
    }

  mt_setbgcolor (bg);
  mt_setfgcolor (fg);
  mt_gotoXY (2 + col, 2 + row);
  write (STDOUT_FILENO, num, strlen (num));
  mt_setdefaultcolor ();
}

void
printBigCell (int address, int bigchars[18][2])
{
  int value;
  if (sc_memoryGet (address, &value) != 0)
    {
      return;
    }

  if (value >= 0)
    {
      bc_printbigchar (bigchars[16], 63, 9, defclr, defclr);
    }
  else
    {
      bc_printbigchar (bigchars[17], 63, 9, defclr, defclr);
      value = -(value);
    }

  bc_printbigchar (bigchars[((value >> 12) & 0x0F)], 72, 9, defclr, defclr);
  bc_printbigchar (bigchars[((value >> 8) & 0x0F)], 81, 9, defclr, defclr);
  bc_printbigchar (bigchars[((value >> 4) & 0x0F)], 90, 9, defclr, defclr);
  bc_printbigchar (bigchars[(value & 0x0F)], 99, 9, defclr, defclr);

  char buf[100];
  mt_gotoXY (64, 17);
  sprintf (buf, "\033[0;34mНомер редактируемой ячейки: %d  ", address + 1);
  write (STDOUT_FILENO, buf, strlen (buf));
  mt_setdefaultcolor ();
}

void
printFlags (void)
{
  char str[5] = "POMTE";
  mt_gotoXY (92, 2);
  for (int i = 0; i < 5; i++)
    {
      int value = 1;
      if (sc_regGet (i + 1, &value) == 0)
        {
          if (value == 0)
            {
              write (STDOUT_FILENO, "_ ", 2);
            }
          else
            {
              write (STDOUT_FILENO, &str[i], 1);
              write (STDOUT_FILENO, " ", 1);
            }
        }
    }
}

void
printDecodedCommand (int value)
{
  char buf[100];
  int len;
  if (value >= 0)
    {
      len = sprintf (buf, "dec: +%.5d | oct: +%.5o | hex: +%.4X  bin: ", value,
                     value, value);
      for (int i = 13; i >= 0; i--)
        {
          buf[len++] = ((value >> i) & 1) ? '1' : '0';
        }
    }
  else
    {
      len = sprintf (buf,
                     "dec: -%.5d | oct: -%.5o | hex: -%.4X  bin: ", -value,
                     -value, -value);
      for (int i = 13; i >= 0; i--)
        {
          buf[len++] = ((-value >> i) & 1) ? '1' : '0';
        }
    }
  buf[len] = '\0';
  mt_gotoXY (2, 17);
  write (STDOUT_FILENO, buf, len);
}

void
printAccumulator (void)
{
  int value;
  sc_accumulatorGet (&value);
  char buf[30];
  int len = (value >= 0)
                ? sprintf (buf, "sc: %.4x hex: +%.4x", value, value)
                : sprintf (buf, "sc: %.4x hex: -%.4x", -value, -value);
  mt_gotoXY (63, 2);
  write (STDOUT_FILENO, buf, len);
}

void
printCounters (void)
{
  int value;
  sc_icounterGet (&value);
  char buf[20];
  int len = sprintf (buf, "T: 00     IC: %.4x", value);
  mt_gotoXY (64, 5);
  write (STDOUT_FILENO, buf, len);
}

void
printTerm (int address, int input)
{
  static char slider[5][15] = { 0 };
  static int counter = 0;
  int value;

  for (int i = counter; i > 0; i--)
    {
      strcpy (slider[i], slider[i - 1]);
    }

  if (input == 1)
    {
      sprintf (slider[0], "%.2x> ", address);
    }
  else
    {
      sc_memoryGet (address, &value);
      if (value >= 0)
        {
          sprintf (slider[0], "%.2x< +%.4x", address, value);
        }
      else
        {
          sprintf (slider[0], "%.2x< -%.4x", address, value);
        }
    }

  if (counter < 5)
    {
      counter++;
    }

  for (int i = 0; i < counter; i++)
    {
      mt_gotoXY (65, 24 - i);
      write (STDOUT_FILENO, slider[i], strlen (slider[i]));
    }
}

void
printCommand (void)
{
  int value;
  int sign, command, operand;
  char buf[10];
  sc_icounterGet (&value);
  sc_commandDecode (value, &sign, &command, &operand);
  sprintf (buf, "+ %.2x : %.2x", command, operand);
  if (sc_commandValidate (command) != 0)
    {
      buf[0] = '!';
    }
  mt_gotoXY (92, 5);
  write (STDOUT_FILENO, buf, strlen (buf));
}

void
printBox (void)
{
  bc_box (1, 1, 61, 15, defclr, defclr, " Оперативная память ", red, defclr);
  bc_box (62, 1, 84, 3, defclr, defclr, " Аккумулятор ", red, defclr);
  bc_box (86, 1, 107, 3, defclr, defclr, " Регистр флагов ", red, defclr);
  bc_box (62, 4, 84, 6, defclr, defclr, " Счётчик команд ", red, defclr);
  bc_box (86, 4, 107, 6, defclr, defclr, " Команда ", red, defclr);
  bc_box (1, 16, 61, 18, defclr, defclr, " Редактируемая ячейка (формат) ",
          red, white);
  bc_box (62, 7, 107, 18, defclr, defclr, "Редактируемая ячейка (увеличено)",
          red, white);
  bc_box (64, 19, 75, 25, defclr, defclr, "IN---OUT", green, white);
  bc_box (1, 19, 63, 25, defclr, defclr, " Кэш память ", green, white);
  bc_box (76, 19, 107, 25, defclr, defclr, " Клавиатура ", green, white);

  char buf[100];
  int len = sprintf (buf, "l - load s - save i - reset");
  mt_gotoXY (78, 20);
  write (STDOUT_FILENO, buf, len);
  len = sprintf (buf, "r - run  t - step");
  mt_gotoXY (78, 21);
  write (STDOUT_FILENO, buf, len);
  len = sprintf (buf, "F5 - accumulator");
  mt_gotoXY (78, 22);
  write (STDOUT_FILENO, buf, len);
  len = sprintf (buf, "F6 - icounter");
  mt_gotoXY (78, 23);
  write (STDOUT_FILENO, buf, len);
  len = sprintf (buf, "ESC - выход");
  mt_gotoXY (78, 24);
  write (STDOUT_FILENO, buf, len);
}

void
memoryInput (int address)
{
  int value;
  int row = address / 10;
  int col = address % 10 * 6;
  mt_gotoXY (2 + col, 2 + row);
  write (STDOUT_FILENO, "     ", 5);
  mt_gotoXY (2 + col, 2 + row);
  rk_readvalue (&value, 0);
  sc_memorySet (address, value);
}

void
memorySave (void)
{
  char msg[100] = "Введите имя файла для сохранения: ";
  int len = strlen (msg);
  char filename[100];

  mt_gotoXY (0, 26);
  mt_delline ();
  write (STDOUT_FILENO, msg, len);
  mt_setcursorvisible (1);
  rk_mytermregime (1, 0, 1, 1, 1);

  len = read (STDIN_FILENO, filename, 100 - 1);
  if (len > 0)
    {
      if (filename[len - 1] == '\n')
        {
          filename[len - 1] = '\0';
        }
      else
        {
          filename[len] = '\0';
        }
    }
  else
    {
      filename[0] = '\0';
    }

  mt_setcursorvisible (0);
  rk_mytermregime (0, 0, 1, 0, 1);

  if (sc_memorySave (filename) == 0)
    {
      len = sprintf (msg, "\033[32m[Успех]\033[39m Файл был сохранён!");
      mt_gotoXY (0, 26);
      mt_delline ();
      write (STDOUT_FILENO, msg, len);
    }
  else
    {
      len = sprintf (msg,
                     "\033[31m[Ошибка]\033[39m Не удалось сохранить файл");
      mt_gotoXY (0, 26);
      mt_delline ();
      write (STDOUT_FILENO, msg, len);
    }
}

void
memoryLoad (void)
{
  char msg[100] = "Введите имя файла для загрузки: ";
  int len = strlen (msg);
  char filename[100];

  mt_gotoXY (0, 26);
  mt_delline ();
  write (STDOUT_FILENO, msg, len);
  mt_setcursorvisible (1);
  rk_mytermregime (1, 0, 1, 1, 1);

  len = read (STDIN_FILENO, filename, 100 - 1);
  if (len > 0)
    {
      if (filename[len - 1] == '\n')
        {
          filename[len - 1] = '\0';
        }
      else
        {
          filename[len] = '\0';
        }
    }
  else
    {
      filename[0] = '\0';
    }

  mt_setcursorvisible (0);
  rk_mytermregime (0, 0, 1, 0, 1);

  if (sc_memoryLoad (filename) == 0)
    {
      for (int i = 0; i < MEMORY_SIZE; i++)
        {
          printCell (i, defclr, defclr);
        }
      len = sprintf (msg, "\033[32m[Успех]\033[39m Файл был загружен!");
      mt_gotoXY (0, 26);
      mt_delline ();
      write (STDOUT_FILENO, msg, len);
    }
  else
    {
      len = sprintf (msg,
                     "\033[31m[Ошибка]\033[39m Не удалось загрузить файл");
      mt_gotoXY (0, 26);
      mt_delline ();
      write (STDOUT_FILENO, msg, len);
    }
}

void
reset (void)
{
  sc_memoryInit ();
  sc_regInit ();
  sc_accumulatorInit ();
  sc_icounterInit ();
  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      printCell (i, defclr, defclr);
    }
  mt_gotoXY (0, 26);
  mt_delline ();
}

void
accumulatorInput (void)
{
  int value;
  mt_gotoXY (77, 2);
  write (STDOUT_FILENO, "     ", 5);
  mt_gotoXY (77, 2);
  rk_readvalue (&value, 0);
  sc_accumulatorSet (value);
}

void
icounterInput (void)
{
  int value;
  mt_gotoXY (78, 5);
  write (STDOUT_FILENO, "     ", 5);
  mt_gotoXY (78, 5);
  rk_readvalue (&value, 0);
  sc_icounterSet (value);
}