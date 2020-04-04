#pragma once

namespace ose
{
	namespace fs
	{
		enum EFileFlag
		{
			FF_READ = 1,
			FF_WRITE = 2,
			FF_APPEND = 3,
			FF_BINARY = 4
		};

		class File final
		{
		public:
			File(std::string const & path);
			~File();

			// Creates the file and any required parent folders
			// Returns true if the file is created or if the file already exists
			bool Create();

			// Returns true iff the file exists
			bool Exists();

			// Opens a stream to the file object for reading/writing/appending
			void Open(int flags=FF_READ|FF_WRITE);

			// Closes the open stream to the file (if there is one)
			void Close();

			// TODO - Remove in favour of read and write functions
			std::fstream & GetStream() { return stream_; }

			// Read
			// TODO

			// Write

		private:
			std::string path_;
			std::fstream stream_;
		};
	}
}
