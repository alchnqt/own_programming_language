#include "Log.h"
#include <ctime>
#include <iostream>
using std::ofstream;
using std::endl;
using std::wcstombs;
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		log.stream = new std::ofstream(logfile);
		log.stream->open(logfile, std::ios::out);
		log.stream->clear();
		if (!log.stream->is_open())
			throw ERROR_THROW(112);

		wcscpy_s(log.logfile, logfile);
		return log;
	}
	void WriteLine(LOG& log,const char* c, ...)
	{
		const char** ptr_c = &c;
		int iterator = 0;
		while (ptr_c[iterator] != "")
			*log.stream << ptr_c[iterator++];
		*log.stream << endl;
	}
	void WriteLine(LOG& log,const wchar_t* c, ...)
	{
		const wchar_t** ptr_c = &c;
		int iterator = 0;
		char buff[100];
		while (ptr_c[iterator] != L"")
		{
			wcstombs(buff, ptr_c[iterator++], sizeof(buff));
			*log.stream << buff;
		}
			
		*log.stream << endl;
	}
	void WriteLog(LOG& log)
	{
		tm* timeInfo;
		time_t _time;
		char buff[100];
		time(&_time);
		timeInfo = localtime(&_time);
		strftime(buff, sizeof(buff), "\n----------- ПРОТОКОЛ ---------\n---- %d.%m.%y. %T ----\n", timeInfo);
		*log.stream << buff;
	}
	void WriteOut(LOG& log, Out::OUT out)
	{
		//Вариант без списка
		/*if (out.bad_lines.empty())
		{
			log.stream << "\n------Объектный модуль-------\n\n"
			<< "\n\nНепрочитанных строк: " << out.incorrect_lines;
			for (auto& n : out.bad_lines)
			{
				log.stream << "\nЦепочка: " << n.first << " не распознана; позиция: " << n.second.position << " символ: " << n.second.symbol << std::endl;
			}
		}
		else
		{
			log.stream << "\n------Объектный модуль-------\n\n"
				<< "\n\nНепрочитанных строк: " << out.incorrect_lines
			    << "\n\nВсе строки были прочитаны!";
		}*/
	}
	void WriteOut(LOG& log)
	{
		*log.stream << "\n------Объектный модуль не cформирован-------\n\n";
	}
	void WriteParm(LOG& log, Parm::PARM& parm)
	{
		char _in[PARM_MAX_SIZE];
		char _out[PARM_MAX_SIZE];
		char _log[PARM_MAX_SIZE];
		wcstombs(_in, parm.in, PARM_MAX_SIZE);
		wcstombs(_out, parm.out, PARM_MAX_SIZE);
		wcstombs(_log, parm.log, PARM_MAX_SIZE);
		*log.stream
			<< "\n--------ПАРАМЕТРЫ -----------" << endl 
			<< "\n-in:" << _in
			<< "\n-out: " << _out
			<< "\n-log: " << _log
			<< endl;
	}
	void WriteIn(LOG& log, In::IN& in)
	{
		*log.stream << "\n------Исходник-------\n\n" << in.text
			<< "\n\nВсего символов: " << in.size
			<< "\n\nВсего строк: " << in.lines
			<< "\n\nПропущено: " << in.ignor << endl;
	}
	void WriteError(LOG& log, Error::ERROR& error)
	{
		*log.stream << "\nОшибка: " << error.id
			<< ": " << error.message
			<< ", \nСтрока: " << error.inext.line
			<< " позиция " << error.inext.col << endl;
		Close(log);
		
		std::cout << "\nОшибка: " << error.id
		<< ": " << error.message
		<< ", \nСтрока: " << error.inext.line
		<< " позиция " << error.inext.col << endl;

		
	}
	
	void Close(LOG& log)
	{
		log.stream->close();
		delete log.stream;
	}
	
}