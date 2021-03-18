#pragma once

#define OUT_CODE_ENDL '\n'
#define OUT_CODE_SEP '|'
#define OUT_CODE_SPACE ' '

#include <fstream>
#include "In.h"
#include "Parm.h"

namespace Out
{
	static const std::string SEPARATE_SYMBOLS = ";,}{()=+-/*\n ";
	struct OUT
	{
		std::string text;
		std::ofstream* stream = NULL;
		int lines;
	};
	void WriteOut(In::IN& in, OUT& out);
	void Close(OUT& out);
	OUT GetOut(Parm::PARM parm, In::IN in);
}
