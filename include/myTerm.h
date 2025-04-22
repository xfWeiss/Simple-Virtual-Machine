#ifndef MY_TERM_H
#define MY_TERM_H

typedef enum
{
  black,
  red,
  green,
  blue,
  brown,
  magenta,
  cyan,
  white,
  defclr
} colors;

int mt_clrscr (void); // Очищает экран терминала
int mt_getscreensize (int *rows,
                      int *cols); // Определяет размер экрана терминала
int mt_gotoXY (int x, int y); // Перемещение курсора в координаты x, y
int mt_setfgcolor (
    colors color); // Устанавливает цвет последующих выводимых символов
int mt_setbgcolor (
    colors color); // Устанавливает цвет фона последующих выводимых символов
int
mt_setdefaultcolor (void); // Устанавливает цвета символов и фона по умолчанию
int mt_setcursorvisible (int value); // Скрывает или показывает курсор
int mt_delline (void); // очищает текущую строку

#endif