#include "AsmTranslation.h"
#include <string>
#include <fstream>
#include <Stack>
#include <vector>
using std::endl;
std::ofstream asm_file;
bool check_lits(std::vector<std::string> lits, std::string lit)
{
	for (auto& n : lits)
	{
		if (n == lit)
			return false;
	}
	return true;
}
void AsmTranslation::create_system_info()
{
	asm_file << ASM_START_BLOCK;
	asm_file << ASM_LIBS_BLOCK;
	asm_file << endl << "includelib \"MAVSTD.lib\"" << endl;
}
void AsmTranslation::create_func_info()
{
	asm_file << "ExitProcess PROTO :DWORD\n";
	asm_file << "std_out_str PROTO: DWORD\n";
	asm_file << "std_out_int PROTO: SDWORD\n";
	/*
	asm_file << "std_in_int PROTO: SDWORD\n";
	asm_file << "strlength PROTO: DWORD\n";
	asm_file << "strtoint PROTO: DWORD\n";
	*/
	asm_file << ASM_STACK_BLOCK;
}
void AsmTranslation::create_const_info(const LexicalAnalyze& table)
{
	asm_file << ASM_CONST_BLOCK;
	std::string lit = "";
	std::vector<std::string> lits;
	for (int i = 0; i < table.lt.size; i++)
	{
		if (table.lt.table[i].lexema == LEX_LITERAL && table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR 
			&& check_lits(lits, table.id.table[table.lt.table[i].idxTI].id))
		{
			lits.push_back(table.id.table[table.lt.table[i].idxTI].id);
			asm_file << "\t" << table.id.table[table.lt.table[i].idxTI].id << " DB \"" << table.id.table[table.lt.table[i].idxTI].value.vstr.str << "\" , 0" << endl;
		}
		
		if (table.lt.table[i].lexema == LEX_LITERAL && table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT 
			&& check_lits(lits, table.id.table[table.lt.table[i].idxTI].id))
		{
			lits.push_back(table.id.table[table.lt.table[i].idxTI].id);
			asm_file << "\t" << table.id.table[table.lt.table[i].idxTI].id << " SDWORD " << table.id.table[table.lt.table[i].idxTI].value.vint << endl;
		}
		
		if (table.lt.table[i].lexema == LEX_LITERAL && table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL
			&& check_lits(lits, table.id.table[table.lt.table[i].idxTI].id))
		{
			lits.push_back(table.id.table[table.lt.table[i].idxTI].id);
			asm_file << "\t" << table.id.table[table.lt.table[i].idxTI].id << " DWORD " << table.id.table[table.lt.table[i].idxTI].value.vbool << endl;
		}
	}
}

void AsmTranslation::create_data_info(const LexicalAnalyze& table)
{
	asm_file << ASM_DATA_BLOCK;
	for (int i = 0; i < table.lt.size; i++)
	{
		if (table.lt.table[i].lexema == LEX_TYPE 
			&& table.lt.table[i + 1].lexema == LEX_ID 
			&& table.id.table[table.lt.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR
			&& table.id.table[table.lt.table[i + 1].idxTI].idtype != IT::IDTYPE::F
			&& table.id.table[table.lt.table[i + 1].idxTI].idtype != IT::IDTYPE::P)
			asm_file << "\t" << table.id.table[table.lt.table[i + 1].idxTI].id << " DWORD " << "?" << " , 0" << endl;

		if (table.lt.table[i].lexema == LEX_TYPE 
			&& table.lt.table[i + 1].lexema == LEX_ID 
			&& table.id.table[table.lt.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::INT
			&& table.id.table[table.lt.table[i + 1].idxTI].idtype != IT::IDTYPE::F
			&& table.id.table[table.lt.table[i + 1].idxTI].idtype != IT::IDTYPE::P)
			asm_file << "\t" << table.id.table[table.lt.table[i + 1].idxTI].id << " SDWORD " << table.id.table[table.lt.table[i + 1].idxTI].value.vint << endl;

		if (table.lt.table[i].lexema == LEX_TYPE 
			&& table.lt.table[i + 1].lexema == LEX_ID 
			&& table.id.table[table.lt.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::BOOL
			&& table.id.table[table.lt.table[i + 1].idxTI].idtype != IT::IDTYPE::F
			&& table.id.table[table.lt.table[i + 1].idxTI].idtype != IT::IDTYPE::P)
			asm_file << "\t" << table.id.table[table.lt.table[i + 1].idxTI].id << " DWORD " << table.id.table[table.lt.table[i + 1].idxTI].value.vbool << endl;
	}
}

bool is_from_std(const std::string& func_name)
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
void create_instructions(const lexical_analyze::LexicalAnalyze& table)
{
	using std::string;
	using std::endl;
	asm_file << "\n.code\n\n";
	string lit = "";
	string id = "";
	string func_name = "";
	std::string while_savestate = "";
	std::stringstream ss_while;
	bool get_func = false;
	bool get_entry = false;
	bool get_if = false;
	bool get_else = false;
	bool get_std_func = false;
	int if_iter = 0;
	std::stack<string> stk;
	for (size_t i = 0; i < table.lt.size; i++)
	{
		switch (table.lt.table[i].lexema)
		{

		case LEX_FUNCTION:
		{
			if (is_from_std(table.id.table[table.lt.table[++(++i)].idxTI].id))
			{
				get_std_func = true;
				get_func = false;
				asm_file << table.id.table[table.lt.table[i].idxTI].id << " PROTO";
			}
			else
			{
				get_std_func = false;
				get_func = true;
				asm_file << (func_name = table.id.table[table.lt.table[i].idxTI].id) << " PROC ";
			}
			while (table.lt.table[i].lexema != LEX_RIGHTHESIS)
			{
				if (table.id.table[table.lt.table[i].idxTI].idtype == IT::IDTYPE::P)
				{
					if (get_std_func)
						asm_file << " : ";
					else
						asm_file << table.id.table[table.lt.table[i].idxTI].id << " : ";
					if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT)
					{
						asm_file << "SDWORD";
					}
					else if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR)
					{
						asm_file << "DWORD";
					}
					else if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL)
					{
						asm_file << "DWORD";
					}
				}
				if (table.lt.table[i].lexema == LEX_COMMA)
				{
					asm_file << ", ";
				}
				i++;
			}

			asm_file << endl;
			break;
		}

		case LEX_MAIN:
		{
			get_entry = true;
			asm_file << "main PROC\n";
			break;
		}

		case LEX_EQUAL:
		{
			id = table.id.table[table.lt.table[i - 1].idxTI].id;
			int result_position = i - 1;
			while (table.lt.table[i].lexema != LEX_SEMICOLON)
			{
				switch (table.lt.table[i].lexema)
				{
				case LEX_ID: case LEX_LITERAL:
				{
					if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT
						&& table.id.table[table.lt.table[i].idxTI].idtype != IT::IDTYPE::F)
					{
						asm_file << "\tpush " << table.id.table[table.lt.table[i].idxTI].id << endl;
						stk.push(table.id.table[table.lt.table[i].idxTI].id);
					}
					if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR
						&& table.id.table[table.lt.table[i].idxTI].idtype == IT::IDTYPE::L)
					{
						asm_file << "\tpush offset " << table.id.table[table.lt.table[i].idxTI].id << endl;
						stk.push("offset " + table.id.table[table.lt.table[i].idxTI].id);
					}
					if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR
						&& table.id.table[table.lt.table[i].idxTI].idtype == IT::IDTYPE::V)
					{
						asm_file << "\tpush dword ptr " << table.id.table[table.lt.table[i].idxTI].id << endl;
						stk.push("dword ptr " + table.id.table[table.lt.table[i].idxTI].id);
					}
					if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL
						&& table.id.table[table.lt.table[i].idxTI].idtype != IT::IDTYPE::F)
					{
						asm_file << "\tpush " << table.id.table[table.lt.table[i].idxTI].id << endl;
						stk.push(table.id.table[table.lt.table[i].idxTI].id);
					}
					break;
				}
				case '@':
				{
					/*for (int j = 1; j <= std::stoi(table.id.table[table.lt.table[i + 1].idxTI].id); j++)
					{
						asm_file << "\tpop edx\n";
					}
					for (int j = 1; j <= table.id.table[table.lt.table[i].idxTI].countOfPar; j++)
					{
						asm_file << "\tpush " << stk.top() << endl;
						stk.pop();
					}*/
					std::string num{ table.lt.table[i + 1].lexema };
					asm_file << "\tcall " << table.id.table[table.lt.table[i - std::stoi(num) - 1].idxTI].id;
					asm_file << "\n\tpush eax\n";
					break;
				}
				case LEX_OPERATOR:
				{
					switch (table.id.table[table.lt.table[i].idxTI].id[0])
					{
					case '*':
					{
						asm_file << "\tpop eax\n\tpop ebx\n";
						asm_file << "\tmul ebx\n\tpush eax\n";
						break;
					}
					case '+':
					{
						asm_file << "\tpop eax\n\tpop ebx\n";
						asm_file << "\tadd eax, ebx\n\tpush eax\n";
						break;
					}
					case '-':
					{
						asm_file << "\tpop ebx\n\tpop eax\n";
						asm_file << "\tsub eax, ebx\n\tpush eax\n";
						break;
					}
					case '/':
					{
						asm_file << "\tpop ebx" << endl;
						asm_file << "\tmov edx, 0" << endl;
						asm_file << "\tpop eax" << endl;
						asm_file << "\tdiv ebx" << endl;
						asm_file << "\tpush eax" << endl;
						break;
					}
					case '%':
					{
						asm_file << "\tpop ebx" << endl;
						asm_file << "\tmov edx, 0" << endl;
						asm_file << "\tpop eax" << endl;
						asm_file << "\tdiv ebx" << endl;
						asm_file << "\tpush edx" << endl;
						break;
					}
					}
					break;
				}
				}
				i++;
			}
			asm_file << "\tpop " << table.id.table[table.lt.table[result_position].idxTI].id << endl;
			id.clear();
			break;
		}

		case LEX_RETURN:
		{
			i++;
			if (table.id.table[table.lt.table[i].idxTI].idtype == IT::IDDATATYPE::INT)
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i].idxTI].id << endl;
			}
			else if (table.id.table[table.lt.table[i].idxTI].idtype == IT::IDDATATYPE::STR && table.lt.table[i].lexema == LEX_LITERAL)
			{
				asm_file << "\tpush offset " << table.id.table[table.lt.table[i].idxTI].id << endl;
			}
			else if (table.id.table[table.lt.table[i].idxTI].idtype == IT::IDDATATYPE::STR && table.lt.table[i].lexema == LEX_ID)
			{
				asm_file << "\tpush dword ptr " << table.id.table[table.lt.table[i].idxTI].id << endl;
			}
			else if (table.id.table[table.lt.table[i].idxTI].idtype == IT::IDDATATYPE::BOOL)
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i].idxTI].id << endl;
			}
			asm_file << "\tpop eax\n";
			asm_file << "\tret\n";
			break;
		}

		case '}':
		{
			if (get_entry && !get_func)
			{
				asm_file << "\tpush 0\n\tcall ExitProcess\nmain ENDP\nend main";
			}
			if (get_func)
			{
				asm_file << "\tpop eax" << endl;
				asm_file << func_name << " ENDP\n\n";
				get_func = false;
			}
			break;
		}
		case LEX_PRINT:
		{
			if (table.id.table[table.lt.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::INT)
			{
				asm_file << "\tpush ";
				asm_file << table.id.table[table.lt.table[i + 1].idxTI].id;
				asm_file << endl << "call std_out_int" << endl;
			}
			else if (table.id.table[table.lt.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR
				&& table.id.table[table.lt.table[i + 1].idxTI].idtype == IT::IDTYPE::V)
			{
				asm_file << "\tpush dword ptr " << table.id.table[table.lt.table[i + 1].idxTI].id << endl << "call std_out_str" << endl;
			}
			else if (table.id.table[table.lt.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR
				&& table.id.table[table.lt.table[i + 1].idxTI].idtype == IT::IDTYPE::L)
			{
				asm_file << "\tpush offset " << table.id.table[table.lt.table[i + 1].idxTI].id << endl << "call std_out_str" << endl;
			}
			break;
		}

		case LEX_IF:
		{
			if_iter++;
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax > ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax < ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax == ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "not_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax != ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax <= ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax >= ebx" << endl; break;
			}
			if (table.lt.table[i + 1].lexema == LEX_ID || table.lt.table[i + 1].lexema == LEX_LITERAL)
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush 1" << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax == ebx" << endl;
			}
			break;
		}
		case LEX_ELSE:
		{
			asm_file << "\t.ELSE" << endl;
			break;
		}
		case LEX_ELIF:
		{
			asm_file << "\t.ENDIF" << endl;
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax > ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax < ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax == ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "not_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax != ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax <= ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax >= ebx" << endl; break;
			}
			if (table.lt.table[i + 1].lexema == LEX_ID || table.lt.table[i + 1].lexema == LEX_LITERAL)
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush 1" << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.IF " << "eax == ebx" << endl;
			}
			break;
		}
		case LEX_ENDIF:
		{
			asm_file << "\t.ENDIF" << endl;
			break;
		}
		case LEX_REPEAT:
		{
			asm_file << "\t.REPEAT " << endl;
			break;
		}
		case LEX_UNTIL:
		{
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.UNTIL " << "eax > ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.UNTIL " << "eax < ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.UNTIL " << "eax == ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "not_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.UNTIL " << "eax != ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.UNTIL " << "eax <= ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.UNTIL " << "eax >= ebx" << endl; break;
			}
			if (table.lt.table[i + 1].lexema == LEX_ID || table.lt.table[i + 1].lexema == LEX_LITERAL)
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush 1" << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.UNTIL " << "eax == ebx" << endl;
			}
			break;
		}

		case LEX_WHILE:
		{
			ss_while.clear();
			ss_while << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl
				<< "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl
				<< "\tpop ebx " << endl << "\tpop eax " << endl;

			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.WHILE " << "eax > ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_than")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.WHILE " << "eax < ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.WHILE " << "eax == ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "not_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.WHILE " << "eax != ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "less_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.WHILE " << "eax <= ebx" << endl; break;
			}
			if (table.id.table[table.lt.table[i + 2].idxTI].id == "more_equals")
			{
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush " << table.id.table[table.lt.table[i + 3].idxTI].id << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.WHILE " << "eax >= ebx" << endl; break;
			}
			if (table.lt.table[i + 1].lexema == LEX_ID || table.lt.table[i + 1].lexema == LEX_LITERAL)
			{
				ss_while.clear();
				ss_while << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl
					<< "\tpush 1" << endl << "\tpop eax " << endl << "\tpop ebx " << endl;

				asm_file << "\tpush " << table.id.table[table.lt.table[i + 1].idxTI].id << endl;
				asm_file << "\tpush 1" << endl;
				asm_file << "\tpop eax " << endl;
				asm_file << "\tpop ebx " << endl;
				asm_file << "\t.WHILE " << "eax == ebx" << endl;
			}

			break;
		}

		case LEX_ENDW:
		{
			asm_file << ss_while.str();
			asm_file << "\t.ENDW" << endl;
			ss_while.clear();
			break;
		}

		case LEX_CONTINUE:
		{
			asm_file << "\t.CONTINUE" << endl;
			break;
		}
		case LEX_BREAK:
		{
			asm_file << "\t.BREAK" << endl;
			break;
		}
		}
	
	}
}

void AsmTranslation::create_code_block(const LexicalAnalyze& table)
{
	/*asm_file << "\n.code" << endl;
	for (size_t i = 0; i < table.lt.size; i++)
	{
		if (table.lt.table[i].lexema == LEX_FUNCTION)
		{
			asm_file << "\r"<< table.id.table[table.lt.table[i + 2].idxTI].id << "_proc PROC " << table.id.table[table.lt.table[i + 2].idxTI].id << ",\n\t";
			i += 3;
		}
		if (table.lt.table[i].lexema == LEX_ID 
			&& table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR 
			&& table.id.table[table.lt.table[i].idxTI].idtype == IT::IDTYPE::P)
		{
			asm_file << table.id.table[table.lt.table[i].idxTI].id << ": BYTE";
			if (table.lt.table[++i].lexema != LEX_RIGHTHESIS)
				asm_file << ",\n\t";
		}
		if (table.lt.table[i].lexema == LEX_ID
			&& table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT
			&& table.id.table[table.lt.table[i].idxTI].idtype == IT::IDTYPE::P)
		{
			asm_file << table.id.table[table.lt.table[i].idxTI].id << ": SDWORD";
			if (table.lt.table[++i].lexema != LEX_RIGHTHESIS)
				asm_file << ",\n\t";
		}
		if (table.lt.table[i].lexema == LEX_ID
			&& table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::BOOL
			&& table.id.table[table.lt.table[i].idxTI].idtype == IT::IDTYPE::P)
		{
			asm_file << table.id.table[table.lt.table[i].idxTI].id << " : BYTE";
			if (table.lt.table[++i].lexema != LEX_RIGHTHESIS)
				asm_file << ",\n\t";
		}
	}*/
}

void AsmTranslation::create_main_proc(const LexicalAnalyze& table)
{
	using std::string;
	asm_file << "main PROC\n\t";
	for (int i = 0; i < table.lt.size; i++)
	{

	}
}

void AsmTranslation::create_end_proc()
{
}

void AsmTranslation::start(const LexicalAnalyze& table, const Parm::PARM& log)
{
	asm_file = std::ofstream(log.asm_file);
	create_system_info();
	create_func_info();
	create_const_info(table);
	create_data_info(table);
	create_instructions(table);
}
