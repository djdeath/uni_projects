#ifndef BISON_CEX_TAB_H
# define BISON_CEX_TAB_H

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


extern YYSTYPE yylval;

#endif /* not BISON_CEX_TAB_H */
