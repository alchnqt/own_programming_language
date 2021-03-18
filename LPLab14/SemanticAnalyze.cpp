#include "SemanticAnalyze.h"
#include "Error.h"
#include <iostream>
using Error::exception;
void semantic::Semantics(LexicalAnalyze& table)
{
	int counterFunction = 0;
	int LTStartFunction = 0, LTFinishFunction = 0, type = 0, functionType = 0;

	for (int j = 0; j < table.lt.size; j++) 
	{
		if (LT::GetEntry(table.lt, j).lexema == 'd' && LT::GetEntry(table.lt, j + 2).lexema == 'i') counterFunction++;
	}

	for (int k = 0; k < counterFunction; k++) {
		for (int i = LTFinishFunction; i < table.lt.size; i++) {
			if (LT::GetEntry(table.lt, i).lexema == 'd' && LT::GetEntry(table.lt, i + 2).lexema == 'i') {
				LTStartFunction = i;
				break;
			}

		}
		for (int i = LTFinishFunction + 1; i < table.lt.size; i++) {
			if (LT::GetEntry(table.lt, i).lexema == '}') {
				LTFinishFunction = i;
				break;
			}
		}
		type = table.id.table[LT::GetEntry(table.lt, LTStartFunction + 2).idxTI].iddatatype;
		functionType = table.id.table[LT::GetEntry(table.lt, LTFinishFunction - 2).idxTI].idtype;
		if (type != functionType && functionType == 1) throw exception("Тип функции и возвращаемой переменной не совпадают", 606);
	}

	for (int i = 0; i < table.lt.size; i++) {
		if (
			 ((	
			 	table.lt.table[i].lexema == LEX_ID 
			 	&& table.lt.table[i + 2].lexema == LEX_ID
			 )
			 || 
			 (	
			 	table.lt.table[i].lexema == LEX_LITERAL
			 	&& table.lt.table[i + 2].lexema == LEX_LITERAL
			 )
			||
			(
				table.lt.table[i].lexema == LEX_ID
				&& table.lt.table[i + 2].lexema == LEX_LITERAL
			)
			||
			(
				table.lt.table[i].lexema == LEX_LITERAL
				&& table.lt.table[i + 2].lexema == LEX_ID
			))
			 && table.lt.table[i + 1].lexema != LEX_COMMA 
			 && table.lt.table[i + 1].lexema != LEX_EQUAL 
			 && table.lt.table[i + 1].lexema != LEX_LEFTHESIS
			 && table.lt.table[i + 1].lexema != LEX_SEMICOLON
			)
		{
			if (
					(
					table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR 
					&& table.id.table[table.lt.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::STR
					)
				)
			{
				std::cout << table.id.table[table.lt.table[i].idxTI].id << table.lt.table[i + 1].lexema << table.id.table[table.lt.table[i + 2].idxTI].id;
				throw exception("Арифметические операции со строками недопустимы", 604, table.lt.table[i].sn);
			}
				
		}
	}
	for (int i = 0; i < table.lt.size; i++) {
		if (
				table.lt.table[i].lexema == LEX_ID
				&& table.lt.table[i + 2].lexema == LEX_ID
				&& table.lt.table[i + 1].lexema != LEX_COMMA
				&& table.lt.table[i + 1].lexema != LEX_LEFTHESIS
				&& table.lt.table[i + 1].lexema != LEX_SEMICOLON
			)
		{
			if (
					(
						table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT
						&& table.id.table[table.lt.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::STR
					)
				||
					(
						table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR
						&& table.id.table[table.lt.table[i + 2].idxTI].iddatatype == IT::IDDATATYPE::INT
					)
				)
			{
				throw exception("Несоответствие типов операндов", 603, table.lt.table[i].sn);
			}

		}
	}
	
	for (int i = 0; i < table.lt.size; i++) {
		if  (
				(table.lt.table[i].lexema == LEX_ID || table.lt.table[i].lexema == LEX_LITERAL)
				&& table.lt.table[i + 2].lexema == LEX_LITERAL
				&& table.lt.table[i + 1].lexema == LEX_OPERATOR
			)
		{
			if (table.id.table[table.lt.table[i].idxTI].iddatatype == IT::IDDATATYPE::INT 
				&& table.id.table[table.lt.table[i + 1].idxTI].idtype == IT::IDTYPE::O 
				&& table.id.table[table.lt.table[i + 1].idxTI].id == "/"
				&& table.id.table[table.lt.table[i + 2].idxTI].value.vint == 0)
			{
				throw exception("Деление на 0", 605, table.lt.table[i].sn);
			}

		}
	}

	int params = 0;
	int total_params = 0;
	for (int i = 0; i < table.lt.size; i++) {
		if (
			(table.lt.table[i].lexema == LEX_ID
				&& table.lt.table[i + 1].lexema == LEX_LEFTHESIS)
			)
		{
			total_params = table.id.table[table.lt.table[i].idxTI].countOfPar;
			for (int k = i; k < table.lt.size; k++, i++)
			{
				if ((table.lt.table[k].lexema == LEX_ID || table.lt.table[k].lexema == LEX_LITERAL)
						&& table.lt.table[k + 1].lexema == LEX_COMMA)
					params++;
				if ((table.lt.table[k].lexema == LEX_ID || table.lt.table[k].lexema == LEX_LITERAL)
					&& table.lt.table[k + 1].lexema == LEX_RIGHTHESIS)
				{
					params++;
					break;
				}
			}
			if (total_params != params)
			{
				std::cout << table.id.table[table.lt.table[i].idxTI].id;
				throw exception("Количество параметров в вызываемой функции не соответствует её сигнатуре", 607, table.lt.table[i].sn);
			}
		}
		params = 0;
	}
}
