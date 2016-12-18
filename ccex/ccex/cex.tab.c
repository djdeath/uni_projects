/* A Bison parser, made from cex.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	IDENTIFIER	257
# define	INTEGER	258
# define	STRING	259
# define	KEY_VOID	260
# define	KEY_INT	261
# define	KEY_ARRAY	262
# define	KEY_THROWABLE	263
# define	KEY_EXTENDS	264
# define	KEY_TRY	265
# define	KEY_CATCH	266
# define	KEY_FINALLY	267
# define	KEY_THROW	268
# define	KEY_WHILE	269
# define	KEY_IF	270
# define	KEY_ELSE	271
# define	KEY_RETURN	272
# define	KEY_LTEQ	273
# define	KEY_GTEQ	274
# define	KEY_NOTEQ	275
# define	KEY_EQ	276
# define	KEY_AND	277
# define	KEY_OR	278
# define	KEY_DEC_LEFT	279
# define	KEY_DEC_RIGHT	280
# define	KEY_ALLOCATE	281
# define	KEY_FREE	282
# define	KEY_NULL	283
# define	KEY_ARRAY_LENGTH	284
# define	KEY_ARRAY_DIMENSION	285
# define	KEY_PRINT	286
# define	KEY_INPUT	287
# define	MOINS_UNAIRE	288
# define	KEY_THEN	289
# define	KEY_END	290
# define	KEY_FCATCH	291

#line 1 "cex.y"

	
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

#line 26 "cex.y"
#ifndef YYSTYPE
typedef union
{
	Expression_t* expr;
	
	Function_t* func;
	List_t* args;
	
	Exception_t* except;
	
	unsigned int label;
	
	Symbol_t* sym;
	
	char* string;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		258
#define	YYFLAG		-32768
#define	YYNTBASE	59

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 291 ? yytranslate[x] : 127)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    47,     2,     2,     2,    44,    37,     2,
      56,    57,    42,    40,    58,    41,     2,    43,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    53,
      38,    34,    39,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    54,     2,    55,    36,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    51,    35,    52,    46,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    45,    48,
      49,    50
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     4,     6,     8,     9,    12,    13,    18,
      20,    23,    26,    31,    39,    49,    52,    55,    58,    60,
      62,    64,    67,    70,    72,    75,    76,    78,    79,    85,
      86,    87,    96,   100,   104,   106,   110,   111,   112,   113,
     114,   126,   127,   130,   131,   136,   138,   141,   142,   148,
     149,   152,   153,   158,   159,   160,   164,   165,   168,   169,
     174,   175,   178,   182,   185,   188,   192,   196,   200,   204,
     208,   212,   216,   220,   224,   228,   230,   235,   242,   248,
     251,   253,   255,   257,   259,   261,   263,   267,   271,   275,
     279,   283,   287,   288,   294,   295,   301,   304,   306,   309,
     311,   314,   318,   320,   324,   326,   329,   332,   337,   342,
     345,   348,   351,   353,   357,   361,   367,   369,   371,   375,
     379,   385,   387,   388,   389,   397,   398,   403,   406,   409,
     412,   415,   419,   420,   425,   427,   431,   434,   437,   438,
     444
};
static const short yyrhs[] =
{
      -1,    59,    60,     0,   124,     0,   115,     0,     0,    62,
      65,     0,     0,    51,    63,    64,    52,     0,    65,     0,
      64,    65,     0,   110,    53,     0,   101,    34,    98,    53,
       0,   101,    54,   105,    55,    34,    98,    53,     0,   101,
      54,    54,   101,    55,    55,    34,    98,    53,     0,   102,
      53,     0,   109,    53,     0,   107,    53,     0,    73,     0,
      68,     0,    76,     0,    96,    53,     0,    66,    53,     0,
      18,     0,    18,    98,     0,     0,    70,     0,     0,    70,
      17,    67,    69,    61,     0,     0,     0,    16,    71,    56,
      98,    57,    67,    72,    61,     0,    74,    75,    61,     0,
      74,    75,    53,     0,    15,     0,    56,    98,    57,     0,
       0,     0,     0,     0,    11,    95,    77,    67,    67,    78,
      81,    79,    84,    80,    90,     0,     0,    82,    65,     0,
       0,    51,    83,    64,    52,     0,    85,     0,    85,    84,
       0,     0,    12,    67,     3,    86,    87,     0,     0,    88,
      65,     0,     0,    51,    89,    64,    52,     0,     0,     0,
      13,    91,    92,     0,     0,    93,    65,     0,     0,    51,
      94,    64,    52,     0,     0,    14,     3,     0,    56,    97,
      57,     0,    41,    97,     0,    46,    97,     0,    97,    40,
      97,     0,    97,    41,    97,     0,    97,    43,    97,     0,
      97,    42,    97,     0,    97,    44,    97,     0,    97,    37,
      97,     0,    97,    36,    97,     0,    97,    35,    97,     0,
      97,    25,    97,     0,    97,    26,    97,     0,   101,     0,
     101,    54,   105,    55,     0,   101,    54,    54,   101,    55,
      55,     0,   101,    30,    56,    97,    57,     0,   101,    31,
       0,   102,     0,   108,     0,   106,     0,     4,     0,    29,
       0,    97,     0,    97,    39,    97,     0,    97,    38,    97,
       0,    97,    19,    97,     0,    97,    20,    97,     0,    97,
      21,    97,     0,    97,    22,    97,     0,     0,    98,    23,
      67,    99,    98,     0,     0,    98,    24,    67,   100,    98,
       0,    47,    98,     0,     3,     0,   103,   104,     0,     3,
       0,    56,    57,     0,    56,   105,    57,     0,    98,     0,
     105,    58,    98,     0,    33,     0,    32,    98,     0,    32,
       5,     0,    27,    56,   105,    57,     0,    27,    54,    97,
      55,     0,    28,   101,     0,     7,   111,     0,     8,   113,
       0,   112,     0,   112,    34,    98,     0,   111,    58,   112,
       0,   111,    58,   112,    34,    98,     0,     3,     0,   114,
       0,   114,    34,    98,     0,   113,    58,   114,     0,   113,
      58,   114,    34,    98,     0,     3,     0,     0,     0,   119,
     120,   116,    51,   117,    64,    52,     0,     0,   119,   120,
     118,    53,     0,     6,     3,     0,     7,     3,     0,     8,
       3,     0,    56,    57,     0,    56,     6,    57,     0,     0,
      56,   121,   122,    57,     0,   123,     0,   123,    58,   122,
       0,     7,     3,     0,     8,     3,     0,     0,   126,    10,
       3,   125,    53,     0,     9,     3,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   137,   138,   141,   142,   145,   145,   146,   146,   149,
     150,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   167,   168,   173,   178,   179,   179,   196,
     196,   196,   201,   202,   205,   208,   213,   213,   213,   213,
     213,   217,   217,   218,   218,   221,   222,   225,   225,   228,
     228,   229,   229,   232,   233,   233,   236,   236,   237,   237,
     240,   245,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   275,   276,   277,   278,   279,
     280,   281,   282,   282,   283,   283,   284,   287,   290,   293,
     296,   297,   300,   301,   304,   307,   308,   311,   312,   315,
     320,   321,   324,   328,   332,   336,   342,   345,   349,   353,
     357,   363,   368,   368,   368,   372,   372,   375,   376,   377,
     380,   381,   382,   382,   385,   386,   389,   390,   395,   395,
     398
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "IDENTIFIER", "INTEGER", "STRING", 
  "KEY_VOID", "KEY_INT", "KEY_ARRAY", "KEY_THROWABLE", "KEY_EXTENDS", 
  "KEY_TRY", "KEY_CATCH", "KEY_FINALLY", "KEY_THROW", "KEY_WHILE", 
  "KEY_IF", "KEY_ELSE", "KEY_RETURN", "KEY_LTEQ", "KEY_GTEQ", "KEY_NOTEQ", 
  "KEY_EQ", "KEY_AND", "KEY_OR", "KEY_DEC_LEFT", "KEY_DEC_RIGHT", 
  "KEY_ALLOCATE", "KEY_FREE", "KEY_NULL", "KEY_ARRAY_LENGTH", 
  "KEY_ARRAY_DIMENSION", "KEY_PRINT", "KEY_INPUT", "'='", "'|'", "'^'", 
  "'&'", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "MOINS_UNAIRE", 
  "'~'", "'!'", "KEY_THEN", "KEY_END", "KEY_FCATCH", "'{'", "'}'", "';'", 
  "'['", "']'", "'('", "')'", "','", "PROGRAMME", "define", 
  "instruction_seq", "@1", "@2", "instruction_seq_", "instruction", "ret", 
  "label", "test_seq", "@3", "test_seq_if", "@4", "@5", "loop_seq", 
  "loop_seq_start", "loop_seq_end", "try_seq", "@6", "@7", "@8", "@9", 
  "instruction_try_seq", "@10", "@11", "catch_seq", "catch", "@12", 
  "instruction_catch_seq", "@13", "@14", "finally_seq", "@15", 
  "instruction_seq_finally", "@16", "@17", "except_var", "throw", "expr", 
  "cond", "@18", "@19", "call_variable", "call_function", 
  "call_function_", "parameters", "parameters_", "input", "print", 
  "allocate", "free", "define_variable", "define_variable_int", 
  "define_variable_int_", "define_variable_array", 
  "define_variable_array_", "define_function", "@20", "@21", "@22", 
  "define_function_type", "define_parameters", "@23", 
  "define_parameters_", "define_parameters__", "define_exception", "@24", 
  "define_exception_throwable", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    59,    59,    60,    60,    62,    61,    63,    61,    64,
      64,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    66,    66,    67,    68,    69,    68,    71,
      72,    70,    73,    73,    74,    75,    77,    78,    79,    80,
      76,    82,    81,    83,    81,    84,    84,    86,    85,    88,
      87,    89,    87,    90,    91,    90,    93,    92,    94,    92,
      95,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    98,    98,    98,    98,    98,
      98,    98,    99,    98,   100,    98,    98,   101,   102,   103,
     104,   104,   105,   105,   106,   107,   107,   108,   108,   109,
     110,   110,   111,   111,   111,   111,   112,   113,   113,   113,
     113,   114,   116,   117,   115,   118,   115,   119,   119,   119,
     120,   120,   121,   120,   122,   122,   123,   123,   125,   124,
     126
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     1,     1,     0,     2,     0,     4,     1,
       2,     2,     4,     7,     9,     2,     2,     2,     1,     1,
       1,     2,     2,     1,     2,     0,     1,     0,     5,     0,
       0,     8,     3,     3,     1,     3,     0,     0,     0,     0,
      11,     0,     2,     0,     4,     1,     2,     0,     5,     0,
       2,     0,     4,     0,     0,     3,     0,     2,     0,     4,
       0,     2,     3,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     4,     6,     5,     2,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     0,     5,     0,     5,     2,     1,     2,     1,
       2,     3,     1,     3,     1,     2,     2,     4,     4,     2,
       2,     2,     1,     3,     3,     5,     1,     1,     3,     3,
       5,     1,     0,     0,     7,     0,     4,     2,     2,     2,
       2,     3,     0,     4,     1,     3,     2,     2,     0,     5,
       2
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     0,     0,     0,     0,     0,     2,     4,     0,     3,
       0,   127,   128,   129,   140,   132,   122,     0,     0,   130,
       0,     0,     0,   138,   131,     0,     0,     0,   134,   123,
     126,     0,   136,   137,   133,     0,     0,   139,   135,    97,
       0,     0,    60,     0,    34,    29,    23,     0,     0,     0,
       9,     0,    19,    26,    18,     0,    20,     0,     0,     0,
       0,     0,     0,     0,   116,   110,   112,   121,   111,   117,
      36,    61,     0,    83,     0,    84,   104,     0,     0,     0,
       0,    85,    24,    75,    80,    82,    81,    97,   109,   106,
     105,   124,    10,    22,    25,     0,     5,    21,     0,     0,
      15,     0,    98,    17,    16,    11,     0,     0,     0,     0,
      25,     0,     0,     0,    63,    64,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,    25,     0,    79,     0,    27,
       0,     7,    33,    32,     0,     0,     0,   102,     0,   100,
       0,   114,   113,   119,   118,    25,     0,     0,     0,    62,
      88,    89,    90,    91,    73,    74,    72,    71,    70,    87,
      86,    65,    66,    68,    67,    69,    92,    94,     0,     0,
       0,     5,    35,     0,     6,    12,     0,     0,     0,   101,
       0,     0,    37,    25,   108,   107,     0,     0,     0,     0,
      76,    28,     0,     0,     0,   103,   115,   120,    41,    30,
      93,    95,    78,     0,     8,     0,     0,    43,    38,     0,
       5,    77,     0,    13,     0,     0,    42,    31,     0,     0,
      25,    39,    45,    14,    44,     0,    53,    46,    47,    54,
      40,    49,    56,    51,    48,     0,    58,    55,     0,     0,
      50,     0,    57,     0,     0,    52,    59,     0,     0
};

static const short yydefgoto[] =
{
       1,     6,   143,   144,   183,    49,    50,    51,   139,    52,
     181,    53,    72,   220,    54,    55,    96,    56,   110,   208,
     225,   236,   218,   219,   224,   231,   232,   241,   244,   245,
     249,   240,   242,   247,   248,   251,    70,    57,    81,   147,
     196,   197,    83,    84,    60,   102,   148,    85,    61,    86,
      62,    63,    65,    66,    68,    69,     7,    21,    36,    22,
       8,    16,    20,    27,    28,     9,    31,    10
};

static const short yypact[] =
{
  -32768,   105,     1,     7,    24,    28,-32768,-32768,   -19,-32768,
      34,-32768,-32768,-32768,-32768,    -3,    13,    74,    11,-32768,
      95,    30,    26,-32768,-32768,    87,    89,    47,    64,-32768,
  -32768,    45,-32768,-32768,-32768,    95,   424,-32768,-32768,    67,
     123,   132,-32768,   136,-32768,-32768,   245,   137,    53,   113,
  -32768,    93,-32768,   125,-32768,    92,-32768,    98,    -1,    99,
      97,   114,   116,   121,-32768,   100,   122,-32768,   117,   144,
  -32768,-32768,   124,-32768,    20,-32768,-32768,    14,    14,   245,
      14,   425,    94,    65,-32768,-32768,-32768,-32768,-32768,-32768,
      94,-32768,-32768,-32768,-32768,   245,   -30,-32768,   245,   103,
  -32768,     5,-32768,-32768,-32768,-32768,   123,   245,   132,   245,
  -32768,   245,    14,   245,-32768,-32768,-32768,   336,    14,    14,
      14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
      14,    14,    14,    14,-32768,-32768,   129,-32768,   242,-32768,
      -9,-32768,-32768,-32768,   424,    12,   137,    94,   -39,-32768,
      76,   145,    94,   147,    94,-32768,    16,   382,    80,-32768,
     445,   445,   445,   445,   120,   120,   458,   295,   432,   445,
     445,    41,    41,-32768,-32768,-32768,-32768,-32768,    14,   137,
     -33,   140,-32768,   424,-32768,-32768,   139,   158,   245,-32768,
     245,   245,-32768,-32768,-32768,-32768,   245,   245,   359,   141,
  -32768,-32768,   248,   142,   245,    94,    94,    94,   148,-32768,
  -32768,   180,-32768,   149,-32768,   171,    40,-32768,-32768,   424,
     140,-32768,   245,-32768,   424,   196,-32768,-32768,    48,   296,
  -32768,-32768,   196,-32768,-32768,   207,   198,-32768,-32768,-32768,
  -32768,   163,   172,-32768,-32768,   424,-32768,-32768,   424,   424,
  -32768,   424,-32768,   315,   338,-32768,-32768,   226,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,  -175,-32768,-32768,  -182,   -47,-32768,  -105,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,    15,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   109,   -20,
  -32768,-32768,   -36,   -29,-32768,-32768,   -89,-32768,-32768,-32768,
  -32768,-32768,-32768,   138,-32768,   135,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   215,-32768,-32768,-32768,-32768
};


#define	YYLAST		502


static const short yytable[] =
{
      58,   202,    92,    18,    11,   155,   201,    59,    39,    73,
      12,    88,   150,    58,   134,   135,   187,    39,    73,   188,
      59,   141,   200,   142,   158,   188,    82,    13,    90,   176,
     177,    14,    74,    98,    75,   134,   135,    15,    76,   134,
     135,    74,   229,    75,    17,   227,    77,    76,   182,   180,
     192,    78,    79,    99,    19,    77,    39,    73,    89,   116,
      78,    80,   149,   134,   135,   185,  -125,   253,    24,   254,
      80,   134,   135,   193,   112,   140,   113,    23,   145,    30,
      74,    29,    75,   131,   132,   133,    76,   152,   209,   154,
      32,   156,    33,   223,    77,   136,   137,   184,    37,    78,
      79,   233,    25,    26,    34,   257,    39,    73,    58,    80,
     186,     2,     3,     4,     5,    59,    39,   134,   135,   138,
      40,    41,    35,   -99,    42,   235,    64,    43,    44,    45,
      74,    46,    75,   189,   188,    67,    76,   195,   188,    71,
      87,    47,    94,   199,    77,    48,    93,    58,    95,    78,
      79,    97,   100,   101,    59,    92,   107,   146,   106,    80,
     129,   130,   131,   132,   133,    91,    58,   103,   205,   104,
     206,   207,   226,    59,   105,   108,   210,   211,   109,   190,
     111,   191,    92,    58,   216,   178,   114,   115,    58,   117,
      59,   141,   204,    58,   203,    59,   213,   215,   250,   217,
      59,   252,   228,   134,   221,   222,    92,    92,   230,    58,
     238,   239,    58,    58,   243,    58,    59,    58,    58,    59,
      59,   157,    59,   246,    59,    59,   258,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   153,   151,    39,    73,   237,    39,    73,
      38,    39,     0,     0,     0,    40,    41,     0,     0,    42,
       0,     0,    43,    44,    45,     0,    46,     0,     0,    74,
       0,    75,    74,     0,    75,    76,    47,     0,    76,     0,
      48,     0,     0,    77,     0,     0,    77,   198,    78,    79,
       0,    78,    79,     0,     0,     0,   179,     0,    80,    39,
     214,    80,     0,    40,    41,     0,     0,    42,     0,     0,
      43,    44,    45,     0,    46,     0,     0,     0,    39,     0,
     122,   123,    40,    41,    47,     0,    42,     0,    48,    43,
      44,    45,   126,    46,     0,   129,   130,   131,   132,   133,
       0,    39,     0,    47,     0,    40,    41,    48,   234,    42,
       0,     0,    43,    44,    45,     0,    46,     0,     0,     0,
       0,   122,   123,     0,     0,     0,    47,   255,     0,     0,
      48,   124,   125,   126,     0,     0,   129,   130,   131,   132,
     133,     0,     0,     0,   122,   123,     0,     0,     0,     0,
     256,     0,     0,   159,   124,   125,   126,     0,     0,   129,
     130,   131,   132,   133,     0,     0,     0,   122,   123,     0,
       0,     0,     0,     0,     0,     0,   212,   124,   125,   126,
       0,     0,   129,   130,   131,   132,   133,    39,     0,     0,
       0,    40,    41,     0,     0,    42,     0,   194,    43,    44,
      45,     0,    46,     0,   118,   119,   120,   121,     0,     0,
     122,   123,    47,     0,     0,     0,    48,   122,   123,     0,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     122,   123,   129,   130,   131,   132,   133,     0,     0,     0,
     124,   125,   126,   122,   123,   129,   130,   131,   132,   133,
       0,     0,     0,     0,   125,   126,     0,     0,   129,   130,
     131,   132,   133
};

static const short yycheck[] =
{
      36,   183,    49,     6,     3,   110,   181,    36,     3,     4,
       3,    47,   101,    49,    23,    24,    55,     3,     4,    58,
      49,    51,    55,    53,   113,    58,    46,     3,    48,   134,
     135,     3,    27,    34,    29,    23,    24,    56,    33,    23,
      24,    27,   224,    29,    10,   220,    41,    33,    57,   138,
     155,    46,    47,    54,    57,    41,     3,     4,     5,    79,
      46,    56,    57,    23,    24,    53,    53,   249,    57,   251,
      56,    23,    24,    57,    54,    95,    56,     3,    98,    53,
      27,    51,    29,    42,    43,    44,    33,   107,   193,   109,
       3,   111,     3,    53,    41,    30,    31,   144,    53,    46,
      47,    53,     7,     8,    57,     0,     3,     4,   144,    56,
     146,     6,     7,     8,     9,   144,     3,    23,    24,    54,
       7,     8,    58,    56,    11,   230,     3,    14,    15,    16,
      27,    18,    29,    57,    58,     3,    33,    57,    58,     3,
       3,    28,    17,   179,    41,    32,    53,   183,    56,    46,
      47,    53,    53,    56,   183,   202,    34,    54,    58,    56,
      40,    41,    42,    43,    44,    52,   202,    53,   188,    53,
     190,   191,   219,   202,    53,    58,   196,   197,    34,    34,
      56,    34,   229,   219,   204,    56,    77,    78,   224,    80,
     219,    51,    34,   229,    55,   224,    55,    55,   245,    51,
     229,   248,   222,    23,    55,    34,   253,   254,    12,   245,
       3,    13,   248,   249,    51,   251,   245,   253,   254,   248,
     249,   112,   251,    51,   253,   254,     0,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   108,   106,     3,     4,   232,     3,     4,
      35,     3,    -1,    -1,    -1,     7,     8,    -1,    -1,    11,
      -1,    -1,    14,    15,    16,    -1,    18,    -1,    -1,    27,
      -1,    29,    27,    -1,    29,    33,    28,    -1,    33,    -1,
      32,    -1,    -1,    41,    -1,    -1,    41,   178,    46,    47,
      -1,    46,    47,    -1,    -1,    -1,    54,    -1,    56,     3,
      52,    56,    -1,     7,     8,    -1,    -1,    11,    -1,    -1,
      14,    15,    16,    -1,    18,    -1,    -1,    -1,     3,    -1,
      25,    26,     7,     8,    28,    -1,    11,    -1,    32,    14,
      15,    16,    37,    18,    -1,    40,    41,    42,    43,    44,
      -1,     3,    -1,    28,    -1,     7,     8,    32,    52,    11,
      -1,    -1,    14,    15,    16,    -1,    18,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    28,    52,    -1,    -1,
      32,    35,    36,    37,    -1,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    57,    35,    36,    37,    -1,    -1,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    35,    36,    37,
      -1,    -1,    40,    41,    42,    43,    44,     3,    -1,    -1,
      -1,     7,     8,    -1,    -1,    11,    -1,    55,    14,    15,
      16,    -1,    18,    -1,    19,    20,    21,    22,    -1,    -1,
      25,    26,    28,    -1,    -1,    -1,    32,    25,    26,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      25,    26,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      35,    36,    37,    25,    26,    40,    41,    42,    43,    44,
      -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    40,    41,
      42,    43,    44
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 5:
#line 145 "cex.y"
{ context_up (CONTEXT_BLOCK); ;
    break;}
case 6:
#line 145 "cex.y"
{ yyval.label = context_returned (); context_down (); ;
    break;}
case 7:
#line 146 "cex.y"
{ context_up (CONTEXT_BLOCK); ;
    break;}
case 8:
#line 146 "cex.y"
{ yyval.label = context_returned (); context_down (); ;
    break;}
case 12:
#line 154 "cex.y"
{ do_affect_var (yyvsp[-3].expr, yyvsp[-1].expr); ;
    break;}
case 13:
#line 155 "cex.y"
{ do_affect_array (yyvsp[-6].expr, yyvsp[-4].args, yyvsp[-1].expr); ;
    break;}
case 14:
#line 156 "cex.y"
{ do_affect_array_a (yyvsp[-8].expr, yyvsp[-5].expr, yyvsp[-1].expr); ;
    break;}
case 15:
#line 157 "cex.y"
{ expression_free (yyvsp[-1].expr); ;
    break;}
case 23:
#line 167 "cex.y"
{ do_return (NULL); ;
    break;}
case 24:
#line 168 "cex.y"
{ do_return (yyvsp[0].expr); ;
    break;}
case 25:
#line 173 "cex.y"
{ yyval.label = label_new (); ;
    break;}
case 26:
#line 178 "cex.y"
{ do_ifend (yyvsp[0].label); context_down (); ;
    break;}
case 27:
#line 180 "cex.y"
{
	do_ifelse (yyvsp[0].label);
	do_ifend (yyvsp[-2].label);
;
    break;}
case 28:
#line 184 "cex.y"
{
	do_ifend (yyvsp[-2].label);
	if (context_returned ())
	{
		context_down ();
		context_set_returned (2);
	}
	else
		context_down ();
;
    break;}
case 29:
#line 196 "cex.y"
{ context_up (CONTEXT_TEST); ;
    break;}
case 30:
#line 196 "cex.y"
{ do_if (yyvsp[-2].expr, yyvsp[0].label); ;
    break;}
case 31:
#line 196 "cex.y"
{ yyval.label = yyvsp[-2].label; ;
    break;}
case 32:
#line 201 "cex.y"
{ do_whileend (yyvsp[-2].label, yyvsp[-1].label); ;
    break;}
case 33:
#line 202 "cex.y"
{ do_whileend (yyvsp[-2].label, yyvsp[-1].label); ;
    break;}
case 34:
#line 205 "cex.y"
{ yyval.label = label_new (); do_while_start (yyval.label); ;
    break;}
case 35:
#line 208 "cex.y"
{ yyval.label = label_new (); do_while (yyvsp[-1].expr, yyval.label); ;
    break;}
case 36:
#line 213 "cex.y"
{ context_up_except (yyvsp[0].expr); ;
    break;}
case 37:
#line 213 "cex.y"
{ do_try (yyvsp[-1].label); ;
    break;}
case 38:
#line 214 "cex.y"
{ do_tryend (yyvsp[-3].label, yyvsp[-2].label, yyvsp[-5].expr); ;
    break;}
case 39:
#line 214 "cex.y"
{ do_finally (yyvsp[-4].label); ;
    break;}
case 40:
#line 214 "cex.y"
{ do_finallyend (); expression_free (yyvsp[-9].expr); context_down (); ;
    break;}
case 41:
#line 217 "cex.y"
{ context_up (CONTEXT_TRY); ;
    break;}
case 42:
#line 217 "cex.y"
{ context_down (); ;
    break;}
case 43:
#line 218 "cex.y"
{ context_up (CONTEXT_TRY); ;
    break;}
case 44:
#line 218 "cex.y"
{ context_down (); ;
    break;}
case 47:
#line 225 "cex.y"
{ do_catch (get_identifier (), yyvsp[-1].label); ;
    break;}
case 48:
#line 225 "cex.y"
{ do_catchend (yyvsp[-3].label); ;
    break;}
case 49:
#line 228 "cex.y"
{ context_up (CONTEXT_CATCH); ;
    break;}
case 50:
#line 228 "cex.y"
{ context_down (); ;
    break;}
case 51:
#line 229 "cex.y"
{ context_up (CONTEXT_CATCH); ;
    break;}
case 52:
#line 229 "cex.y"
{ context_down (); ;
    break;}
case 54:
#line 233 "cex.y"
{ do_finallystart (); ;
    break;}
case 56:
#line 236 "cex.y"
{ context_up (CONTEXT_FINALLY); ;
    break;}
case 57:
#line 236 "cex.y"
{ context_down (); ;
    break;}
case 58:
#line 237 "cex.y"
{ context_up (CONTEXT_FINALLY); ;
    break;}
case 59:
#line 237 "cex.y"
{ context_down (); ;
    break;}
case 60:
#line 240 "cex.y"
{ yyval.expr = expression_new_temporary (TYPE_INT); ;
    break;}
case 61:
#line 245 "cex.y"
{ do_throw (get_identifier ()); ;
    break;}
case 62:
#line 250 "cex.y"
{ yyval.expr = yyvsp[-1].expr; ;
    break;}
case 63:
#line 251 "cex.y"
{ yyval.expr = do_unary ('-', yyvsp[0].expr); ;
    break;}
case 64:
#line 252 "cex.y"
{ yyval.expr = do_unary ('~', yyvsp[0].expr); ;
    break;}
case 65:
#line 253 "cex.y"
{ yyval.expr = do_binary ('+', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 66:
#line 254 "cex.y"
{ yyval.expr = do_binary ('-', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 67:
#line 255 "cex.y"
{ yyval.expr = do_binary ('/', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 68:
#line 256 "cex.y"
{ yyval.expr = do_binary ('*', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 69:
#line 257 "cex.y"
{ yyval.expr = do_binary ('%', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 70:
#line 258 "cex.y"
{ yyval.expr = do_binary ('&', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 71:
#line 259 "cex.y"
{ yyval.expr = do_binary ('^', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 72:
#line 260 "cex.y"
{ yyval.expr = do_binary ('|', yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 73:
#line 261 "cex.y"
{ yyval.expr = do_dec (KEY_DEC_LEFT, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 74:
#line 262 "cex.y"
{ yyval.expr = do_dec (KEY_DEC_RIGHT, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 75:
#line 263 "cex.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 76:
#line 264 "cex.y"
{ yyval.expr = do_array_get (yyvsp[-3].expr, yyvsp[-1].args); ;
    break;}
case 77:
#line 265 "cex.y"
{ yyval.expr = do_array_get_a (yyvsp[-5].expr, yyvsp[-2].expr); ;
    break;}
case 78:
#line 266 "cex.y"
{ yyval.expr = do_array_get_lenght (yyvsp[-4].expr, yyvsp[-1].expr); ;
    break;}
case 79:
#line 267 "cex.y"
{ yyval.expr = do_array_get_dimension (yyvsp[-1].expr); ;
    break;}
case 80:
#line 268 "cex.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 81:
#line 269 "cex.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 82:
#line 270 "cex.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 83:
#line 271 "cex.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 84:
#line 272 "cex.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 85:
#line 275 "cex.y"
{ yyval.expr = yyvsp[0].expr ;
    break;}
case 86:
#line 276 "cex.y"
{ yyval.expr = do_cmp ('>', 0, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 87:
#line 277 "cex.y"
{ yyval.expr = do_cmp ('<', 0, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 88:
#line 278 "cex.y"
{ yyval.expr = do_cmp ('\0', KEY_LTEQ, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 89:
#line 279 "cex.y"
{ yyval.expr = do_cmp ('\0', KEY_GTEQ, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 90:
#line 280 "cex.y"
{ yyval.expr = do_cmp ('\0', KEY_NOTEQ, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 91:
#line 281 "cex.y"
{ yyval.expr = do_cmp ('\0', KEY_EQ, yyvsp[-2].expr, yyvsp[0].expr); ;
    break;}
case 92:
#line 282 "cex.y"
{ do_and (yyvsp[-2].expr, yyvsp[0].label); ;
    break;}
case 93:
#line 282 "cex.y"
{ yyval.expr = do_andend (yyvsp[0].expr, yyvsp[-4].expr, yyvsp[-2].label); ;
    break;}
case 94:
#line 283 "cex.y"
{ do_or (yyvsp[-2].expr, yyvsp[0].label); ;
    break;}
case 95:
#line 283 "cex.y"
{ yyval.expr = do_orend (yyvsp[0].expr, yyvsp[-4].expr, yyvsp[-2].label); ;
    break;}
case 96:
#line 284 "cex.y"
{ yyval.expr = do_cmp ('\0', KEY_EQ, yyvsp[0].expr, expression_new_constant (TYPE_INT, 0)); ;
    break;}
case 97:
#line 287 "cex.y"
{ yyval.expr = expression_new_symbol (SYMBOL (context_get_var (get_identifier ()))); ;
    break;}
case 98:
#line 290 "cex.y"
{ yyval.expr = do_call (yyvsp[-1].func, yyvsp[0].args); ;
    break;}
case 99:
#line 293 "cex.y"
{ yyval.func = context_get_func (get_identifier ()); ;
    break;}
case 100:
#line 296 "cex.y"
{ yyval.args = NULL; ;
    break;}
case 101:
#line 297 "cex.y"
{ yyval.args = yyvsp[-1].args; ;
    break;}
case 102:
#line 300 "cex.y"
{ yyval.args = list_new (yyvsp[0].expr); ;
    break;}
case 103:
#line 301 "cex.y"
{ list_append (&yyvsp[-2].args, yyvsp[0].expr); yyval.args = yyvsp[-2].args; ;
    break;}
case 104:
#line 304 "cex.y"
{ yyval.expr = do_input (); ;
    break;}
case 105:
#line 307 "cex.y"
{ do_print_expr (yyvsp[0].expr); ;
    break;}
case 106:
#line 308 "cex.y"
{ do_print_string (string_new (), yyvsp[0].string); ;
    break;}
case 107:
#line 311 "cex.y"
{ yyval.expr = do_allocate (yyvsp[-1].args); ;
    break;}
case 108:
#line 312 "cex.y"
{ yyval.expr = do_allocate_a (yyvsp[-1].expr); ;
    break;}
case 109:
#line 315 "cex.y"
{ do_free (yyvsp[0].expr); ;
    break;}
case 112:
#line 325 "cex.y"
{
	do_affect_var (yyvsp[0].expr, expression_new_constant (TYPE_INT, 0));
;
    break;}
case 113:
#line 329 "cex.y"
{
	do_affect_var (yyvsp[-2].expr, yyvsp[0].expr);
;
    break;}
case 114:
#line 333 "cex.y"
{
	do_affect_var (yyvsp[0].expr, expression_new_constant (TYPE_INT, 0));
;
    break;}
case 115:
#line 337 "cex.y"
{
	do_affect_var (yyvsp[-2].expr, yyvsp[0].expr);
;
    break;}
case 116:
#line 342 "cex.y"
{ yyval.expr = expression_new_symbol (SYMBOL (context_add_var (TYPE_INT, get_identifier ()))); ;
    break;}
case 117:
#line 346 "cex.y"
{
	do_affect_var (yyvsp[0].expr, expression_new_constant (TYPE_ARRAY, 0));
;
    break;}
case 118:
#line 350 "cex.y"
{
	do_affect_var (yyvsp[-2].expr, yyvsp[0].expr);
;
    break;}
case 119:
#line 354 "cex.y"
{
	do_affect_var (yyvsp[0].expr, expression_new_constant (TYPE_ARRAY, 0));
;
    break;}
case 120:
#line 358 "cex.y"
{
	do_affect_var (yyvsp[-2].expr, yyvsp[0].expr);
;
    break;}
case 121:
#line 363 "cex.y"
{ yyval.expr = expression_new_symbol (SYMBOL (context_add_var (TYPE_ARRAY, get_identifier ()))); ;
    break;}
case 122:
#line 369 "cex.y"
{ function_check_args_def (yyvsp[-1].func, yyvsp[0].args); do_func_start (yyvsp[-1].func); ;
    break;}
case 123:
#line 370 "cex.y"
{ context_up_func (yyvsp[-3].func); ;
    break;}
case 124:
#line 371 "cex.y"
{ do_returned (yyvsp[-6].func); context_down (); context_down (); do_func_end (yyvsp[-6].func); ;
    break;}
case 125:
#line 372 "cex.y"
{ function_check_args_def (yyvsp[-1].func, yyvsp[0].args); context_down (); ;
    break;}
case 127:
#line 375 "cex.y"
{ yyval.func = FUNCTION (context_add_func_defined (TYPE_VOID, get_identifier ())); ;
    break;}
case 128:
#line 376 "cex.y"
{ yyval.func = FUNCTION (context_add_func_defined (TYPE_INT, get_identifier ())); ;
    break;}
case 129:
#line 377 "cex.y"
{ yyval.func = FUNCTION (context_add_func_defined (TYPE_ARRAY, get_identifier ())); ;
    break;}
case 130:
#line 380 "cex.y"
{ context_up (CONTEXT_FUNC_ARG); yyval.args = NULL; ;
    break;}
case 131:
#line 381 "cex.y"
{ context_up (CONTEXT_FUNC_ARG); yyval.args = NULL; ;
    break;}
case 132:
#line 382 "cex.y"
{ context_up (CONTEXT_FUNC_ARG); ;
    break;}
case 133:
#line 382 "cex.y"
{ yyval.args = yyvsp[-1].args; ;
    break;}
case 134:
#line 385 "cex.y"
{ yyval.args = list_new (yyvsp[0].sym); ;
    break;}
case 135:
#line 386 "cex.y"
{ list_insert_first (&yyvsp[0].args, yyvsp[-2].sym); yyval.args = yyvsp[0].args; ;
    break;}
case 136:
#line 389 "cex.y"
{ yyval.sym = SYMBOL (context_add_var_arg (TYPE_INT, get_identifier ())); ;
    break;}
case 137:
#line 390 "cex.y"
{ yyval.sym = SYMBOL (context_add_var_arg (TYPE_ARRAY, get_identifier ())); ;
    break;}
case 138:
#line 395 "cex.y"
{ exception_set_parent (yyvsp[-2].except, get_identifier ()); ;
    break;}
case 140:
#line 398 "cex.y"
{ yyval.except = context_add_except (get_identifier ()); ;
    break;}
}

#line 705 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 401 "cex.y"


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
