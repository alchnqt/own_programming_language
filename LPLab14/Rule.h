#pragma once
#ifndef RULE_H_
#define RULE_H_
#pragma once
#include "Greibach.h"
#include "LT.h"
#define GRB_ERROR_SERIES 171
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(
		NS('S'), TS('$'),                     // стартовый символ, дно стека
		8,									  // количество правил
		Rule(NS('S'), GRB_ERROR_SERIES + 0,			//Неверная структура программы
			4,		//S >	m{NrE;};	|	tfi(F){NrE;};S	|	m{NrE;};S	|	tfi(F){NrE;}; 
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(13, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(12, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(8, TS('f'), TS('t'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('S'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1,			//Ошибочный оператор
			22,		//N	>	dti; | rE; | i = E; | dtfi(F); | dti;N | rE;N | i=E;N | dtfi(F);N | pE; | pE;N 
			Rule::Chain(1, TS('b'), TS(';')),
			Rule::Chain(1, TS('c'), TS(';')),
			Rule::Chain(3, TS('t'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(5, TS('?'), NS('P'), NS('N'), TS(LEX_ENDIF), NS('N')), //if endif ...code
			Rule::Chain(4, TS('?'), NS('P'), NS('N'), TS(LEX_ENDIF)), //if endif
			Rule::Chain(6, TS('?'), NS('P'), NS('N'), TS('e'), NS('N'), TS(LEX_ENDIF)), //if else endif
			Rule::Chain(7, TS('?'), NS('P'), NS('N'), TS('e'), NS('N'), TS(LEX_ENDIF), NS('N')), //if else endif ...code
			Rule::Chain(4, TS(LEX_REPEAT), NS('N'), TS(LEX_UNTIL), NS('P')), //repeat until bool_var
			Rule::Chain(4, TS(LEX_WHILE), NS('P'), NS('N'), TS(LEX_ENDW)),  //while bool_var endw
			Rule::Chain(5, TS(LEX_REPEAT), NS('N'), TS(LEX_UNTIL), NS('P'), NS('N')), //repeat until bool_var
			Rule::Chain(5, TS(LEX_WHILE), NS('P'), NS('N'), TS(LEX_ENDW), NS('N')),  //while bool_var endw
			Rule::Chain(5, TS('?'), NS('P'), NS('N'), NS('T'), TS(LEX_ENDIF)),  //if elif endif
			Rule::Chain(6, TS('?'), NS('P'), NS('N'), NS('T'), TS(LEX_ENDIF), NS('N')),  //if elif endif ..code
			Rule::Chain(7, TS('?'), NS('P'), NS('N'), NS('T'), TS('e'), NS('N'), TS(LEX_ENDIF)), //if elif else endif
			Rule::Chain(8, TS('?'), NS('P'), NS('N'),NS('T'), TS('e'), NS('N'), TS(LEX_ENDIF), NS('N')),//if elif else endif ..code
			Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2,			//Ошибка в выражении
			9,		//E >	i	|	l	|	(E)	|	i(W)	|	iM	|	lM	|	(E)M	|	i(W)M|	i()
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, NS('F')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 3,			//Ошибка в параметрах функции
			4,		//W	>	i	|	l	|	i,W	|	l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4,			//Ошибка в параметрах вызываемой функции
			2,		//F	>	ti	|	ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 5,			//Ошибочное выражение
			2,		//M>		vE	|	vEM
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),
		Rule(NS('P'), GRB_ERROR_SERIES + 6,			//Ошибка в логическом выражении
			6,		
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS('q'), TS('i')),
			Rule::Chain(3, TS('l'), TS('q'), TS('i')),
			Rule::Chain(3, TS('i'), TS('q'), TS('l')),
			Rule::Chain(3, TS('l'), TS('q'), TS('l'))
		),
		Rule(NS('T'), GRB_ERROR_SERIES + 7,			//Ошибка в условном операторе
			2,
			Rule::Chain(3, TS(LEX_ELIF), NS('P'), NS('N')), //elif
			Rule::Chain(4, TS(LEX_ELIF), NS('P'), NS('N'), NS('T'))  //elif elif ...elif 
		)
	);
}
#endif // !RULE_H_
