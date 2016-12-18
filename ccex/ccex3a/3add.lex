%{

#include "line.h"
#include "list.h"
#include "function.h"
#include "variable.h"
#include "identifier.h"
#include "3add.tab.h"

%}

%%

\r ;
\n { inc_line (); }

[ \t] ;

":RET" { return RET; }
":CMP" { return CMP; }
":JMP" { return JMP; }
":TRY" { return TRY; }
":CALL"	{ return CALL; }
":STRING" { return STRING; }
":FUNC_START" { return FUNC_START; }
":FUNC_END" { return FUNC_END; }
":je"|":jne"|":jg"|":jge"|":jl"|":jle" { set_identifier (yytext + 1); return JC; }
":sete"|":setne"|":setg"|":setge"|":setl"|":setle" { set_identifier (yytext + 1); return SV; }

"."[_a-zA-Z][_a-zA-Z0-9]* { /*printf ("->%s\n", yytext);*/ set_identifier (yytext); return LABEL; }

"$"[-]?[0-9]+ { /*printf ("->%s\n", yytext);*/ yylval.var = variable_new_constant (atoi (yytext + 1)); return CONSTANT; }

[_a-zA-Z][_a-zA-Z0-9]* { /*printf ("->%s\n", yytext);*/ set_identifier (yytext); return IDENTIFIER; }

\"((\\\")|([^\"]))*\" { yylval.string = yytext; return SSTRING; }

. return yytext[0];

%%
