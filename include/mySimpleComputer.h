#ifndef MY_SIMPLE_COMPUTER_H
#define MY_SIMPLE_COMPUTER_H

#define MEMORY_SIZE 128
#define MIN_VALUE -16383
#define MAX_VALUE 16383 // (2^14) - 1

#define FLAG_OVERFLOW 1
#define FLAG_DIV_BY_ZERO 2
#define FLAG_WRONG_ADDRESS 3
#define FLAG_IGNORE_PULSE 4
#define FLAG_WRONG_COMMAND 5

#define BIT_SET(X, Y) X = X | (1 << (Y - 1)) // Устанавалиет единицу
#define BIT_DEL(X, Y) X = X & (~(1 << (Y - 1))) // Устанавливает ноль
#define BIT_GET(X, Y) X >> (Y - 1) & 0x1 // Возвращает нужный бит

// Функции для работы с оперативной памятью
int sc_memoryInit (void);
int sc_memorySet (int address, int value);
int sc_memoryGet (int address, int *value);
int sc_memorySave (char *filename);
int sc_memoryLoad (char *filename);

// Функции для работы с регистрами
int sc_regInit (void);
int sc_regSet (int register, int value);
int sc_regGet (int register, int *value);
int sc_accumulatorInit (void);
int sc_accumulatorSet (int value);
int sc_accumulatorGet (int *value);
int sc_icounterInit (void);
int sc_icounterSet (int value);
int sc_icounterGet (int *value);

// Функции для кодирования и декодирования команд
int sc_commandEncode (int sign, int command, int operand, int *value);
int sc_commandDecode (int value, int *sign, int *command, int *operand);
int sc_commandValidate (int command);

#endif