#include "PolishNotation.h"
#include <stack>
#include <queue>
#include "LT.h"
#include <iostream>

bool polish_notation::PolishNotation(int i, LexicalAnalyze& lex)
{
	std::stack<LT::Entry> stack;
	std::queue<LT::Entry> queue;
	LT::Entry temp;
	temp.idxTI = -1;
	temp.lexema = ' ';
	temp.sn = -1;
	LT::Entry func;
	func.lexema = '@';
	LT::Entry num;
	num.lexema = ' ';
	int countLex = 0;
	int posLex = i;
	bool findFunc = false;
	int buf = 0;

	for (i; lex.lt.table[i].lexema != LEX_SEMICOLON; i++, countLex++)
	{
		switch (lex.lt.table[i].lexema)
		{
		case LEX_ID:
		{
			if (findFunc)
				buf++;
			queue.push(lex.lt.table[i]);
			continue;
		}
		case LEX_LITERAL:
		{
			if (findFunc)
				buf++;
			queue.push(lex.lt.table[i]);
			continue;
		}
		case LEX_LEFTHESIS:
		{
			if (lex.id.table[lex.lt.table[i - 1].idxTI].idtype == IT::F)
			{
				findFunc = true;
			}
			stack.push(lex.lt.table[i]);
			continue;
		}
		case LEX_RIGHTHESIS:
		{
			if (findFunc)
			{
				num.lexema = (char)buf + '0';
				stack.push(num);
				stack.push(func);
				findFunc = false;
				buf = 0;
			}
			while (stack.top().lexema != LEX_LEFTHESIS)
			{
				queue.push(stack.top());
				stack.pop();
				if (stack.empty())
					return false;
			}
			stack.pop();
			continue;
		}
		case LEX_OPERATOR:
		{
			while (!stack.empty() && lex.lt.table[i].priority <= stack.top().priority)

			{
				queue.push(stack.top());
				stack.pop();
			}
			stack.push(lex.lt.table[i]);
			continue;
		}
		}
	}
	while (!stack.empty())
	{
		if (stack.top().lexema == LEX_LEFTHESIS || stack.top().lexema == LEX_RIGHTHESIS)
			return false;
		queue.push(stack.top());
		stack.pop();
	}
	while (lex.lt.table[posLex].lexema != ';')
	{
		if (!queue.empty()) {
			lex.lt.table[posLex++] = queue.front();
			queue.pop();
		}
		else
		{
			lex.lt.table[posLex++] = temp;
		}
		countLex--;
	}

	for (int i = 0; i < posLex; i++)
	{
		if (lex.lt.table[i].lexema == LEX_OPERATOR || lex.lt.table[i].lexema == LEX_LITERAL)
			lex.id.table[lex.lt.table[i].idxTI].idxfirstLE = i;
	}
	return true;
}

bool polish_notation::start(LexicalAnalyze& lex)
{
	bool rc = false;
	for (int i = 0; i < lex.lt.size; i++)
	{
		if (lex.lt.table[i].lexema == LEX_EQUAL)		// если нашли оператор присваивания
		{
			rc = PolishNotation(i + 1, lex);		// передаем номер следующей за ним лексемы
			if (!rc)
				return rc;
		}
	}
	return rc;
}

