#pragma once
#ifndef LEXICAL_ANALYZE_H_
#define LEXICAL_ANALYZE_H_
#include "LT.h"
#include "IT.h"
#include "Parm.h"
#include "In.h"
#include <map>
namespace lexical_analyze
{
	struct LexicalAnalyze
	{
		LT::LexTable lt;
		IT::IdTable id;
	};
	struct Lexema
	{
		std::string value;
		int line;
		Lexema();
		Lexema(std::string str,const int& line);
		~Lexema();
	};
	Lexema create_lexema(const std::string& str, const int& l);
	void Delete(LexicalAnalyze& LA);
	LexicalAnalyze Start(In::IN& in);
}

#endif // LEXICAL_ANALYZE_H_



