#include "LexicalAnalyze.h"
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include "Parm.h"
#include "FST.h"
#include "In.h"
#include "Out.h"
#include "Error.h"
#include <vector>
#include <regex>
#include <sstream>
#pragma warning(disable:4996)
using Out::OUT;
using LT::LexTable;
using IT::IdTable;
using LTEntry = LT::Entry;
using lexical_analyze::LexicalAnalyze;
#define DIV '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='

std::string strconcat(const char* str1,const char* str2)
{
	std::string _str1(str1);
	std::string _str2(str2);
	std::string res = _str1 + _str2;
	return res;
}
std::string toString(int val)
{
	std::ostringstream oss;
	oss << val;
	return oss.str();
}
bool check_separate_flag(const int& i, In::IN in)
{
	for (size_t k = 0; k < Out::SEPARATE_SYMBOLS.length(); k++)
	{
		if (in.text[i] == OUT_CODE_SPACE && in.text[i + 1] == Out::SEPARATE_SYMBOLS[k] ||
			in.text[i] == OUT_CODE_SPACE && in.text[i - 1] == Out::SEPARATE_SYMBOLS[k] ||
			in.text[i] == Out::SEPARATE_SYMBOLS[k] || in.text[i + 1] == Out::SEPARATE_SYMBOLS[k])
		{
			return true;
			break;
		}
	}
	return false;
}
void erase_space(std::string& mystr)
{
	size_t index = 0;
	if(mystr[mystr.length() - 1] == ' ')
		while ((index = mystr.find(' ')) != std::string::npos)
			mystr.erase(index, 1);
}
bool get_string_literal(size_t& i, In::IN in, std::string& lexema, const int& line_num)
{
	bool valid = false;
	int str_counter = 0;
	if (in.text[i] == '\'')
	{
		while (i != 255)
		{
			lexema += in.text[i];
			i++;
			str_counter++;
			if (in.text[i] == '\'')
			{
				lexema += in.text[i];
				valid = true;
				break;
			}
			if (str_counter > 255 || str_counter >= in.size)
				throw Error::exception("Длина строкового литерала не должны превышать размера в 255 символов", 608, line_num);
		}
	}
	return valid;
}
bool is_operator(const std::string& lexema)
{
	return lexema == "+" || lexema == "-" || lexema == "*" || lexema == "/" || lexema == "%";
}
std::map<int, lexical_analyze::Lexema> to_string_array(In::IN in)
{
	using lexical_analyze::Lexema;
	using lexical_analyze::create_lexema;
	std::map<int, Lexema> lexemas;
	bool separate_flag = false;
	std::string lexema = "";
	int lines = 0;
	bool is_pushable = false;
	bool is_string_literal = false;
	int line_num = 0;
	int lexema_counter = 0;
	for (size_t i = 0; i < in.size; i++)
	{
		is_string_literal = get_string_literal(i, in, lexema, line_num);
		is_pushable = lexema != "\n" && lexema != "" && lexema != " " && lexema != "\'";
		if ((is_pushable && separate_flag) || is_string_literal)
		{
			lexema_counter++;
			lexemas.emplace(lexema_counter, create_lexema(lexema, line_num));
			lexema = "";
			separate_flag = false;
		}
		if (in.text[i] != OUT_CODE_ENDL || separate_flag)
		{
			if (lexema == " " || lexema == "\n" || lexema == "\'")
				lexema = "";
			lexema += in.text[i];
			erase_space(lexema);
			if (i == in.size - 1)
			{
				lexema_counter++;
				lexemas.emplace(lexema_counter, Lexema(lexema, line_num));
			}
		}
		if (in.text[i] == OUT_CODE_ENDL)
		{
			line_num++;
		}
		separate_flag = check_separate_flag(i, in);		
	}
	return lexemas;
}
bool is_bool_operator(const std::string& lexema)
{
	return lexema == "equals" || lexema == "more_than" 
		|| lexema == "less_than" || lexema == "not_equals"
		|| lexema == "less_equals" || lexema == "more_equals";
}
bool is_std_func(const std::string& func_name)
{
	std::vector<std::string> std_funcs =
	{
		"std_in_int",
		"strtoint",
		"strlength"
	};
	for (auto& n : std_funcs)
	{
		if (n == func_name)
			return true;
	}
	return false;
}

namespace lexical_analyze
{
	Lexema create_lexema(const std::string& str, const int& l)
	{
		Lexema tmp;
		std::stringstream ss;
		ss << str;
		tmp.line = l;
		ss >> tmp.value; // = str;
		return { str, l };
	}
	void Delete(LexicalAnalyze& LA)
	{
		LT::Delete(LA.lt);
		IT::Delete(LA.id);
	}
	LexicalAnalyze Start(In::IN& in)
	{
		LexicalAnalyze LA;
		LT::LexTable lt = LT::Create(LT_MAXSIZE);
		IT::IdTable it = IT::Create(TI_MAXSIZE);
		bool separate_flag = false;
		int lines_counter = 2;
		int indxLex = 0;
		int indexID = 0;
		int clit = 1;
		int line = 1;
		int position = 0;
		int bufPos = 0;

		bool findMain = false;
		int countMain = 0;
		bool findDec = false;
		bool findType = false;

		char emptystr[] = "";
		std::string prefix = "";
		char* L = new  char[9]{ "literal_" };
		char* bufL = new  char[TI_STR_MAXSIZE];
		char* nameLiteral = new  char[10]{ "" };
		char* charclit = new char[10]{ "" };

		bool findFunc = false;
		bool findParm = false;
		bool got_std_func = false;
		IT::Entry entryIT;
		std::map<int, lexical_analyze::Lexema> lexemas = to_string_array(in);
			using std::regex;

			regex string_literal("\'([^]|.)*\'", std::regex_constants::ECMAScript);
			regex identifier("^[A-Za-z][0-9A-Za-z]*(_[0-9A-Za-z]+)*$", std::regex_constants::ECMAScript);
			regex int_literal("^-?([1-9]*[0-9A-Za-z]*|0[xX][0-9a-fA-F]+|0[bB][0-1]+|0[oO][0-7]+)$", std::regex_constants::ECMAScript);
			std::map<int, lexical_analyze::Lexema>::iterator lexema;


			for (size_t i = 1; i <= lexemas.size(); i++, indxLex++)
			{
				bool findSameID = false;
				position = 0;
				lexema = lexemas.find(i);
				if (lexema->second.value == "int")
				{
					LT::Add(lt, { LEX_INTEGER, LT_TI_NULLIDX, lexema->second.line });
					findType = true;
					if (!findFunc)
						findDec = true;
					entryIT.iddatatype = IT::IDDATATYPE::INT;
					continue;
				}
				if (lexema->second.value == "boolean")
				{
					LT::Add(lt, { LEX_INTEGER, LT_TI_NULLIDX, lexema->second.line });
					findType = true;
					if(!findFunc)
						findDec = true;
					entryIT.iddatatype = IT::IDDATATYPE::BOOL;
					continue;
				}
				if (lexema->second.value == "string")
				{
					LT::Add(lt, { LEX_STRING, LT_TI_NULLIDX, lexema->second.line });
					findType = true;
					if (!findFunc)
						findDec = true;
					entryIT.iddatatype = IT::IDDATATYPE::STR;
					entryIT.value.vstr.str = emptystr;
					continue;
				}
				if (lexema->second.value == "def" || lexema->second.value == "spot")
				{
					if (lexema->second.value == "spot")
						got_std_func = true;
					else
						got_std_func = false;

					if(is_std_func(lexemas.find(i + 2)->second.value) && !got_std_func)
						throw Error::exception("Объявление функции стандартной библиотеки должно осуществляться с помощью ключевого слово spot", 610, lexema->second.line);

					LT::Add(lt, { LEX_FUNCTION, LT_TI_NULLIDX, lexema->second.line });
					entryIT.idtype = IT::IDTYPE::F;
					findFunc = true;
					prefix = "";
					continue;
				}
				if (lexema->second.value == "return")
				{
					LT::Add(lt, { LEX_RETURN, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "s_out")
				{
					LT::Add(lt, { LEX_PRINT, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "main")
				{
					LT::Add(lt, { LEX_MAIN, LT_TI_NULLIDX, lexema->second.line });
					prefix = lexema->second.value;
					entryIT.visibility = emptystr;
					findMain = true;
					got_std_func = false;
					countMain++;
					if (countMain > 1)
						throw Error::exception("Точка входа main может быть только в единственном экземпляре", 602);
					continue;
				}
				if (lexema->second.value == "if")
				{
					LT::Add(lt, { '?', LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "elif")
				{
					LT::Add(lt, { LEX_ELIF, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "endif")
				{
					LT::Add(lt, { LEX_ENDIF, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "repeat")
				{
					LT::Add(lt, { LEX_REPEAT, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "until")
				{
					LT::Add(lt, { LEX_UNTIL, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "while")
				{
					LT::Add(lt, { LEX_WHILE, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "endw")
				{
					LT::Add(lt, { LEX_ENDW, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "else")
				{
					LT::Add(lt, { 'e' , LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == ",")
				{
					LT::Entry entryLT = { LEX_COMMA, LT_TI_NULLIDX, lexema->second.line };
					entryLT.priority = 1;
					LT::Add(lt, entryLT);
					continue;
				}
				if (is_bool_operator(lexema->second.value))
				{
					LT::Entry entryLT = { LEX_BOOL_OPERATOR, indexID++, lexema->second.line };
					entryLT.priority = 2;
					LT::Add(lt, entryLT);
					entryIT.id = lexema->second.value;
					entryIT.idxfirstLE = indxLex;
					entryIT.idtype = IT::IDTYPE::O;
					IT::Add(it, entryIT);
					continue;
				}
				
				if (lexema->second.value == "true" || lexema->second.value == "false")
				{
					LT::Add(lt, { LEX_LITERAL, indexID++, lexema->second.line });
					entryIT.value.vbool = lexema->second.value == "true" ? true : false;
					entryIT.idtype = IT::IDTYPE::L;
					entryIT.iddatatype = IT::IDDATATYPE::BOOL;
					entryIT.idxfirstLE = indxLex;
					_itoa_s(clit++, charclit, sizeof(char) * 10, 10);
					strcpy(bufL, L);
					nameLiteral = strcat(bufL, (char*)charclit);
					entryIT.id = nameLiteral;
					IT::Add(it, entryIT);
					continue;
				}
				if (is_operator(lexema->second.value))
				{
					LT::Entry entryLT = { LEX_OPERATOR, indexID++, lexema->second.line };
					switch (lexema->second.value[0])
					{
					case PLUS: case MINUS:
						entryLT.priority = 2;
						break;
					case DIRSLASH: case STAR: case '%':
						entryLT.priority = 3;
						break;
					}
					LT::Add(lt, entryLT);
					entryIT.id = lexema->second.value;
					entryIT.idxfirstLE = indxLex;
					entryIT.idtype = IT::IDTYPE::O;
					IT::Add(it, entryIT);
					continue;
				}
				if (std::regex_search(lexema->second.value, identifier))
				{
					int idx = TI_NULLIDX;
					auto get_func = lexemas.find(i + 1);
					if (prefix != "" && get_func->second.value == "(")
					{
						idx = IT::IsId(it, lexema->second.value);
						LT::Add(lt, { LEX_ID, idx, lexema->second.line });
						continue;
					}

					if(prefix != lexema->second.value)
						lexema->second.value = prefix + lexema->second.value;

					
					if (findFunc)
					{
						prefix = lexema->second.value;
						bufPos = it.size;
						idx = IT::IsId(it, lexema->second.value);
						if (idx != TI_NULLIDX)
						{
							LT::Add(lt, { LEX_ID, idx, lexema->second.line });
							findFunc = false;
							continue;
						}
					}
					idx = IT::IsId(it, lexema->second.value);
					if (idx == TI_NULLIDX && !findDec && !findType)
						throw Error::exception("Переменная должна иметь определенный тип данных", 611, lexema->second.line);
					if (idx != TI_NULLIDX)
					{
						if (findDec == true)
							throw Error::exception("Попытка переопределения идентификатора", 600, lexema->second.line);
						LT::Add(lt, { LEX_ID, idx, lexema->second.line });
						findFunc = false;
						continue;
					}
					LT::Add(lt, { LEX_ID, indexID++, lexema->second.line });
					findDec = false;
					if (findParm) 
					{
						entryIT.idtype = IT::IDTYPE::P;
						it.table[bufPos].countOfPar++;
						entryIT.visibility = prefix;
					}
					else if (!findFunc) {
						entryIT.idtype = IT::IDTYPE::V;
						entryIT.visibility = prefix;
						if (entryIT.iddatatype == IT::IDDATATYPE::INT)
							entryIT.value.vint = TI_INT_DEFAULT;
						if (entryIT.iddatatype == IT::IDDATATYPE::STR) {
							entryIT.value.vstr.len = 0;
							entryIT.value.vstr.str = "";
						}
					}
					else 
					{
						if (prefix != lexema->second.value)
							lexema->second.value= prefix + lexema->second.value;
						entryIT.visibility = emptystr;
					}

					entryIT.idxfirstLE = indxLex;
					entryIT.id = lexema->second.value;
					IT::Add(it, entryIT);
					findFunc = false;
					findType = false;
					continue;
				}
				if (std::regex_search(lexema->second.value, int_literal))
				{
					int value = 0;
					char* end;
					if (lexema->second.value.find('x') != std::string::npos || lexema->second.value.find('X') != std::string::npos)
					{
						lexema->second.value.erase(lexema->second.value.begin(), lexema->second.value.begin() + 2);
						value = strtol((char*)lexema->second.value.c_str(), &end, 16);
					}
					else if (lexema->second.value.find('b') != std::string::npos || lexema->second.value.find('B') != std::string::npos)
					{
						lexema->second.value.erase(lexema->second.value.begin(), lexema->second.value.begin() + 2);
						value = strtol((char*)lexema->second.value.c_str(), &end, 2);
					}
					else if (lexema->second.value.find('o') != std::string::npos || lexema->second.value.find('O') != std::string::npos)
					{
						lexema->second.value.erase(lexema->second.value.begin() + 1, lexema->second.value.begin() + 2);
						value = strtol((char*)lexema->second.value.c_str(), &end, 8);
					}
					else
						value = atoi((char*)lexema->second.value.c_str());

					end = nullptr;

					for (int k = 0; k < it.size; k++)
					{
						if (it.table[k].value.vint == value && it.table[k].idtype == IT::IDTYPE::L)
						{
							LT::Add(lt, { LEX_LITERAL, k, lexema->second.line });
							findSameID = true;
							break;
						}
					}
					if (findSameID)
					{
						continue;
					}
					LT::Add(lt, { LEX_LITERAL, indexID++, lexema->second.line });
					entryIT.idtype = IT::IDTYPE::L;
					entryIT.iddatatype = IT::IDDATATYPE::INT;
					entryIT.value.vint = value;
					entryIT.idxfirstLE = indxLex;
					_itoa_s(clit++, charclit, sizeof(char) * 10, 10);
					strcpy(bufL, L);
					lexema->second.value = strcat(bufL, (char*)charclit);
					entryIT.id = lexema->second.value;
					IT::Add(it, entryIT);
					continue;
				}
				if (std::regex_search(lexema->second.value, string_literal))
				{
					int length = lexema->second.value.length();
					lexema->second.value.erase(lexema->second.value.begin());
					lexema->second.value.erase(lexema->second.value.end() - 1);
					for (int k = 0; k < it.size; k++)
					{
						if (it.table[k].value.vstr.str == lexema->second.value)
						{
							findSameID = true;
							LT::Add(lt, { LEX_LITERAL, k, lexema->second.line });
							break;
						}
					}
					if (findSameID)
					{
						continue;
					}
					LT::Add(lt, { LEX_LITERAL, indexID++, lexema->second.line });
					entryIT.value.vstr.str = lexema->second.value;
					entryIT.value.vstr.len = length - 2;
					entryIT.idtype = IT::IDTYPE::L;
					entryIT.iddatatype = IT::IDDATATYPE::STR;
					entryIT.idxfirstLE = indxLex;
					_itoa_s(clit++, charclit, sizeof(char) * 10, 10);
					strcpy(bufL, L);
					nameLiteral = strcat(bufL, (char*)charclit);
					entryIT.id = nameLiteral;
					IT::Add(it, entryIT);
					continue;
				}
				
				if (lexema->second.value == ";")
				{
					LT::Add(lt, { LEX_SEMICOLON, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "{")
				{
					if (got_std_func)
						throw Error::exception("Функции стандартной библиотеки не могут иметь определения", 609, lexema->second.line);

					LT::Add(lt, { LEX_LEFTBRACE, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "}")
				{
					LT::Add(lt, { LEX_BRACELET, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				if (lexema->second.value == "(")
				{
					LT::Entry entryLT = { LEX_LEFTHESIS, LT_TI_NULLIDX, lexema->second.line };
					entryLT.priority = 0;
					LT::Add(lt, entryLT);
					if (it.table[indexID - 1].idtype == IT::IDTYPE::F)
						findParm = true;
					continue;
				}
				if (lexema->second.value == ")")
				{
					LT::Entry entryLT = { LEX_RIGHTHESIS, LT_TI_NULLIDX, lexema->second.line };
					entryLT.priority = 0;
					/*if (findParm && lexema[i + 1][0] != LEX_LEFTBRACE && lexema[i + 2][0] != LEX_LEFTBRACE)
						strcpy(prefix, oldprefix);*/
					findParm = false;
					LT::Add(lt, entryLT);
					continue;
				}
				if (lexema->second.value == "=")
				{
					LT::Add(lt, { LEX_EQUAL, LT_TI_NULLIDX, lexema->second.line });
					continue;
				}
				position += strlen(lexema->second.value.c_str());
				lexema->second.line++;
				indxLex--;
			throw ERROR_THROW_IN(108, lexema->second.line, position);
		}
		if (!findMain) throw Error::exception("Отсутствует точка входа main", 601);
		delete[] L;
		delete[] bufL;
		delete[] charclit;
		LA.id = it;
		LA.lt = lt;
		return LA;
	}
	Lexema::Lexema()
	{

	}
	Lexema::Lexema(std::string str, const int& line)
	{
		this->value = str;
		this->line = line;
	}
	Lexema::~Lexema()
	{
	}
}
	



