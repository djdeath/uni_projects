%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "line.h"
#include "identifier.h"
#include "parse.h"
#include "output.h"
#include "seq.h"
#include "context.h"

	int yyerror (const char *fmt, ...);
	int yylex ();

%}

%union
{
	Variable_t* var;
	Function_t* func;
	List_t* args;
	char* string;
}

%token CMP
%token JC
%token SV
%token JMP
%token FUNC_START
%token FUNC_END
%token CALL
%token RET
%token TRY

%token IDENTIFIER
%token CONSTANT
%token LABEL
%token STRING
%token SSTRING

%type <var> var
%type <var> CONSTANT

%type <func> function

%type <args> args
%type <args> args_

%type <string> SSTRING

%%

PROGRAMME: /* */
| command ';' PROGRAMME
;

command: FUNC_START function { context_set ($2); if (parse_writing ()) do_funcstart ($2); } defargs
| FUNC_END function { if (parse_writing ()) do_funcend ($2); }
| STRING LABEL { if (!parse_writing ()) output ("%s:\n", get_identifier ()); } SSTRING { if (!parse_writing ()) output ("\t.string %s\n", $4); }
| var '=' expr { if (parse_writing ()) { /*printf ("----\n");*/ do_affect ($1); /*printf ("----\n");*/ } }
| TRY LABEL { if (parse_writing ()) do_try (get_identifier ()); }
| CALL function args
{
	if (parse_writing ())
		do_call ($2, $3);
	else
		function_set_stack_call (context_get_current (), list_length ($3));
	list_foreach ($3, FOREACH (variable_free_constant), NULL);
	list_free_all ($3);
}
| CALL function LABEL { if (parse_writing ()) do_print_string ($2, get_identifier ()); }
| CMP var var
{
	if (parse_writing ())
		do_cmp ($2, $3);
	variable_free_constant ($2);
	variable_free_constant ($3);
}
| JC { if (parse_writing ()) output ("\t%s\t", get_identifier ()); } LABEL { if (parse_writing ()) output ("%s\n", get_identifier ()); }
| JMP LABEL { if (parse_writing ()) output ("\tjmp\t%s\n", get_identifier ()); }
| LABEL { if (parse_writing ()) output ("%s:\n", get_identifier ()); }
| RET var { if (parse_writing ()) do_ret ($2); variable_free_constant ($2); }
| RET { if (parse_writing ()) do_ret (NULL); }
;

expr:  var '+' var
{
	if (parse_writing ())
		do_add ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '*' var
{
	if (parse_writing ())
		do_mult ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '-' var
{
	if (parse_writing ())
		do_minus ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '/' var
{
	if (parse_writing ())
		do_div ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '%' var
{
	if (parse_writing ())
		do_mod ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '<' var
{
	if (parse_writing ())
		do_dec_left ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '>' var
{
	if (parse_writing ())
		do_dec_right ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var { if (parse_writing ()) do_nop ($1); }
| '~' var
{
	if (parse_writing ())
		do_unary_tilde ($2);
	variable_free_constant ($2);
}
| '-' var
{
	if (parse_writing ())
		do_unary_minus ($2);
	variable_free_constant ($2);
}
| var '|' var
{
	if (parse_writing ())
		do_or ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '&' var
{
	if (parse_writing ())
		do_and ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| var '^' var
{
	if (parse_writing ())
		do_xor ($1, $3);
	variable_free_constant ($1);
	variable_free_constant ($3);
}
| CALL function args
{ 
	if (parse_writing ())
		do_call ($2, $3);
	else
		function_set_stack_call (context_get_current (), list_length ($3));
	list_foreach ($3, FOREACH (variable_free_constant), NULL);
	list_free_all ($3);
}
| SV { if (parse_writing ()) do_sv (get_identifier ()); }
;

var: IDENTIFIER { $$ = context_get_var (get_identifier ()); }
| CONSTANT { $$ = $1; }
;

function: IDENTIFIER { $$ = context_get_func (get_identifier ()); }
;

args: /* */ { $$ = NULL; }
| args_ { $$ = $1; }
;

args_: var { $$ = list_new ($1); }
| args_ var { list_insert_first (&$1, $2); $$ = $1; }
;

defargs: /* */
| defargs_
;

defargs_: defvararg
| defvararg defargs_
;

defvararg: IDENTIFIER { context_add_arg (get_identifier ()); }
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
	int i;
	FILE *file;
	
	output_init (stdout);
	
	for (i = 1 ; i < argc ; i++)
	{
		if ((file = fopen (argv[i], "r")))
		{
			stdin = file;
			
			line_init ();
			context_init ();
			
			parse_set_writing (0);
			output (".section\t.rodata\n");
			
			if (!yyparse ())
			{
				output (".text\n");
				fseek (file, 0, SEEK_SET);
				
				parse_set_writing (1);
				yyparse ();
			}
			context_end ();
			
			fclose (file);
		}
		else
			perror (argv[i]);
	}
	
	return 0;
}
