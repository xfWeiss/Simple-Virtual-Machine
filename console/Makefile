TARGET = ../simplecomputer

CC = gcc
CFLAGS = -Wall -Wno-unused-result -O2 -std=c11 -I../include

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

LIBS = ../mySimpleComputer/libmysimplecomputer.a ../myTerm/libmyterm.a ../myBigChars/libmybigch.a ../myReadKey/libmyreadkey.a

all: $(TARGET)

$(TARGET): $(OBJ) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
