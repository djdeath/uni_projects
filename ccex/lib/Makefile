CC=gcc

CFLAGS=-Wall -g
LIBS=

OBJ=array.o exception.o main.o print.o

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c

clean:
	@-rm -f *.o ; rm -f \#*

edit :
	@emacs -r .&
