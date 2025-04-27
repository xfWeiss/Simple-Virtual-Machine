#ifndef MY_BIG_CHARS_H
#define MY_BIG_CHARS_H

#include "myTerm.h"

#define BOXCH_REC "a"
#define BOXCH_BR "j"
#define BOXCH_BL "m"
#define BOXCH_TR "k"
#define BOXCH_TL "l"
#define BOXCH_VERT "x"
#define BOXCH_HOR "q"

#define BIGCH_SIZE 2

int bc_strlen (char *str);
int bc_printA (char *str);
int bc_box (int x1, int y1, int x2, int y2, colors box_fg, colors box_bg,
            char *header, colors header_fg, colors header_bg);
int bc_setbigcharpos (int *big, int x, int y, int value);
int bc_getbigcharpos (int *big, int x, int y, int *value);
int bc_printbigchar (int big[2], int x, int y, colors fg, colors bg);
int bc_bigcharwrite (int fd, int *big, int count);
int bc_bigcharread (int fd, int *big, int need_count, int *count);

#endif