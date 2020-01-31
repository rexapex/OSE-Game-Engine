#pragma once

namespace ose
{
	namespace logging
	{
		// Convenience method for output
		inline void Print(std::string text)
		{
			std::cout << text << std::endl;
		}

		// Convenience method for output
		// Use \n instead of endl so the buffer isn't flushed with every log
		inline void Log(std::string text)
		{
			std::cout << text << "\n";
		}

		// Convenience method for output of error messages
		inline void LogError(std::string text)
		{
			std::cerr << text << std::endl;
		}

		// Convenience method for output in debug mode only
		inline void DebugLog(std::string text)
		{
			std::cout << text << std::endl;
		}
	}
}

// Convenience method for output
// Use \n instead of endl so the buffer isn't flushed with every log
#define LOG(x) ose::logging::Log(x)

// Convenience method for output of error messages
#define LOG_ERROR(x) ose::logging::LogError(x)

// Export functions for testing iff building in debug mode
#ifdef _WIN32
#	ifdef _DEBUG
// Output stream for debug mode only
// Could possibly convert to inline function for type safety
#		define DEBUG_LOG(x) ose::logging::DebugLog(x)

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
