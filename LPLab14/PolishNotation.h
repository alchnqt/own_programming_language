#pragma once
#ifndef POLISH_NOTATION_H_
#define POLISH_NOTATION_H_
#include "LexicalAnalyze.h"
namespace polish_notation
{
	using lexical_analyze::LexicalAnalyze;
	bool PolishNotation(int i, LexicalAnalyze& lex);
	bool start(LexicalAnalyze& lex);
}
#endif // !POLISH_NOTATION_H_


