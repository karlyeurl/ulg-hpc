
CC = clang
EXEC = a.out
FILES = main_Matlab.o vectorlib.o
LDFLAGS = -lm -std=c99
CFLAGS = -lm -std=c99


all: $(EXEC)

a.out: $(FILES) 
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm $(FILES)

mrproper: clean
	rm $(EXEC)

