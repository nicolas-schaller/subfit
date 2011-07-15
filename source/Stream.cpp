#include "Stream.h"

namespace subfit
{

// Type definition
	class Stream::File_Open_Failed : public Error
	{
	public: // Initialization
		File_Open_Failed( const char* file_name ) :
			file_name_( file_name )
		{
		}

	private: // Callback
		virtual void write( std::ostream& stream ) const
		{
			stream << "Fail to open file " << file_name_;
		}

	private: // Data
		const char* file_name_;
	};

	class Stream::Invalid_IO : public Error
	{
	private: // Callback
		virtual void write( std::ostream& stream ) const
		{
			stream << "I/O operation invalid";
		}
	};

	Stream Stream::open( const char* path )
	{
		FILE* file = fopen( path, "rt" );
		if( file == 0 ) throw File_Open_Failed( path );

		return Stream( file, extract_type( file ), false );
	}

	Stream Stream::create( const char* path, utf_t type )
	{
		FILE* file = fopen( path, "wt" );
		if( file == 0 ) throw File_Open_Failed( path );

		write_type( file, type );

		return Stream( file, type, true );
	}

	Stream::Stream( const Stream& stream ) :
		file_( stream.file_ ), type_( stream.type_ ), writable_( stream.writable_ )
	{
		stream.file_ = 0;
	}

	Stream::Stream( FILE* file, utf_t type, bool writable ) :
		file_( file ), type_( type ), writable_( writable )
	{
	}

	Stream::~Stream()
	{
		if( file_ != 0 )
			fclose( file_ );
	}

	bool Stream::end_reached() const
	{
		if( file_ == 0 ) throw Invalid_IO();

		return feof( file_ );
	}

	size_t Stream::read_line( wchar_t* buffer, size_t length, bool skip_emtpy_line )
	{
		if( file_ == 0 ) throw Invalid_IO();

		size_t count;
		wchar_t c = 0;
		do
		{
			count = 0;
			while( count < length && !feof( file_ ) )
			{
				buffer[count] = c = fgetwc( file_ );
				count++;

				if( c == '\n' ) break;
				if( c == '\r' )
				{
					buffer[count - 1] = c = '\n';

					if( !feof( file_ ) )
					{
						size_t next = fgetwc( file_ );
						if( next != '\n' )
							ungetwc( next, file_ );
					}

					break;
				}
			}
		} while( skip_emtpy_line && count == 1 && c == '\n' );

		if( count < length )
			buffer[count] = 0;
		else if( length > 0 )
			buffer[length - 1] = 0;

		return count;
	}

	bool Stream::write( const wchar_t* format, ... )
	{
		if( file_ == 0 ) throw Invalid_IO();
		if( !writable_ ) throw Invalid_IO();

		va_list arg_list;

		va_start( arg_list, format );

		size_t count = vfwprintf( file_, format, arg_list );

		va_end( arg_list );

		return count;
	}

	// Lazy code to detect unicode type and move the file cursor just after the
	// possible type code
	utf_t Stream::extract_type( FILE* file )
	{
		utf_t type; 
		int c = fgetc( file );

		switch( c )
		{
			case 0xef :
				fgetc( file ); fgetc( file );
				type = UTF_8;
				break;

			case 0xfe :
				fgetc( file );
				type = UTF_16L;
				break;

			case 0xff :
				fgetc( file );
				c = fgetc( file );
				if( c == 0x00 )
				{
					fgetc( file );
					type = UTF_32L;
				}
				else
				{
					ungetc( c, file );
					type = UTF_16B;
				}
				break;

			case 0x00 :
				fgetc( file ); fgetc( file ); fgetc( file );
				type = UTF_32B;
				break;

			default:
				ungetc( c, file );
				type = UTF_NONE;
		}

		return type;
	}

	void Stream::write_type( FILE* file, utf_t type )
	{
		char utf8_header[] = { 0xef, 0xbb, 0xbf };
		char utf16l_header[] = { 0xfe, 0xff };
		char utf16b_header[] = { 0xff, 0xfe };
		char utf32l_header[] = { 0xff, 0xfe, 0x00, 0x00 };
		char utf32b_header[] = { 0x00, 0x00, 0xfe, 0xff };

		switch( type )
		{
			case UTF_8   : fwrite( utf8_header, sizeof( utf8_header ), 1, file ); break;
			case UTF_16L : fwrite( utf16l_header, sizeof( utf16l_header ), 1, file ); break;
			case UTF_16B : fwrite( utf16b_header, sizeof( utf16b_header ), 1, file ); break;
			case UTF_32L : fwrite( utf32l_header, sizeof( utf32l_header ), 1, file ); break;
			case UTF_32B : fwrite( utf32b_header, sizeof( utf32b_header ), 1, file ); break;
			default: break;
		}
	}

} // namespace subfit
