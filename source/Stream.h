#ifndef SRT_STREAM_H
#define SRT_STREAM_H

#include <stdio.h>
#include "utf.h"
#include "Error.h"

namespace subfit
{

	class Stream
	{
	public: // Type definition
		class File_Open_Failed;
		class Invalid_IO;

	public: // Factory
		static Stream open( const char* path );
		static Stream create( const char* path, utf_t type );

	public: // Initialization and finalization
		Stream( const Stream& stream );
		~Stream();

	public: // Command
		utf_t type() const { return type_; }
		bool end_reached() const;

	public: // Command
		size_t read_line( wchar_t* buffer, size_t length, bool skip_emtpy_line = false );
		bool write( const wchar_t* format, ... );

	private: // Helper
		Stream( FILE* file, utf_t type, bool writable );
		static utf_t extract_type( FILE* file );
		static void write_type( FILE* file, utf_t type );

	private: // Data
		mutable FILE* file_;
		utf_t type_;
		bool writable_;
	};

} // namespace subfit

#endif // SRT_STREAM_H
