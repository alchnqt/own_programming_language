#include "In.h"
#include <fstream>
#include "Error.h"
#include <sstream>
#include "FST.h"
#include "Log.h"
#include <iostream>
namespace In
{
	IN In::getin(wchar_t infile[])
	{
		IN in;
		in.ignor = NULL;
		in.lines = NULL;
		in.size = NULL;

		int col = NULL;
		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];
		std::string fst_str;
		
		std::ifstream f_in(infile);
		int position_counter = 0;
		int line_counter = 0;
		if (f_in.fail())
			throw ERROR_THROW(110);
		char c = IN_CODE_NULL;
		while (in.size < IN_MAX_LEN_TEXT)
		{
			f_in.get(c);
			
			volatile unsigned char index = (volatile unsigned char)(c);

			if (f_in.eof())
			{
				text[in.size] = IN_CODE_NULL;
				in.lines++;
				break;
			}

			switch (in.code[index])
			{
			case in.N:
				text[in.size] = index;
				in.size++;
				in.lines++;
				col++;
				break;

			case in.T:
				text[in.size] = index;
				in.size++;
				col++;
				break;

			case in.P:
				text[in.size] = index;
				in.size++;
				col++;
				break;

			case in.I: 
				in.ignor++;	
				break;
			case in.F: 
				throw ERROR_THROW_IN(111, in.lines, col); 
				break;

			default:
				throw ERROR_THROW_IN(111, in.lines, col);
				break;
			}
		}
		in.text = (char*)text;
		delete[] text;
		
		return in;
	}
}

