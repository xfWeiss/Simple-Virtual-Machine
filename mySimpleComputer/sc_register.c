#include "mySimpleComputer.h"

static int sc_reg;
static int sc_accumulator;
static int sc_instructionCounter;

int
sc_regInit (void)
{
  sc_reg = 0;
  return 0;
}

int
sc_regSet (int reg, int value)
{
  if (reg > 0 && reg <= 5)
    {
      if (value)
        {
          if (value == 1)
            {
              BIT_SET (sc_reg, reg);
              return 0;
            }
          else
            {
              BIT_SET (sc_reg, FLAG_OVERFLOW);
              return -1;
            }
        }
      else
        {
          BIT_DEL (sc_reg, reg);
          return 0;
        }
    }
  return -1;
}

int
sc_regGet (int reg, int *value)
{
  if (reg > 0 && reg <= 5)
    {
      *value = BIT_GET (sc_reg, reg);
      return 0;
    }
  return -1;
}

int
sc_accumulatorInit (void)
{
  sc_accumulator = 0;
  return 0;
}

int
sc_accumulatorSet (int value)
{
  if (value >= MIN_VALUE && value <= MAX_VALUE)
    {
      sc_accumulator = value;
      return 0;
    }
  return -1;
}

int
sc_accumulatorGet (int *value)
{
  *value = sc_accumulator;
  return 0;
}

int
sc_icounterInit (void)
{
  sc_instructionCounter = 0;
  return 0;
}

int
sc_icounterSet (int value)
{
  if (value >= 0 && value <= MEMORY_SIZE)
    {
      sc_instructionCounter = value;
      return 0;
    }
  return -1;
}
int
sc_icounterGet (int *value)
{
  *value = sc_instructionCounter;
  return 0;
}
