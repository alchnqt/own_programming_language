#include "LT.h"
#include "FST.h"
#include <fstream>
#include "Parm.h"
#include "Error.h"
using LT::LexTable;
using LT::Entry;

namespace LT
{
	LexTable Create(int size)
	{
		LexTable Table;
		if (size > LT_MAXSIZE)
			throw Error::geterror(120);
		Table.maxsize = size;
		Table.size = 0;
		Table.table = new Entry[size];

		return Table;
	}

	void Add(LexTable& lextable, const Entry& entry)
	{
		if (lextable.size + 1 > lextable.maxsize)
			throw Error::geterror(121);
		lextable.table[lextable.size] = entry;
		lextable.size += 1;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}

	Entry writeEntry(Entry& entry, char lexema, int indx, int line)
	{
		entry.lexema = lexema;
		entry.idxTI = indx;
		entry.sn = line;
		return entry;
	}

	void LT::Show(LexTable& lextable, Parm::PARM parm)
	{
		std::ofstream out(parm.lex_file);
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema == '\n')
				out << lextable.table[i].lexema << std::endl;
			else
				out << lextable.table[i].lexema;
		}
		out.close();
	}

}

