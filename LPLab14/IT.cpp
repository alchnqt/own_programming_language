#include "IT.h"

using IT::Entry;
using IT::IdTable;

IdTable IT::IdTable::Create(int size)
{
    IdTable it;
    it.maxsize = size;
    it.size = 0;
    it.table = new Entry[size];
    return it;
}

void IT::IdTable::Add(IdTable& it,const Entry& e)
{
    it.table[size] = e; size++;
}

Entry IT::IdTable::GetEntry(IdTable& it, int n)
{
    return it.table[n];
}

int IT::IdTable::IsId(IdTable& it, char id[ID_MAXSIZE])
{
    size_t i = 0;
    for (; i < it.size; i++)
    {
        if (it.table[i].id == id)
            break;
    }
    return i;
}

void IT::IdTable::Delete(IdTable& idtable)
{
    if (idtable.table)
    {
        delete[] idtable.table;
        idtable.table = nullptr;
    }
}
