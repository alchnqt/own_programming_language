#include "IT.h"
#include "Error.h"
#include <string.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
using IT::Entry;
using IT::IdTable;
namespace IT
{
	std::string toString(int val)
	{
		std::ostringstream oss;
		oss << val;
		return oss.str();
	}
	IdTable Create(int size)
	{
		IdTable tabl;
		if (size > TI_MAXSIZE) throw ERROR_THROW(160);

		tabl.maxsize = size;
		tabl.size = 0;
		tabl.table = new Entry[size];
		return tabl;
	}

	void Add(IdTable& idtable, const Entry& entry)
	{
		if (idtable.size + 1 > idtable.maxsize)
			throw ERROR_THROW(161);

		idtable.table[idtable.size] = entry;
		idtable.size += 1;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IsId(IdTable& idtable,const char* id)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (!strcmp(idtable.table[i].id.c_str(), id)) return i;
		}

		return TI_NULLIDX;
	}
	int IsId(IdTable& idtable, const std::string& id)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].id == id)
				return i;
		}
		return TI_NULLIDX;
	}
	int IsId(IdTable& idtable,const std::string& id, char* prefix)
	{
		std::string tmp(prefix);
		tmp += id;
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].id == tmp)
				return i;
		}

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}
	void Show(IdTable& idtable, Parm::PARM parm)
	{
		using IT::IDDATATYPE;
		using IT::IDTYPE;
		std::ofstream out(parm.id_file);
		int i, numberOP = 0;
		out << std::setfill(' ') << std::setw(100) << ' ' << std::endl;
		out << " №" << " * " << "Идентификатор" << " * " << "Тип данных" << " * " << "Тип идентификатора" << " * " << "Индекс в ТЛ" << " * " << "Значение" << std::setw(toString(idtable.size).length()) << std::endl;
		out << std::setw(100) << ' ' << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			out << std::setfill('0') << std::setw(toString(idtable.size).length()) << std::right << i << " * ";
			out << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " * ";
			switch (idtable.table[i].iddatatype)
			{
			case IDDATATYPE::INT: out << std::setw(10) << std::left;
				if (idtable.table[i].idtype == IDTYPE::O) out << "-" << " * ";
				else out << "integer" << " * "; break;
			case IDDATATYPE::STR: out << std::setw(10) << std::left;
				if (idtable.table[i].idtype == IDTYPE::O) out << "-" << " | ";
				else out << "string" << " * "; break;
			case IDDATATYPE::BOOL: out << std::setw(10) << std::left;
				if (idtable.table[i].idtype == IDTYPE::O) out << "-" << " | ";
				else out << "boolean" << " * "; break;
			default: out << std::setw(10) << std::left << "-" << " * "; break;
			}
			switch (idtable.table[i].idtype)
			{
			case IDTYPE::V: out << std::setw(18) << std::left << "переменная" << " * "; break;
			case IDTYPE::F: out << std::setw(18) << std::left << "функция"    << " * "; break;
			case IDTYPE::P: out << std::setw(18) << std::left << "параметр"   << " * "; break;
			case IDTYPE::L: out << std::setw(18) << std::left << "литерал"    << " * "; break;
			case IDTYPE::O: out << std::setw(18) << std::left << "оператор"   << " * ";
				numberOP++;
				break;
			default: out << std::setw(18) << std::left << "-" << " - "; break;
			}
			out << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " * ";
			if (idtable.table[i].iddatatype == IDDATATYPE::INT && (idtable.table[i].idtype == IDTYPE::V || idtable.table[i].idtype == IDTYPE::L))
				out << std::setw(18) << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == IDDATATYPE::STR && (idtable.table[i].idtype == IDTYPE::V || idtable.table[i].idtype == IDTYPE::L))
				out << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else if (idtable.table[i].iddatatype == IDDATATYPE::BOOL && (idtable.table[i].idtype == IDTYPE::V || idtable.table[i].idtype == IDTYPE::L))
				out << std::setw(18) << idtable.table[i].value.vbool;
			else
				out << "-";
			out << std::endl;
		}
		out << std::setfill(' ') << std::setw(100) << ' ' << std::endl;
		out << "Количество идентификаторов: " << i - numberOP << std::endl;
		out << std::setw(100) << '=' << std::endl;
	}
}