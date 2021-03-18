#pragma once
#ifndef IT_H_
#define IT_H_
#include <climits>
#define ID_MAXSIZE 5
#define TI_MAXSIZE 4096 
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX INT_MAX
#define TI_STR_MAXSIZE 255
#include <string>
#include "Parm.h"
namespace IT
{
	enum IDDATATYPE {INT, STR, BOOL};
	enum IDTYPE {N = -1, V = 0, F, P, L, O};

	struct Entry
	{
		int idxfirstLE;
		std::string visibility;
		std::string id;
		IDDATATYPE iddatatype;
		IDTYPE idtype = N;
		int countOfPar = 0;
		struct
		{
			bool vbool = false;
			int vint;
			struct 
			{
				int len;
				std::string str;
			}vstr;
		}value;
	};

	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};
	IdTable Create(int size);
	void Add(IdTable& idtable, const Entry& entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, const char* id);
	int IsId(IdTable& idtable, const std::string& id, char* prefix);
	int IsId(IdTable& idtable, const std::string& id);
	void Delete(IdTable& idtable);
	void Show(IdTable& idtable, Parm::PARM parm);
}
#endif // !IT_H_
