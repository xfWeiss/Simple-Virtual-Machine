LIBRARY = libmyterm.a

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CFLAGS = -Wall -Wno-unused-result -O2 -std=c11 -I../include -fPIC

all: $(LIBRARY)

$(LIBRARY): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIBRARY)

.PHONY: all clean