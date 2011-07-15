#include "Timestamp.h"

namespace subfit
{

	Timestamp::Timestamp() :
		hour( 0 ),
		minute( 0 ),
		second( 0 ),
		msecond( 0 )
	{
	}

	size_t Timestamp::to_integer() const
	{
		return
			hour * 3600 * 1000 +
			minute * 60 * 1000 +
			second * 1000 +
			msecond;
	}

	bool Timestamp::read( Stream& stream )
	{
		wchar_t buffer[32];
		float seconds;

		stream.read_line( buffer, 32, true );

		if( swscanf( buffer, L"%u:%u:%f", &hour, &minute, &seconds ) != 3 )
			return false;

		second = (size_t)seconds;
		msecond = (size_t)( ( seconds - second ) * 1000.0f );

		return true;
	}

	void Timestamp::write( Stream& stream ) const
	{
		stream.write( 
			L"%02u:%02u:%.3f\n",
			hour, minute, (float)second + (float)msecond / 1000.0f );
	}


} // namespace subfit
