%{

#include "line.h"
#include "identifier.h"
#include "expression.h"
#include "cex.tab.h"

%}

%x COMMENT
%x COMMENTPP
%x ARRAY_PROP

%%

\r ;
\n { inc_line (); }
<COMMENT>\n { inc_line (); }

<COMMENT>"*/" { BEGIN INITIAL; }
<COMMENTPP>\n {  inc_line (); BEGIN INITIAL; }

<COMMENT>.|\n ;
<COMMENTPP>. ;

<ARRAY_PROP>[ \t]+ ;
<ARRAY_PROP>"length" { BEGIN INITIAL; return KEY_ARRAY_LENGTH; }
<ARRAY_PROP>"dimension" { BEGIN INITIAL; return KEY_ARRAY_DIMENSION; }
<ARRAY_PROP>.|\n ;

[ \t\r]+ ;

"void" { return KEY_VOID; }
"int" { return KEY_INT; }
"array" { return KEY_ARRAY; }
"null" { yylval.expr = expression_new_constant (TYPE_ARRAY, 0); return KEY_NULL; }

"return" { return KEY_RETURN; }

"allocate" { return KEY_ALLOCATE; }
"free" { return KEY_FREE; }

"print" { return KEY_PRINT; }
"input" { return KEY_INPUT; }

"if" { return KEY_IF; }
"else" { return KEY_ELSE; }

"while" { return KEY_WHILE; }

"throwable" { return KEY_THROWABLE; }
"extends" { return KEY_EXTENDS; }

"try" { return KEY_TRY; }
"catch" { return KEY_CATCH; }
"finally" { return KEY_FINALLY; }
"throw" { return KEY_THROW; }

"<=" { return KEY_LTEQ; }
">=" { return KEY_GTEQ; }
"==" { return KEY_EQ; }
"!=" { return KEY_NOTEQ; }
"&&" { return KEY_AND; }
"||" { return KEY_OR; }
"<<" { return KEY_DEC_LEFT; }
">>" { return KEY_DEC_RIGHT; }

[0-9]+ { int v; sscanf (yytext, "%i", &v); yylval.expr = expression_new_constant (TYPE_INT, v); return INTEGER; }
[_a-zA-Z][_a-zA-Z0-9]* { set_identifier (yytext); return IDENTIFIER; }

\"((\\\")|([^\"]))*\" { yylval.string = yytext; return STRING; }

"//" { BEGIN COMMENTPP; }
"/*" { BEGIN COMMENT; }
"." { BEGIN ARRAY_PROP; }

. return yytext[0];

%%
