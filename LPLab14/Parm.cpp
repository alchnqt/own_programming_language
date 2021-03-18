#include "Parm.h"
#include "Error.h"
using namespace Error;
namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		PARM parm;
		bool in = false;
		bool out = false;
		bool log = false;
		for (int i = 1; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104);

			if (wcsstr(argv[i], PARM_IN))
			{
				wcscpy_s(parm.in, argv[i] + wcslen(PARM_IN));
				in = true;
			}
			if (wcsstr(argv[i], PARM_OUT))
			{
				wcscpy_s(parm.asm_file, argv[i] + wcslen(PARM_OUT));
				out = true;
			}
			if (wcsstr(argv[i], PARM_LOG))
			{
				wcscpy_s(parm.log, argv[i] + wcslen(PARM_LOG));
				log = true;
			}
		}

		if (!in)
			throw ERROR_THROW(100);
		if (!out)
		{
			wcscpy_s(parm.asm_file, parm.in);
			wcscat_s(parm.asm_file, PARM_OUT_DEFAULT_EXT);
		}
		if (!log)
		{
			wcscpy_s(parm.log, parm.in);
			wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
		}
		wcscpy_s(parm.lex_file, parm.in);
		wcscat_s(parm.lex_file, L".mavlextable");
		wcscpy_s(parm.id_file, parm.in);
		wcscat_s(parm.id_file, L".mavidtable");
		wcscpy_s(parm.syntax_file, parm.in);
		wcscat_s(parm.syntax_file, L".mavsyntaxstream");
		return parm;
	}
}
