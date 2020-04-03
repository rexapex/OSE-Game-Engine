#pragma once

#include <string>
#include <vector>
#include <map>
#include <array>
#include <unordered_map>

#include <numeric>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <iostream>

#include <memory>
#include <functional>
#include <any>
#include <variant>
#include <initializer_list>

#include "OSE-Core/Types.h"

#define GLM_FORCE_RADIANS
#include "OSE-Core/EngineDependencies/glm/glm.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtx/quaternion.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/type_ptr.hpp"
#include "OSE-Core/EngineDependencies/glm/gtc/matrix_transform.hpp"

// Convenience method for output
// Use \n instead of endl so the buffer isn't flushed with every log
#define LOG(x) do {std::cout << x << "\n";} while(0)

// Convenience method for output of error messages
#define ERROR_LOG(x) do {std::cerr << x << std::endl;} while(0)

// Export functions for testing iff building in debug mode
#ifdef _WIN32
#	ifdef _DEBUG
		// Output stream for debug mode only
		// Could possibly convert to inline function for type safety
#		define DEBUG_LOG(x) do {std::cout << x << std::endl;} while(0)

		// Define warning C4251 when debugging as DEBUG_EXPORT is only needed for unit tests
		// Hopefully unit tests still function correctly ?!?!?!?
		// #pragma warning(disable:4251)
#		define DEBUG_EXPORT __declspec(dllexport)
#	else
#		define DEBUG_EXPORT
#		define DEBUG_LOG(x) do {} while(0)
#	endif // DEBUG
#else
#		define DEBUG_EXPORT
#		define DEBUG_LOG(x) do {} while(0)
#endif
