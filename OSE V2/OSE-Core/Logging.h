#pragma once

namespace ose
{
	namespace logging
	{
		template <typename T>
		inline void LogToStream(std::ostream & stream, T t)
		{
			stream << t;
		}

		template <typename T, typename... Args>
		inline void LogToStream(std::ostream & stream, T t, Args... args)
		{
			stream << t << " ";
			LogToStream(stream, args...);
		}

		// Convenience method for output
		// Use \n instead of endl so the buffer isn't flushed with every log
		template <typename T, typename... Args>
		inline void Log(T t, Args... args)
		{
#ifdef CONSOLE_LOGGING
			std::clog << "LOG   : ";
			LogToStream(std::clog, t, args...);
			std::clog << "\n";
#endif
		}

		// Convenience method for output of error messages
		template <typename T, typename... Args>
		inline void LogError(T t, Args... args)
		{
#ifdef CONSOLE_LOGGING
			std::cerr << "ERROR : ";
			LogToStream(std::cerr, t, args...);
			std::cerr << std::endl;
#endif
		}

		// Convenience method for output in debug mode only
		template <typename T, typename... Args>
		inline void DebugLog(T t, Args... args)
		{
#ifdef CONSOLE_LOGGING
			std::clog << "DEBUG : ";
			LogToStream(std::clog, t, args...);
			std::clog << std::endl;
#endif
		}
	}
}

#ifndef __FILE__
#	define __FILE__ "#"
#endif
#ifndef __LINE__
#	define __LINE__ "#"
#endif
#ifdef _MSC_VER
#	define __func__ "#"//__FUNCTION__
#endif

#ifndef STR
#	define STR(x) #x
#endif
#ifndef TOSTR
#	define TOSTR(x) STR(x)
#endif

#ifndef REL_PATH_NAME
#	define REL_PATH_NAME __FILE__
#endif

#define FUNC_DETAIL_STR "\"" REL_PATH_NAME "\" : " TOSTR(__LINE__) " : " __func__ " :"

// Convenience method for output
// Use \n instead of endl so the buffer isn't flushed with every log
#define LOG(x, ...) ose::logging::Log(FUNC_DETAIL_STR, x, ##__VA_ARGS__)

// Convenience method for output of error messages
#define LOG_ERROR(x, ...) ose::logging::LogError(FUNC_DETAIL_STR, x, ##__VA_ARGS__)

// Export functions for testing iff building in debug mode
#ifdef _WIN32
#	ifdef _DEBUG
#		define DEBUG_LOG(x, ...) ose::logging::DebugLog(FUNC_DETAIL_STR, x, ##__VA_ARGS__)
#	else
#		define DEBUG_LOG(x, ...) do {} while(0)
#	endif // _DEBUG
#else
#		define DEBUG_LOG(x, ...) do {} while(0)
#endif // _WIN32
