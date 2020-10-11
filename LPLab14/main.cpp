#include "Error.h"
#include "Log.h"
#include "In.h"
#include "Parm.h"
#include "Out.h"
#include <iostream>
#include <locale>
#include "FST.h"

int _tmain(int argc, wchar_t* argv[])
{

	Log::LOG log;
	setlocale(LC_ALL, "rus");
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "Тест: ", "Без ошибок", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);
		Out::OUT out = Out::GetOut(parm, in);
		Out::WriteOut(in, out);
		Out::Close(out);
		Log::Close(log);
		FST::FST ft = FST::FST("1", FST_INTEGER_LITERAL);
		if (FST::execute(ft))
			std::cout << "\naccess\n";
		else
			std::cout << ft.string[ft.position] << " position: " << ft.position + 1;
	}
	catch (Error::ERROR e)
	{
		Parm::PARM parm = Parm::getparm(argc, argv); 
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "Тест: ", "C ошибками", "");
		Log::WriteLog(log);
		Log::WriteOut(log);
		Log::WriteError(log, e);
	}

	return 0;
}