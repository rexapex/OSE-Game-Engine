#pragma once

namespace ose
{
	namespace std_ext
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  Starts/Ends With
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Returns true iff the string "str" starts with the string "start"
		inline bool starts_with(std::string str, std::string start)
		{
			// Based on: https://stackoverflow.com/questions/8095088/how-to-check-string-start-in-c/8095132
			return start.size() <= str.size()
				&& std::equal(start.begin(), start.end(), str.begin());
		}

		// Returns true iff the string "str" ends with the string "end"
		inline bool ends_with(std::string str, std::string end)
		{
			// Based on: https://stackoverflow.com/questions/8095088/how-to-check-string-start-in-c/8095132
			return end.size() <= str.size()
				&& std::equal(end.begin(), end.end(), str.end()-end.size());
		}



		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  Trimming
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Trim from start (in place)
		inline void ltrim_inplace(std::string & s)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !std::isspace(ch);
			}));
		}

		// Trim from end (in place)
		inline void rtrim_inplace(std::string & s)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch);
			}).base(), s.end());
		}

		// Trim from both ends (in place)
		inline void trim_inplace(std::string & s)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			ltrim_inplace(s);
			rtrim_inplace(s);
		}

		// Trim from start (copying)
		inline std::string ltrim(std::string s)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			ltrim_inplace(s);
			return s;
		}

		// Trim from end (copying)
		inline std::string rtrim(std::string s)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			rtrim_inplace(s);
			return s;
		}

		// Trim from both ends (copying)
		inline std::string trim(std::string s)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			trim_inplace(s);
			return s;
		}

		// Trim from start (in place)
		inline void ltrim_inplace(std::string & s, int c)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [c](int ch) {
				return c != ch;
			}));
		}

		// Trim from end (in place)
		inline void rtrim_inplace(std::string & s, int c)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			s.erase(std::find_if(s.rbegin(), s.rend(), [c](int ch) {
				return c != ch;
			}).base(), s.end());
		}

		// Trim from both ends (in place)
		inline void trim_inplace(std::string & s, int c)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			ltrim_inplace(s, c);
			rtrim_inplace(s, c);
		}

		// Trim from start (copying)
		inline std::string ltrim(std::string s, int c)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			ltrim_inplace(s, c);
			return s;
		}

		// Trim from end (copying)
		inline std::string rtrim(std::string s, int c)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			rtrim_inplace(s, c);
			return s;
		}

		// Trim from both ends (copying)
		inline std::string trim(std::string s, int c)
		{
			// Based on: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
			trim_inplace(s, c);
			return s;
		}
	}
}
