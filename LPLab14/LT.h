#pragma once
#ifndef LT_H_
#define LT_H_
#include "Parm.h"
#define LEXEMA_FIXSIZE 1
#define LT_MAXSIZE 4096 
#define LT_TI_NULLIDX INT_MAX
#define LEX_INTEGER 't'
#define LEX_STRING 't'
#define LEX_TYPE 't'
#define LEX_ID 'i'
#define LEX_LITERAL 'l'
#define LEX_FUNCTION 'f'

#define LEX_RETURN 'r'
#define LEX_PRINT 'p'
#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define LEX_BRACELET '}'
#define LEX_LEFTHESIS '('
#define LEX_RIGHTHESIS ')'
#define LEX_PLUS 'v'
#define LEX_MINUS 'v'
#define LEX_STAR 'v'
#define LEX_DIRSLACH 'v'
#define LEX_MAIN 'm'
#define LEX_OPERATOR 'v'
#define LEX_EQUAL '='
#define LEX_BOOL_OPERATOR 'q'
#define LEX_BREAK 'b'
#define LEX_CONTINUE 'c'
#define LEX_ELIF '^'
#define LEX_ENDIF 'n'
#define LEX_REPEAT '!'
#define LEX_UNTIL 'u'
#define LEX_IF '?'
#define LEX_ELSE 'e'
#define LEX_ENDW '*'
#define LEX_WHILE '~'
namespace LT
{
	struct Entry
	{
		char lexema;
		int idxTI;
		int sn;
		int priority;
	};

	struct LexTable
	{
		int maxsize;
		int size;
		Entry* table;
	};
	LexTable Create(int size);

	void Add(LexTable& lextable,const Entry& entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
	void Show(LexTable& lextable, Parm::PARM parm);
}
#endif