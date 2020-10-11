#include "Out.h"
#include "Error.h"
#include <iostream>
#include "FST.h"
#include "In.h"
#include "LT.h"
#include "IT.h"
#pragma warning(disable:4996)
using Out::OUT;
using LT::LexTable;
using IT::IdTable;
using LTEntry = LT::Entry;

std::string toString(int val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}
void char_memset(char id[ID_MAXSIZE], const char* str)
{
	for (size_t i = 0; i < 5; i++)
	{
		id[0] = str[0];
	}
}

void checkFST(const char* lexema, LexTable& lt, IdTable& it,const int& lines_counter)
{
	FST::FST fst_main = FST::FST(lexema, FST_MAIN);
	FST::FST fst_integer = FST::FST(lexema, FST_INTEGER);
	FST::FST fst_string = FST::FST(lexema, FST_STRING);
	FST::FST fst_function = FST::FST(lexema, FST_FUNCTION);
	FST::FST fst_declare = FST::FST(lexema, FST_DECLARE);
	FST::FST fst_return = FST::FST(lexema, FST_RETURN);
	FST::FST fst_print = FST::FST(lexema, FST_PRINT);
	FST::FST fst_semicolon = FST::FST(lexema, FST_SEMICOLON);
	FST::FST fst_comma = FST::FST(lexema, FST_COMMA);
	FST::FST fst_bracelet = FST::FST(lexema, FST_BRACELET);
	FST::FST fst_leftbrace = FST::FST(lexema, FST_LEFTBRACE);
	FST::FST fst_lefthesis = FST::FST(lexema, FST_LEFTHESIS);
	FST::FST fst_rightesis = FST::FST(lexema, FST_RIGHTESIS);
	FST::FST fst_plus = FST::FST(lexema, FST_PLUS);
	FST::FST fst_minus = FST::FST(lexema, FST_MINUS);
	FST::FST fst_star = FST::FST(lexema, FST_STAR);
	FST::FST fst_dirslach = FST::FST(lexema, FST_DIRSLACH);
	FST::FST fst_integer_literal = FST::FST(lexema, FST_INTEGER_LITERAL);
	FST::FST fst_string_literal = FST::FST(lexema, FST_STRING_LITERAL);
	FST::FST fst_identifier = FST::FST(lexema, FST_IDENTIFIER);

	LT::Entry eLT;
	IT::Entry eIT;

	if (FST::execute(fst_main))
	{
		std::cout << "m"; 
		eLT.lexema[0] = LEX_ID;
		eLT.idxTI = IT::IDTYPE::F;
		eLT.sn = lines_counter;
		eIT.idxfirstLE = eLT.sn;
		char_memset(eIT.id, lexema);
		return;
	}


	if (FST::execute(fst_integer))
	{
		std::cout << "i";
		return;
	}

	if (FST::execute(fst_string))
	{
		std::cout << "s";
		return;
	}
		

	if (FST::execute(fst_function))
	{
		std::cout << "f";  return;
	}
		

	if (FST::execute(fst_declare))
	{
		std::cout << "d";  return;
	}
		

	if (FST::execute(fst_return))
	{
		std::cout << "r"; return;
	}
		

	if (FST::execute(fst_print))
	{
		std::cout << "p";  return;
	}
		

	if (FST::execute(fst_semicolon))
	{
		std::cout << ";";  return;
	}
		

	if (FST::execute(fst_comma))
	{
		std::cout << ",";  return;
	}
		

	if (FST::execute(fst_bracelet))
	{
		std::cout << "}";  return;
	}
		

	if (FST::execute(fst_leftbrace))
	{
		std::cout << "{";  return;
	}
		

	if (FST::execute(fst_lefthesis))
	{
		std::cout << "(";  return;
	}
		

	if (FST::execute(fst_rightesis))
	{
		std::cout << ")";  return;
	}
		

	if (FST::execute(fst_plus))
	{
		std::cout << "+";  return;
	}
		

	if (FST::execute(fst_minus))
	{
		std::cout << "-";  return;
	}
		

	if (FST::execute(fst_star))
	{
		std::cout << "*";  return;
	}
		

	if (FST::execute(fst_dirslach))
	{
		std::cout << "/";  return;
	}
		

	if (FST::execute(fst_integer_literal))
	{
		std::cout << "L";  return;
	}
		

	if (FST::execute(fst_string_literal))
	{
		std::cout << "L";  return;
	}
		

	if (FST::execute(fst_identifier))
	{
		std::cout << "I";  return;
	}
		

	if (lexema == "=")
	{
		std::cout << "=";  return;
	}
		
}

void del_waste(In::IN& in, Out::OUT& out)
{
	LT::LexTable lt = LT::Create(LT_MAXSIZE);
	IT::IdTable it = it.Create(TI_MAXSIZE);
	std::string text_out = "1 ";
	bool separate_flag = false;
	int lines_counter = 2;
	std::string lexema = "";
	char _new_line_target = '\0';
	for (size_t i = 0; i < in.size; i++)
	{
		if (in.text[i] == '\'')
		{
			while (i != in.size + 255)
			{
				text_out += in.text[i];
				i++;
				if (in.text[i] == '\'')
					break;
			}
		}

		if (in.text[i] == OUT_CODE_SPACE || separate_flag || in.text[i] == OUT_CODE_ENDL)
		{
			checkFST(lexema.c_str(), lt, it, lines_counter);
			lexema = "";
			separate_flag = false;
		}
		if (in.text[i] != OUT_CODE_ENDL || separate_flag)
		{
			if (lexema == " ")
				lexema = "";
			lexema += in.text[i];
		}
		if (in.text[i] == OUT_CODE_ENDL)
		{
			std::cout << std::endl;
		}

		text_out += in.text[i];
		if (in.text[i] == OUT_CODE_ENDL)
		{
			text_out += toString(lines_counter) + ' ';
			lines_counter++;
			continue;
		}
		
		for (size_t k = 0; k < Out::SEPARATE_SYMBOLS.length(); k++)
		{
			if (in.text[i] == OUT_CODE_SPACE && in.text[i + 1] == Out::SEPARATE_SYMBOLS[k] ||
				in.text[i] == OUT_CODE_SPACE && in.text[i - 1] == Out::SEPARATE_SYMBOLS[k] ||
				in.text[i] == Out::SEPARATE_SYMBOLS[k] || in.text[i + 1] == Out::SEPARATE_SYMBOLS[k])
			{
				separate_flag = true;
				break;
			}
		}
	}
	out.text = text_out;
	out.lines = in.lines;
	LT::Delete(lt);
	it.Delete(it);
}

void CreateTables(Out::OUT& out)
{
	LexTable lt = LT::Create(out.lines);
	FST::FST ft = FST::FST("ppffe", FST_INTEGER_LITERAL);;
	for (size_t i = 0; i < out.lines; i++)
	{

	}
	LT::Delete(lt);
}



void Out::WriteOut(In::IN& in, OUT& out)
{
	del_waste(in, out);
	* out.stream << out.text;
}
void Out::Close(OUT& out)
{
	out.stream->close();
	delete out.stream;
}
OUT Out::GetOut(Parm::PARM parm, In::IN in)
{
	OUT out;
	out.stream = new std::ofstream(parm.out, std::ios::out);
	if (!out.stream->is_open())
		throw ERROR_THROW(113);
	std::string fst_str;
	int size = 0;
	out.text = in.text;
	return out;
}




