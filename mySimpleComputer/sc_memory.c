#include <stdio.h>

#include "mySimpleComputer.h"

static int sc_memory[MEMORY_SIZE];

int
sc_memoryInit (void)
{
  for (int i = 0; i < MEMORY_SIZE; i++)
    {
      sc_memory[i] = 0;
    }
  return 0;
}

int
sc_memorySet (int address, int value)
{
  if (address >= 0 && address < MEMORY_SIZE && value >= MIN_VALUE
      && value <= MAX_VALUE)
    {
      sc_memory[address] = value;
      return 0;
    }
  return -1;
}

int
sc_memoryGet (int address, int *value)
{
  if (address >= 0 && address < MEMORY_SIZE)
    {
      *value = sc_memory[address];
      return 0;
    }
  return -1;
}

int
sc_memorySave (char *filename)
{
  if (filename != NULL)
    {
      FILE *fp = fopen (filename, "wb");
      if (fp != NULL)
        {
          fwrite (sc_memory, sizeof (int), MEMORY_SIZE, fp);
          fclose (fp);
          return 0;
        }
      return -1;
    }
  return -1;
}

int
sc_memoryLoad (char *filename)
{
  if (filename != NULL)
    {
      FILE *fp = fopen (filename, "rb");
      if (fp != NULL)
        {
          fread (sc_memory, sizeof (int), MEMORY_SIZE, fp);
          fclose (fp);
          return 0;
        }
      return -1;
    }
  return -1;
}
