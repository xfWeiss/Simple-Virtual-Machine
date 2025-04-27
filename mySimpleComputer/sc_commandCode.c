#include "mySimpleComputer.h"

typedef enum
{
  NOP = 0,     // Пустая операция
  CPUINFO = 1, // Информация о CPU
  READ = 10,   // Ввод с терминала
  WRITE = 11,  // Вывод на терминал
  LOAD = 20,   // Загрузка в аккумулятор
  STORE = 21,  // Выгрузка из аккумулятора
  ADD = 30,    // Сложение
  SUB = 31,    // Вычитание
  DIVIDE = 32, // Деление
  MUL = 33,    // Умножение
  JUMP = 40,   // Переход
  JNEG = 41,   // Переход, если отрицательное
  JZ = 42,     // Переход, если ноль
  HALT = 43,   // Остановка
  NOT = 51,    // Двоичная инверсия
  AND = 52,    // Логическое И
  OR = 53,     // Логическое ИЛИ
  XOR = 54,    // Исключающее ИЛИ
  JNS = 55,    // Переход, если положительное
  JC = 56,     // Переход, если переполнение
  JNC = 57,    // Переход, если нет переполнения
  JP = 58,     // Переход, если четное
  JNP = 59,    // Переход, если нечетное
  CHL = 60,    // Сдвиг влево
  SHR = 61,    // Сдвиг вправо
  RCL = 62,    // Циклический сдвиг влево
  RCR = 63,    // Циклический сдвиг вправо
  NEG = 64,    // Дополнительный код
  ADDC = 65, // Сложение с адресом в аккумуляторе
  SUBC = 66, // Вычитание с адресом в аккумуляторе
  LOGLC = 67, // Логический сдвиг влево
  LOGRC = 68, // Логический сдвиг вправо
  RCCL = 69,  // Циклический сдвиг влево
  RCCR = 70,  // Циклический сдвиг вправо
  MOVA = 71,  // Перемещение в ячейку
  MOVR = 72,  // Перемещение из ячейки
  MOVCA = 73, // Перемещение с учетом адреса в аккумуляторе
  MOVCR = 74, // Перемещение из ячейки по адресу в аккумуляторе
  ADDC2 = 75, // Сложение с учетом адреса
  SUBC2 = 76  // Вычитание с учетом адреса
} SimpleComputerCommandCodes;

int
sc_commandValidate (int command)
{
  // Массив допустимых кодов команд в порядке возрастания
  int validCommands[] = { NOP,  CPUINFO, READ,  WRITE, LOAD, STORE, ADD,
                          SUB,  DIVIDE,  MUL,   JUMP,  JNEG, JZ,    HALT,
                          NOT,  AND,     OR,    XOR,   JNS,  JC,    JNC,
                          JP,   JNP,     CHL,   SHR,   RCL,  RCR,   NEG,
                          ADDC, SUBC,    LOGLC, LOGRC, RCCL, RCCR,  MOVA,
                          MOVR, MOVCA,   MOVCR, ADDC2, SUBC2 };

  for (int i = 0; i < sizeof (validCommands) / sizeof (validCommands[0]); i++)
    {
      if (command == validCommands[i])
        {
          return 0; // Код команды валиден
        }
    }
  return -1; // Несуществующий код команды
}

int
sc_commandEncode (int sign, int command, int operand, int *value)
{
  if (sign >= 0 && sign <= 1 && sc_commandValidate (command) == 0
      && operand >= 0 && operand < MEMORY_SIZE)
    {
      int code = operand;
      code += (command << 7);
      code = code & (~(1 << 14));
      *value = code;
      return 0;
    }
  return -1;
}

int
sc_commandDecode (int value, int *sign, int *command, int *operand)
{
  int temp1 = *sign, temp2 = *command;
  *sign = (value >> 14) & 1;
  *command = (value >> 7) & 127;
  *operand = value & 127;
  if (*sign < 0 || *sign > 1 || sc_commandValidate (*command) == -1
      || *operand < 0 || *operand > 127)
    {
      *sign = temp1;
      *command = temp2;
      return -1;
    }
  return 0;
}
