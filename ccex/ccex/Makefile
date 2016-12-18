CC=gcc

FLEX=flex
BISON=bison -d -v
CFLAGS=-Wall -g
LIBS=-lfl -lm

BISS=cex.y
LEXS=cex.lex

OBJ=lex.yy.o cex.tab.c\
context.o expression.o exception.o\
identifier.o hash_table.o hash_close.o\
hash_func.o hash_open.o line.o label.o\
list.o output.o symbol.o temporary.o string.o\
seq_array_allocate.o seq_affect.o seq_array_get.o\
seq_input_print.o seq_try.o seq_test.o seq_call.o\
seq_return.o seq_ops.o

BIN=ccex

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

edit :
	@emacs -r .&
