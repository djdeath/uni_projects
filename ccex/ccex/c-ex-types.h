#ifndef __C_EX_TYPES_H__
#define __C_EX_TYPES_H__

typedef enum
{
	TYPE_ARRAY = 0x1,
	TYPE_INT   = 0x2,
	TYPE_VOID  = 0x4
} CExType_t;

int yyerror (const char* fmt, ...);

#endif
