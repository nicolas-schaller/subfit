#include "Timerange.h"
#include <math.h>

namespace subfit
{

	Timerange::Timerange()
	{
	}

	Timerange::Timerange( const Timerange& other ) :
		start_( other.start_ ),
		stop_( other.stop_ )
	{
	}

	bool Timerange::read( Stream& stream )
	{
		wchar_t buffer[32];

		stream.read_line( buffer, 32, true );

		return swscanf(
			buffer,
			L"%u:%u:%u,%u --> %u:%u:%u,%u",
			&start_.hour, &start_.minute, &start_.second, &start_.msecond,
			&stop_.hour, &stop_.minute, &stop_.second, &stop_.msecond ) == 8;
	}

	void Timerange::write( Stream& stream ) const
	{
		stream.write( 
			L"%02u:%02u:%02u,%u --> %02u:%02u:%02u,%u\n",
			start_.hour, start_.minute, start_.second, start_.msecond,
			stop_.hour, stop_.minute, stop_.second, stop_.msecond ) == 8;
	}

	float Timerange::get_distance( const Timerange& other ) const
	{
		size_t first_start = start().to_integer();
		unsigned int first_stop = stop().to_integer();
		unsigned int second_start = other.start().to_integer();
		unsigned int second_stop = other.stop().to_integer();

		unsigned int start = first_start < second_start ? second_start : first_start;
		unsigned int stop = first_stop < second_stop ? first_stop : second_stop;

		if( start < stop )
		{
			return 1.0f - ( (float)( stop - start ) * 2.0f ) /
				(float)( ( first_stop - first_start ) + ( second_stop - second_start ) );
		}
		else
		{
			return first_stop < second_start ?
				second_start - first_stop : first_start - second_stop;
		}
	}

} // namespace subfit
