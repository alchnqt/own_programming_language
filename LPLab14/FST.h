#pragma once
#include <cstddef> //NULL

#define FST_VAR_13 9, \
		FST::NODE(1, FST::RELATION('p', 1)), \
		FST::NODE(3, FST::RELATION('f', 1), FST::RELATION('f', 2), FST::RELATION('f', 6)), \
		FST::NODE(1, FST::RELATION('t', 3)), \
		FST::NODE(4, FST::RELATION('c', 4), FST::RELATION('c', 6), FST::RELATION('s', 5), FST::RELATION('s', 5)), \
		FST::NODE(2, FST::RELATION('y', 4), FST::RELATION('y', 6)), \
		FST::NODE(2, FST::RELATION('x', 5), FST::RELATION('x', 6)), \
		FST::NODE(3, FST::RELATION('f', 7), FST::RELATION('f', 6), FST::RELATION('t', 3)), \
		FST::NODE(1, FST::RELATION('e', 8)), \
		FST::NODE()

#define FST_INTEGER 8, \
		FST::NODE(1, FST::RELATION('i', 1)),\
		FST::NODE(1, FST::RELATION('n', 2)),\
		FST::NODE(1, FST::RELATION('t', 3)),\
		FST::NODE(1, FST::RELATION('e', 4)),\
		FST::NODE(1, FST::RELATION('g', 5)),\
		FST::NODE(1, FST::RELATION('e', 6)),\
		FST::NODE(1, FST::RELATION('r', 7)),\
FST::NODE()

#define FST_STRING 7, \
		FST::NODE(1, FST::RELATION('s', 1)),\
		FST::NODE(1, FST::RELATION('t', 2)),\
		FST::NODE(1, FST::RELATION('r', 3)),\
		FST::NODE(1, FST::RELATION('i', 4)),\
		FST::NODE(1, FST::RELATION('n', 5)),\
		FST::NODE(1, FST::RELATION('g', 6)),\
FST::NODE()

#define FST_FUNCTION 9, \
		FST::NODE(1, FST::RELATION('f', 1)),\
		FST::NODE(1, FST::RELATION('u', 2)),\
		FST::NODE(1, FST::RELATION('n', 3)),\
		FST::NODE(1, FST::RELATION('c', 4)),\
		FST::NODE(1, FST::RELATION('t', 5)),\
		FST::NODE(1, FST::RELATION('i', 6)),\
		FST::NODE(1, FST::RELATION('o', 7)),\
		FST::NODE(1, FST::RELATION('n', 8)),\
FST::NODE()

#define FST_DECLARE 8, \
		FST::NODE(1, FST::RELATION('d', 1)),\
		FST::NODE(1, FST::RELATION('e', 2)),\
		FST::NODE(1, FST::RELATION('c', 3)),\
		FST::NODE(1, FST::RELATION('l', 4)),\
		FST::NODE(1, FST::RELATION('a', 5)),\
		FST::NODE(1, FST::RELATION('r', 6)),\
		FST::NODE(1, FST::RELATION('e', 7)),\
FST::NODE()

#define FST_RETURN 7, \
		FST::NODE(1, FST::RELATION('r', 1)),\
		FST::NODE(1, FST::RELATION('e', 2)),\
		FST::NODE(1, FST::RELATION('t', 3)),\
		FST::NODE(1, FST::RELATION('u', 4)),\
		FST::NODE(1, FST::RELATION('r', 5)),\
		FST::NODE(1, FST::RELATION('n', 6)),\
FST::NODE()


#define FST_PRINT 6, \
		FST::NODE(1, FST::RELATION('p', 1)),\
		FST::NODE(1, FST::RELATION('r', 2)),\
		FST::NODE(1, FST::RELATION('i', 3)),\
		FST::NODE(1, FST::RELATION('n', 4)),\
		FST::NODE(1, FST::RELATION('t', 5)),\
FST::NODE()


#define FST_MAIN 5, \
		FST::NODE(1, FST::RELATION('m', 1)),\
		FST::NODE(1, FST::RELATION('a', 2)),\
		FST::NODE(1, FST::RELATION('i', 3)),\
		FST::NODE(1, FST::RELATION('n', 4)),\
FST::NODE()

#define SNODE(Y) 2, FST::NODE(1, FST::RELATION(Y, 1)),\
FST::NODE()

#define FST_SEMICOLON SNODE(';')
#define FST_COMMA SNODE(',')
#define FST_BRACELET SNODE('}')

#define FST_LEFTBRACE SNODE('{')

#define FST_LEFTHESIS SNODE('(')

#define FST_RIGHTESIS SNODE(')')

#define FST_PLUS SNODE('+')
#define FST_MINUS SNODE('-')
#define FST_STAR SNODE('*')
#define FST_DIRSLACH SNODE('/')

#define INTEGER_NODE(X) FST::RELATION('0', X), FST::RELATION('1', X), FST::RELATION('2', X), FST::RELATION('3', X), FST::RELATION('4', X), \
FST::RELATION('5', X), FST::RELATION('6', X), FST::RELATION('7', X), FST::RELATION('8', X), FST::RELATION('9', X)

#define MINUS_NODE(X) FST::RELATION('-', X), FST::RELATION('0', X), FST::RELATION('1', X), FST::RELATION('2', X), FST::RELATION('3', X), FST::RELATION('4', X), \
FST::RELATION('5', X), FST::RELATION('6', X), FST::RELATION('7', X), FST::RELATION('8', X), FST::RELATION('9', X)

#define FST_INTEGER_LITERAL 1,	\
	FST::NODE(10,	FST::RELATION('0', 0),	\
					FST::RELATION('1', 0),	\
					FST::RELATION('2', 0),	\
					FST::RELATION('3', 0),	\
					FST::RELATION('4', 0),	\
					FST::RELATION('5', 0),	\
					FST::RELATION('6', 0),	\
					FST::RELATION('7', 0),	\
					FST::RELATION('8', 0),	\
					FST::RELATION('9', 0)),	\
	FST::NODE()
								

#define REL(X, Y) FST::RELATION(X, Y)

#define STRING_NODE(X) INTEGER_NODE(X),\
REL(' ', X),\
REL('à', X), REL('á', X), REL('â', X), REL('ã', X), REL('ä', X), REL('å', X), REL('¸', X), REL('æ', X),REL('ç', X), REL('è', X), REL('é', X),\
REL('ê', X), REL('ê', X), REL('ë', X),REL('ì', X),REL('í', X),REL('î', X), REL('ï', X),REL('ð', X),REL('ñ', X),REL('ò', X), REL('ó', X),\
REL('ô', X), REL('õ', X), REL('ö', X),REL('÷', X),REL('ø', X),REL('ù', X), REL('ú', X),REL('ü', X),REL('û', X),REL('ý', X), REL('þ', X), REL('ÿ', X)

//33+2+10 => 2: ' ' 45
#define FST_STRING_LITERAL 3,	\
	FST::NODE(1, FST::RELATION('\'', 1)),	\
	FST::NODE(85,	FST::RELATION(' ', 1),	\
					FST::RELATION('a', 1),	\
					FST::RELATION('b', 1),	\
					FST::RELATION('c', 1),	\
					FST::RELATION('d', 1),	\
					FST::RELATION('e', 1),	\
					FST::RELATION('f', 1),	\
					FST::RELATION('g', 1),	\
					FST::RELATION('h', 1),	\
					FST::RELATION('i', 1),	\
					FST::RELATION('j', 1),	\
					FST::RELATION('k', 1),	\
					FST::RELATION('l', 1),	\
					FST::RELATION('m', 1),	\
					FST::RELATION('n', 1),	\
					FST::RELATION('o', 1),	\
					FST::RELATION('p', 1),	\
					FST::RELATION('q', 1),	\
					FST::RELATION('r', 1),	\
					FST::RELATION('s', 1),	\
					FST::RELATION('t', 1),	\
					FST::RELATION('u', 1),	\
					FST::RELATION('v', 1),	\
					FST::RELATION('w', 1),	\
					FST::RELATION('x', 1),	\
					FST::RELATION('y', 1),	\
					FST::RELATION('z', 1),	\
					FST::RELATION('0', 1),	\
					FST::RELATION('1', 1),	\
					FST::RELATION('2', 1),	\
					FST::RELATION('3', 1),	\
					FST::RELATION('4', 1),	\
					FST::RELATION('5', 1),	\
					FST::RELATION('6', 1),	\
					FST::RELATION('7', 1),	\
					FST::RELATION('8', 1),	\
					FST::RELATION('9', 1),	\
					FST::RELATION('à', 1),	\
					FST::RELATION('á', 1),	\
					FST::RELATION('â', 1),	\
					FST::RELATION('ã', 1),	\
					FST::RELATION('ä', 1),	\
					FST::RELATION('å', 1),	\
					FST::RELATION('¸', 1),	\
					FST::RELATION('æ', 1),	\
					FST::RELATION('ç', 1),	\
					FST::RELATION('è', 1),	\
					FST::RELATION('é', 1),	\
					FST::RELATION('ê', 1),	\
					FST::RELATION('ë', 1),	\
					FST::RELATION('ì', 1),	\
					FST::RELATION('í', 1),	\
					FST::RELATION('î', 1),	\
					FST::RELATION('ï', 1),	\
					FST::RELATION('ð', 1),	\
					FST::RELATION('ñ', 1),	\
					FST::RELATION('ò', 1),	\
					FST::RELATION('ó', 1),	\
					FST::RELATION('ô', 1),	\
					FST::RELATION('õ', 1),	\
					FST::RELATION('ö', 1),	\
					FST::RELATION('÷', 1),	\
					FST::RELATION('ø', 1),	\
					FST::RELATION('ù', 1),	\
					FST::RELATION('ü', 1),	\
					FST::RELATION('ú', 1),	\
					FST::RELATION('û', 1),	\
					FST::RELATION('ý', 1),	\
					FST::RELATION('þ', 1),	\
					FST::RELATION('ÿ', 1),	\
					FST::RELATION('=', 1),	\
					FST::RELATION('+', 1),	\
					FST::RELATION('-', 1),	\
					FST::RELATION('*', 1),	\
					FST::RELATION('/', 1),	\
					FST::RELATION('(', 1),	\
					FST::RELATION(')', 1),	\
					FST::RELATION('{', 1),	\
					FST::RELATION('}', 1),	\
					FST::RELATION(';', 1),	\
					FST::RELATION(',', 1),	\
					FST::RELATION('?', 1),	\
					FST::RELATION('!', 1),	\
					FST::RELATION('\'', 2)),\
	FST::NODE()

#define FST_IDENTIFIER      1,	\
			FST::NODE(60,	FST::RELATION('a', 0),	\
							FST::RELATION('b', 0),	\
							FST::RELATION('c', 0),	\
							FST::RELATION('d', 0),	\
							FST::RELATION('e', 0),	\
							FST::RELATION('f', 0),	\
							FST::RELATION('g', 0),	\
							FST::RELATION('h', 0),	\
							FST::RELATION('i', 0),	\
							FST::RELATION('j', 0),	\
							FST::RELATION('k', 0),	\
							FST::RELATION('l', 0),	\
							FST::RELATION('m', 0),	\
							FST::RELATION('n', 0),	\
							FST::RELATION('o', 0),	\
							FST::RELATION('p', 0),	\
							FST::RELATION('q', 0),	\
							FST::RELATION('r', 0),	\
							FST::RELATION('s', 0),	\
							FST::RELATION('t', 0),	\
							FST::RELATION('u', 0),	\
							FST::RELATION('v', 0),	\
							FST::RELATION('w', 0),	\
							FST::RELATION('x', 0),	\
							FST::RELATION('y', 0),	\
							FST::RELATION('z', 0),	\
							FST::RELATION('_', 0)),	\
							FST::RELATION('à', 0),	\
							FST::RELATION('á', 0),	\
							FST::RELATION('â', 0),	\
							FST::RELATION('ã', 0),	\
							FST::RELATION('ä', 0),	\
							FST::RELATION('å', 0),	\
							FST::RELATION('¸', 0),	\
							FST::RELATION('æ', 0),	\
							FST::RELATION('ç', 0),	\
							FST::RELATION('è', 0),	\
							FST::RELATION('é', 0),	\
							FST::RELATION('ê', 0),	\
							FST::RELATION('ë', 0),	\
							FST::RELATION('ì', 0),	\
							FST::RELATION('í', 0),	\
							FST::RELATION('î', 0),	\
							FST::RELATION('ï', 0),	\
							FST::RELATION('ð', 0),	\
							FST::RELATION('ñ', 0),	\
							FST::RELATION('ò', 0),	\
							FST::RELATION('ó', 0),	\
							FST::RELATION('ô', 0),	\
							FST::RELATION('õ', 0),	\
							FST::RELATION('ö', 0),	\
							FST::RELATION('÷', 0),	\
							FST::RELATION('ø', 0),	\
							FST::RELATION('ù', 0),	\
							FST::RELATION('ü', 0),	\
							FST::RELATION('ú', 0),	\
							FST::RELATION('û', 0),	\
							FST::RELATION('ý', 0),	\
							FST::RELATION('þ', 0),	\
							FST::RELATION('ÿ', 0),	\
			FST::NODE()			

namespace FST
{
	struct RELATION
	{
		char symbol;
		short nnode;
		RELATION(char c = 0x00, short ns = NULL);
	};
	struct NODE
	{
		short n_relation;
		RELATION* realations;
		NODE();
		NODE(short n, RELATION rel, ...);
	};
	struct FST
	{
		char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;
		FST(const char* s, short ns, NODE n, ...);
	};

	bool execute(FST& fst);
}