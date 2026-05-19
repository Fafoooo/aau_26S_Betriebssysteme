CC=gcc
CFLAGS=-I.

OBJ = main.o add.o

myprogram: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ) myprogram
