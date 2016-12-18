CC=gcc

FLEX=flex
BISON=bison -d -v
CFLAGS=-Wall -g
LIBS=-lfl -lm


BISS=3add.y
LEXS=3add.lex

OBJ=lex.yy.o 3add.tab.o context.o hash_table.o hash_close.o\
hash_func.o hash_open.o identifier.o line.o list.o output.o\
function.o variable.o parse.o seq.o

BIN=ccex3a

all: flex bison $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN) $(LIBS)

%.o:
	$(CC) $(CFLAGS) -c $*.c

flex:
	$(FLEX) $(LEXS)

bison:
	$(BISON) $(BISS)

clean:
	@-rm -f *.o ; rm -f \#* ; rm -f $(BIN)

edit:
	@emacs -r .&
