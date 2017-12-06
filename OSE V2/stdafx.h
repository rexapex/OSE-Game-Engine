#pragma once

#include <string>
#include <vector>
#include <map>

#include <iostream>
#include <memory>

#include "EngineReferences.h"

//convenience method for output
#define LOG(x) do {std::cerr << x << std::endl;} while(0)

//export functions for testing iff building in debug mode
#ifdef _WIN32
#	ifdef _DEBUG
		//output stream for debug mode only
		//could possibly convert to inline function for type safety
#		define DEBUG_LOG(x) do {std::cerr << x << std::endl;} while(0)

		//define warning C4251 when debugging as DEBUG_EXPORT is only needed for unit tests
		//hopefully unit tests still function correctly ?!?!?!?
		//#pragma warning(disable:4251)
#		define DEBUG_EXPORT __declspec(dllexport)
#	else
#		define DEBUG_EXPORT
#		define DEBUG_LOG(x) do {} while(0)
#	endif // DEBUG
#endif
