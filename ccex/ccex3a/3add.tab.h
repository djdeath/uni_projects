#ifndef BISON_3ADD_TAB_H
# define BISON_3ADD_TAB_H

#ifndef YYSTYPE
typedef union
{
	Variable_t* var;
	Function_t* func;
	List_t* args;
	char* string;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	CMP	257
# define	JC	258
# define	SV	259
# define	JMP	260
# define	FUNC_START	261
# define	FUNC_END	262
# define	CALL	263
# define	RET	264
# define	TRY	265
# define	IDENTIFIER	266
# define	CONSTANT	267
# define	LABEL	268
# define	STRING	269
# define	SSTRING	270


extern YYSTYPE yylval;

#endif /* not BISON_3ADD_TAB_H */
