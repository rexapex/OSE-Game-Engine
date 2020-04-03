#pragma once

#include "OSE-Core/Resources/FileHandlingUtil.h"

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
		inline void Log(char const * filename, char const * line, char const * function, T t, Args... args)
		{
#ifdef CONSOLE_LOGGING
			std::clog << "LOG   : " << FileHandlingUtil::GetRelativePath(filename, FileHandlingUtil::GetParentPathFromPath(PROJECT_DIR)) << " : " << line << " : " << function << " : ";
			LogToStream(std::clog, t, args...);
			std::clog << "\n";
#endif
			// If debug build with console logging enabled, don't bother logging to file
#if !defined(CONSOLE_LOGGING) || !defined(_DEBUG)
			// TODO - Log to file
#endif
		}

		// Convenience method for output of error messages
		template <typename T, typename... Args>
		inline void LogError(char const * filename, char const * line, char const * function, T t, Args... args)
		{
#ifdef CONSOLE_LOGGING
			std::cerr << "ERROR : " << FileHandlingUtil::GetRelativePath(filename, FileHandlingUtil::GetParentPathFromPath(PROJECT_DIR)) << " : " << line << " : " << function << " : ";
			LogToStream(std::cerr, t, args...);
			std::cerr << std::endl;
#endif
			// If debug build with console logging enabled, don't bother logging to file
#if !defined(CONSOLE_LOGGING) || !defined(_DEBUG)
			// TODO - Log to file
#endif
		}

		// Convenience method for output in debug mode only
		template <typename T, typename... Args>
		inline void DebugLog(char const * filename, char const * line, char const * function, T t, Args... args)
		{
#ifdef CONSOLE_LOGGING
			std::clog << "DEBUG : " << FileHandlingUtil::GetRelativePath(filename, FileHandlingUtil::GetParentPathFromPath(PROJECT_DIR)) << " : " << line << " : " << function << " : ";
			LogToStream(std::clog, t, args...);
			std::clog << std::endl;
#endif
			// If debug build with console logging enabled, don't bother logging to file
#if !defined(CONSOLE_LOGGING) || !defined(_DEBUG)
			// TODO - Log to file
#endif
		}
	}
}

#ifndef PROJECT_DIR
#error "PROJECT_DIR is not defined, add PROJECT_DIR=R\"($(ProjectDir))\"; as a preprocessor token"
#endif

#ifndef __FILE__
#	define __FILE__ "#"
#endif
#ifndef __LINE__
#	define __LINE__ "#"
#endif
#ifdef _MSC_VER
#	define __func__ __FUNCTION__
#endif

#ifndef STR
#	define STR(x) #x
#endif
#ifndef TOSTR
#	define TOSTR(x) STR(x)
#endif

// Convenience method for output
// Use \n instead of endl so the buffer isn't flushed with every log
#define LOG(x, ...) ose::logging::Log(__FILE__, TOSTR(__LINE__), __func__, x, ##__VA_ARGS__)

// Convenience method for output of error messages
#define LOG_ERROR(x, ...) ose::logging::LogError(__FILE__, TOSTR(__LINE__), __func__, x, ##__VA_ARGS__)

#ifdef _WIN32
#	ifdef _DEBUG
#		define DEBUG_LOG(x, ...) ose::logging::DebugLog(__FILE__, TOSTR(__LINE__), __func__, x, ##__VA_ARGS__)
#	else
#		define DEBUG_LOG(x, ...) do {} while(0)
#	endif // _DEBUG
#else
#		define DEBUG_LOG(x, ...) do {} while(0)
#endif // _WIN32
