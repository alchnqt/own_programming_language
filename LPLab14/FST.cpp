#include "FST.h"
#include <algorithm>
#include <cstring>
FST::RELATION::RELATION(char c, short ns)
{
	symbol = c;
	nnode = ns;
}

FST::NODE::NODE()
{
	n_relation = 0;
	realations = NULL;
}

FST::NODE::NODE(short n, RELATION rel, ...)
{
	n_relation = n;
	RELATION* ptr = &rel;
	realations = new RELATION[n];
	for (short i = 0; i < n; i++)
		realations[i] = ptr[i];
}

FST::FST::FST(const char* s, short ns, NODE n, ...)
{
	string = const_cast<char*>(s);
	
	nstates = ns;
	nodes = new NODE[ns];
	NODE* ptr = &n;
	for (int i = 0; i < ns; i++)
		nodes[i] = ptr[i];
	rstates = new short[nstates];
	memset(rstates, 0xff, sizeof(short) * nstates);
	rstates[0] = 0;
	position = -1;
}

bool step(FST::FST& fst, short*& rstates)
{
	bool rc = false;
	std::swap(rstates, fst.rstates);
	for (short i = 0; i < fst.nstates; i++)
	{
		if (rstates[i] == fst.position)
		{
			for (short j = 0; j < fst.nodes[i].n_relation; j++)
			{
				if (fst.nodes[i].realations[j].symbol == fst.string[fst.position])
				{
					fst.rstates[fst.nodes[i].realations[j].nnode] = fst.position + 1;
					rc = true;
				}
			}
		}
	}
	return rc;
}
bool FST::execute(FST& fst)
{
	short len = static_cast<short>(std::strlen(fst.string));
	short* rstates = new short[fst.nstates];
	std::memset(rstates, 0xff, sizeof(short) * fst.nstates);
	bool rc = true;
	for (short i = 0; i < len && rc; i++)
	{
		fst.position++;
		rc = step(fst, rstates);
	}
	delete[] rstates;
	return (rc ? (fst.rstates[fst.nstates - 1] == len) : rc);
}
