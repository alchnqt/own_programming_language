#include "Error.h"
#include "Log.h"
#include "In.h"
#include "Parm.h"
#include "Out.h"
#include <iostream>
#include <locale>
#include "FST.h"
#include "LexicalAnalyze.h"
#include "PolishNotation.h"
#include "MFST.h"
#include "Greibach.h"
#include "SemanticAnalyze.h"
#include "AsmTranslation.h"
int _tmain(int argc, wchar_t* argv[])
{

	Log::LOG log;
	setlocale(LC_ALL, "rus");
	try
	{
		using lexical_analyze::LexicalAnalyze;
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "Тест: ", "Без ошибок", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);
		LexicalAnalyze LA = lexical_analyze::Start(in);
		IT::Show(LA.id, parm);
		LT::Show(LA.lt, parm);
		unsigned int start_time = clock();
		MFST::Mfst mfst(LA.lt, GRB::getGreibach());
		mfst.start(parm);
		mfst.savededucation();
		semantic::Semantics(LA);
		bool rc = polish_notation::start(LA);
		AsmTranslation::start(LA, parm);
		lexical_analyze::Delete(LA);
		Log::Close(log);
		std::cout << "Трансляция файла в язык MASM32 прошла успешно" << std::endl;
	}
	catch (Error::ERROR e)
	{
		Parm::PARM parm;
		wcscpy_s(parm.log,L"log.txt.log");
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "Тест: ", "C ошибками", "");
		Log::WriteLog(log);
		Log::WriteOut(log);
		Log::WriteError(log, e);
	}
	catch (Error::exception e)
	{
		Parm::PARM parm;
		wcscpy_s(parm.log, L"log.txt.log");
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "Тест: ", "C ошибками", "");
		Log::WriteLog(log);
		Log::WriteOut(log);
		Log::WriteError(log, e);
	}

	return 0;
}