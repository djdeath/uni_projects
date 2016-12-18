%{
	
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "line.h"
#include "identifier.h"

#include "context.h"
#include "symbol.h"
#include "expression.h"
#include "temporary.h"
#include "label.h"	
#include "output.h"
#include "seq.h"
#include "string.h"

#include "cex.tab.h"

	int yyerror (const char *, ...);
	int yylex ();
	int label = 0;
%}

%union
{
	Expression_t* expr;
	
	Function_t* func;
	List_t* args;
	
	Exception_t* except;
	
	unsigned int label;
	
	Symbol_t* sym;
	
	char* string;
}

%token IDENTIFIER
%token INTEGER STRING

%token KEY_VOID KEY_INT KEY_ARRAY

%token KEY_THROWABLE KEY_EXTENDS
%token KEY_TRY KEY_CATCH KEY_FINALLY KEY_THROW

%token KEY_WHILE
%token KEY_IF KEY_ELSE KEY_RETURN

%token KEY_LTEQ KEY_GTEQ KEY_NOTEQ KEY_EQ KEY_AND KEY_OR

%token KEY_DEC_LEFT KEY_DEC_RIGHT

%token KEY_ALLOCATE KEY_FREE KEY_NULL

%token KEY_ARRAY_LENGTH KEY_ARRAY_DIMENSION

%token KEY_PRINT KEY_INPUT

%left '='

%left KEY_OR
%left KEY_AND

%left '|'
%left '^'
%left '&'

%left KEY_EQ KEY_NOTEQ

%left '<' KEY_LTEQ '>' KEY_GTEQ

%left KEY_DEC_LEFT KEY_DEC_RIGHT

%left '+' '-'
%left '*' '/' '%'
%left MOINS_UNAIRE

%left '~'
%left '!'

%left KEY_THEN
%left KEY_ELSE

%left KEY_END
%left KEY_FINALLY

%left KEY_FCATCH
%left KEY_CATCH

%type <expr> expr
%type <expr> cond
%type <expr> INTEGER
%type <expr> KEY_NULL

%type <expr> call_variable
%type <expr> call_function

%type <expr> allocate
%type <expr> KEY_ALLOCATE

%type <expr> input
%type <expr> KEY_INPUT

%type <expr> define_variable_array_
%type <expr> define_variable_int_

%type <expr> except_var

%type <label> test_seq_if
%type <label> label

%type <label> loop_seq_start
%type <label> loop_seq_end

%type <func> define_function_type
%type <func> call_function_

%type <args> define_parameters
%type <args> define_parameters_
%type <args> parameters
%type <args> parameters_

%type <except> define_exception_throwable

%type <sym> define_parameters__

%type <label> instruction_seq

%type <string> STRING

%%

PROGRAMME : /* rien ;) */
| PROGRAMME define
;

define : define_exception
| define_function
;

instruction_seq : { context_up (CONTEXT_BLOCK); } instruction { $$ = context_returned (); context_down (); }
| '{' { context_up (CONTEXT_BLOCK); } instruction_seq_ '}' { $$ = context_returned (); context_down (); }
;

instruction_seq_ : instruction
| instruction_seq_ instruction
;

instruction : define_variable ';'
| call_variable '=' cond ';' { do_affect_var ($1, $3); }
| call_variable '[' parameters_ ']' '=' cond ';' { do_affect_array ($1, $3, $6); }
| call_variable '[' '[' call_variable ']' ']' '=' cond ';' { do_affect_array_a ($1, $4, $8); }
| call_function ';' { expression_free ($1); }
| free ';'
| print ';'
| loop_seq
| test_seq
| try_seq
| throw ';'
| ret ';'
;

ret : KEY_RETURN { do_return (NULL); }
| KEY_RETURN cond { do_return ($2); }
;

/* COMMON */

label : { $$ = label_new (); }
;

/* IF/ELSE */

test_seq : test_seq_if { do_ifend ($1); context_down (); } %prec KEY_THEN
| test_seq_if KEY_ELSE label
{
	do_ifelse ($3);
	do_ifend ($1);
} instruction_seq
{
	do_ifend ($3);
	if (context_returned ())
	{
		context_down ();
		context_set_returned (2);
	}
	else
		context_down ();
}
;

test_seq_if : KEY_IF { context_up (CONTEXT_TEST); } '(' cond ')' label { do_if ($4, $6); } instruction_seq { $$ = $6; }
;

/* WHILE */

loop_seq : loop_seq_start loop_seq_end instruction_seq { do_whileend ($1, $2); }
| loop_seq_start loop_seq_end ';' { do_whileend ($1, $2); }
;

loop_seq_start : KEY_WHILE { $$ = label_new (); do_while_start ($$); }
;

loop_seq_end : '(' cond ')' { $$ = label_new (); do_while ($2, $$); }
;

/* TRY/CATCH/FINALLY */

try_seq : KEY_TRY except_var { context_up_except ($2); } label label { do_try ($4); } instruction_try_seq
{ do_tryend ($4, $5, $2); } catch_seq { do_finally ($5); } finally_seq { do_finallyend (); expression_free ($2); context_down (); }
;

instruction_try_seq : { context_up (CONTEXT_TRY); } instruction { context_down (); }
| '{' { context_up (CONTEXT_TRY); } instruction_seq_ '}' { context_down (); }
;

catch_seq : catch %prec KEY_FCATCH
| catch catch_seq
;

catch : KEY_CATCH label IDENTIFIER { do_catch (get_identifier (), $2); } instruction_catch_seq { do_catchend ($2); }
;

instruction_catch_seq : { context_up (CONTEXT_CATCH); } instruction { context_down (); }
| '{' { context_up (CONTEXT_CATCH); } instruction_seq_ '}' { context_down (); }
;

finally_seq : /* Epsilon */ %prec KEY_END
| KEY_FINALLY { do_finallystart (); } instruction_seq_finally
;

instruction_seq_finally : { context_up (CONTEXT_FINALLY); } instruction { context_down (); }
| '{' { context_up (CONTEXT_FINALLY); } instruction_seq_ '}' { context_down (); }
;

except_var : { $$ = expression_new_temporary (TYPE_INT); }
;

/* */

throw : KEY_THROW IDENTIFIER { do_throw (get_identifier ()); }
;

/* */

expr : '(' expr ')' { $$ = $2; }
| '-' expr %prec MOINS_UNAIRE { $$ = do_unary ('-', $2); }
| '~' expr { $$ = do_unary ('~', $2); }
| expr '+' expr { $$ = do_binary ('+', $1, $3); }
| expr '-' expr { $$ = do_binary ('-', $1, $3); }
| expr '/' expr { $$ = do_binary ('/', $1, $3); }
| expr '*' expr { $$ = do_binary ('*', $1, $3); }
| expr '%' expr { $$ = do_binary ('%', $1, $3); }
| expr '&' expr { $$ = do_binary ('&', $1, $3); }
| expr '^' expr { $$ = do_binary ('^', $1, $3); }
| expr '|' expr { $$ = do_binary ('|', $1, $3); }
| expr KEY_DEC_LEFT expr { $$ = do_dec (KEY_DEC_LEFT, $1, $3); }
| expr KEY_DEC_RIGHT expr { $$ = do_dec (KEY_DEC_RIGHT, $1, $3); }
| call_variable { $$ = $1; }
| call_variable '[' parameters_ ']' { $$ = do_array_get ($1, $3); }
| call_variable '[' '[' call_variable ']' ']' { $$ = do_array_get_a ($1, $4); }
| call_variable KEY_ARRAY_LENGTH '(' expr ')' { $$ = do_array_get_lenght ($1, $4); }
| call_variable KEY_ARRAY_DIMENSION { $$ = do_array_get_dimension ($1); }
| call_function { $$ = $1; }
| allocate { $$ = $1; }
| input { $$ = $1; }
| INTEGER { $$ = $1; }
| KEY_NULL { $$ = $1; }
;

cond : expr { $$ = $1 }
| expr '>' expr { $$ = do_cmp ('>', 0, $1, $3); }
| expr '<' expr { $$ = do_cmp ('<', 0, $1, $3); }
| expr KEY_LTEQ expr { $$ = do_cmp ('\0', KEY_LTEQ, $1, $3); }
| expr KEY_GTEQ expr { $$ = do_cmp ('\0', KEY_GTEQ, $1, $3); }
| expr KEY_NOTEQ expr { $$ = do_cmp ('\0', KEY_NOTEQ, $1, $3); }
| expr KEY_EQ expr { $$ = do_cmp ('\0', KEY_EQ, $1, $3); }
| cond KEY_AND label { do_and ($1, $3); } cond { $$ = do_andend ($5, $1, $3); }
| cond KEY_OR label { do_or ($1, $3); } cond { $$ = do_orend ($5, $1, $3); }
| '!' cond { $$ = do_cmp ('\0', KEY_EQ, $2, expression_new_constant (TYPE_INT, 0)); }
;

call_variable : IDENTIFIER { $$ = expression_new_symbol (SYMBOL (context_get_var (get_identifier ()))); }
;

call_function : call_function_ parameters { $$ = do_call ($1, $2); }
;

call_function_ : IDENTIFIER { $$ = context_get_func (get_identifier ()); }
;

parameters : '(' ')' { $$ = NULL; }
| '(' parameters_ ')' { $$ = $2; }
;

parameters_ : cond { $$ = list_new ($1); }
| parameters_ ',' cond { list_append (&$1, $3); $$ = $1; }
;

input : KEY_INPUT { $$ = do_input (); }
;

print : KEY_PRINT cond { do_print_expr ($2); }
| KEY_PRINT STRING { do_print_string (string_new (), $2); }
;

allocate : KEY_ALLOCATE '(' parameters_ ')' { $$ = do_allocate ($3); }
| KEY_ALLOCATE '[' expr ']' { $$ = do_allocate_a ($3); }
;

free : KEY_FREE call_variable { do_free ($2); }
;

/* DEFINITIONS VARIABLES */

define_variable : KEY_INT define_variable_int
| KEY_ARRAY define_variable_array
;

define_variable_int : define_variable_int_
{
	do_affect_var ($1, expression_new_constant (TYPE_INT, 0));
}
| define_variable_int_ '=' cond
{
	do_affect_var ($1, $3);
}
| define_variable_int ',' define_variable_int_
{
	do_affect_var ($3, expression_new_constant (TYPE_INT, 0));
}
| define_variable_int ',' define_variable_int_ '=' cond
{
	do_affect_var ($3, $5);
}
;

define_variable_int_ : IDENTIFIER { $$ = expression_new_symbol (SYMBOL (context_add_var (TYPE_INT, get_identifier ()))); } %prec KEY_THEN
;

define_variable_array : define_variable_array_
{
	do_affect_var ($1, expression_new_constant (TYPE_ARRAY, 0));
}
| define_variable_array_ '=' cond
{
	do_affect_var ($1, $3);
}
| define_variable_array ',' define_variable_array_
{
	do_affect_var ($3, expression_new_constant (TYPE_ARRAY, 0));
}
| define_variable_array ',' define_variable_array_ '=' cond
{
	do_affect_var ($3, $5);
}
;

define_variable_array_ : IDENTIFIER { $$ = expression_new_symbol (SYMBOL (context_add_var (TYPE_ARRAY, get_identifier ()))); } %prec KEY_THEN
;

/* DEFINITION FONCTIONS */

define_function : define_function_type define_parameters
{ function_check_args_def ($1, $2); do_func_start ($1); } '{'
{ context_up_func ($1); } instruction_seq_ '}'
{ do_returned ($1); context_down (); context_down (); do_func_end ($1); }
| define_function_type define_parameters { function_check_args_def ($1, $2); context_down (); } ';'
;

define_function_type : KEY_VOID IDENTIFIER { $$ = FUNCTION (context_add_func_defined (TYPE_VOID, get_identifier ())); }
| KEY_INT IDENTIFIER { $$ = FUNCTION (context_add_func_defined (TYPE_INT, get_identifier ())); }
| KEY_ARRAY IDENTIFIER { $$ = FUNCTION (context_add_func_defined (TYPE_ARRAY, get_identifier ())); }
;

define_parameters : '(' ')' { context_up (CONTEXT_FUNC_ARG); $$ = NULL; }
| '(' KEY_VOID ')' { context_up (CONTEXT_FUNC_ARG); $$ = NULL; }
| '(' { context_up (CONTEXT_FUNC_ARG); } define_parameters_ ')' { $$ = $3; }
;

define_parameters_ : define_parameters__ { $$ = list_new ($1); }
| define_parameters__ ',' define_parameters_ { list_insert_first (&$3, $1); $$ = $3; }
;

define_parameters__: KEY_INT IDENTIFIER { $$ = SYMBOL (context_add_var_arg (TYPE_INT, get_identifier ())); }
| KEY_ARRAY IDENTIFIER { $$ = SYMBOL (context_add_var_arg (TYPE_ARRAY, get_identifier ())); }
;

/* DEFINITION EXCEPTIONS */

define_exception : define_exception_throwable KEY_EXTENDS IDENTIFIER { exception_set_parent ($1, get_identifier ()); } ';'
;

define_exception_throwable : KEY_THROWABLE IDENTIFIER { $$ = context_add_except (get_identifier ()); }
;

%%

int
yyerror (const char *fmt, ...)
{
	va_list args;
	
	va_start (args, fmt); 
	fprintf (stderr, "line %i : ", get_line ());
	vfprintf (stderr, fmt, args);
	fprintf (stderr, "\n");
	va_end (args); 
	
	return 0;
}

int
main (int argc, char* argv[])
{
	int i, ret;
	FILE* file;
	
	ret = 0;
	for (i = 1 ; i < argc ; i++)
	{
		if ((file = fopen (argv[i], "r")))
		{
			stdin = file;
			
			/* Initialisation des différents modules. */
			line_init ();
			string_init ();
			context_init ();
			exception_init ();
			temporary_var_init ();
			output_init (stdout);
			
			/* parsing */
			ret += yyparse ();
			
			/* Libération */
			exception_end ();
			context_end ();
		}
		else
		{
			ret++;
			perror (argv[i]);
		}
	}
	
	return ret;
}
