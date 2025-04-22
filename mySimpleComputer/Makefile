LIBRARY = libmysimplecomputer.a

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CFLAGS = -std=c11 -fPIC

all: $(LIBRARY)

$(LIBRARY): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ) $(LIBRARY)

.PHONY: all clean
