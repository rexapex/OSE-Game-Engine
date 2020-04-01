#pragma once

namespace ose
{
	namespace std_ext
	{
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
	}
}
