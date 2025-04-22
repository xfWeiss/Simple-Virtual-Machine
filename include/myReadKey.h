#ifndef MY_READ_KEY_H
#define MY_READ_KEY_H

typedef enum
{
  KEY_F5,
  KEY_F6,
  KEY_ESC,
  KEY_ENTER,
  KEY_UP,
  KEY_DOWN,
  KEY_RIGHT,
  KEY_LEFT,
  KEY_L,
  KEY_S,
  KEY_I,
  KEY_R,
  KEY_T,
  KEY_PLUS,
  KEY_MINUS,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_UNKNOWN
} keys;

/*
  Осуществляет управление настройками режимов терминала
  1.1 regime = 1 - канонический режим (ввод с буферизацией)
  1.2 regime = 0 - неканонический режим (без буферизации)
  2.  vtime - время ожидания для ввода в неканоническом режиме (в милисекундах)
  3.  vmin - минимальное количество символов для чтения в неканоническом режиме
  4.1 echo = 1 - отображать вводимые символы в терминале
  4.2 echo = 0 - не отображать
  5.1 sigint = 1 - обрабатывать сигнал прерывания
  5.2 sigint = 0 - не обрабатывать сигнал прерывания
*/
int rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint);

int rk_readkey (keys *key); // Регистрирует нажатие валидных клавиш
int rk_mytermsave (void);
int rk_mytermrestore (void);
int rk_readvalue (int *value,
                  int timeout); // Считывает вводимое шестнадцатеричное число

#endif