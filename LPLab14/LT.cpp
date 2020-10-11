#include "LT.h"
#include "FST.h"


using LT::LexTable;
using LT::Entry;

LexTable LT::Create(int size)
{
	LexTable lt;
	lt.maxsize = size;
	lt.size = 0;
	lt.table = new Entry[size];
	return lt;
}

void LT::Add(LexTable& lt,const Entry& entry)
{
	
	lt.table[lt.size] = entry;
}

Entry LT::GetEntry(LexTable& lextable, int n)
{
	return lextable.table[n];
}

void LT::Delete(LexTable& lextable)
{
	if (lextable.table)
	{
		delete[] lextable.table;
		lextable.table = nullptr;
	}
}
