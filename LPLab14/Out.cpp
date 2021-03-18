#include "Out.h"
#include "Error.h"
#include <iostream>
#include "FST.h"
#include "In.h"
#include "LT.h"
#include "IT.h"
#include <iomanip>
#include "LexicalAnalyze.h"


void Out::WriteOut(In::IN& in, OUT& out)
{
	//del_waste(in, out);
	* out.stream << out.text;
}
void Out::Close(OUT& out)
{
	out.stream->close();
	delete out.stream;
}




