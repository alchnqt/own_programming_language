#pragma once
#ifndef ASM_TRANSLATION_H_
#include "LexicalAnalyze.h"
#include "Parm.h"
#define ASM_START_BLOCK    ".586\n.model flat,stdcall\n"
#define ASM_LIBS_BLOCK     "includelib libucrt.lib\nincludelib kernel32.lib\n"
#define ASM_CONST_BLOCK    "\n.const\n"
#define ASM_STACK_BLOCK    "\n.stack 4096\n"
#define ASM_DATA_BLOCK     "\n.data\n"
#define ASM_MAIN_BLOCK     "\nmain PROC\n"
#define ASM_MAIN_END_BLOCK "\nend main\n"
namespace AsmTranslation
{
	using lexical_analyze::LexicalAnalyze;
	
	void create_system_info();
	void create_const_info(const LexicalAnalyze& table);
	void create_data_info(const LexicalAnalyze& table);
	void create_func_info();
	void create_code_block(const LexicalAnalyze& table);

	void create_main_proc(const LexicalAnalyze& table);
	void create_end_proc();
	void start(const LexicalAnalyze& table,const Parm::PARM& log);
};
#define ASM_TRANSLATION_H_
#endif


