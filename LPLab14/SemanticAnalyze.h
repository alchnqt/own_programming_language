#pragma once

#ifndef SemanticAnalyze_h__
#define SemanticAnalyze_h__
#include "LexicalAnalyze.h"
namespace semantic
{
	using lexical_analyze::LexicalAnalyze;
	void Semantics(LexicalAnalyze& la);
}

#endif // SemanticAnalyze_h__

