#include "Subtitle.h"

namespace subfit
{

	size_t read_index( Stream& stream )
	{
		wchar_t buffer[32];
		size_t index = 0;

		stream.read_line( buffer, 32, true );
		swscanf( buffer, L"%u", &index );

		return index;
	}

	bool Subtitle::read( Stream& stream )
	{
		if( read_index( stream ) != 0 &&  range_.read( stream ) )
		{
			unsigned int index = 0;
			unsigned int length = 0;
			text_[0] = 0;

			do
			{
				length = stream.read_line( &text_[index], 1024, index == 0 );
				index += length;
			} while( length > 1 );

			if( length == 1 || text_[index - 1] == '\n' )
				text_[index - 1] = 0;

			return true;
		}
		else return false;
	}

} // namespace subfit
