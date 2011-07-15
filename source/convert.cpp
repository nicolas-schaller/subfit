#include "convert.h"
#include "Stream.h"
#include "Subtitle.h"
#include "Timerange.h"
#include "Timeranges.h"
#include "Format.h"
#include "Error.h"
#include <iostream>

namespace subfit
{

	int convert( const Arguments& arguments )
	{
		Format output_format = format_code( arguments["output_format"].get_value< const char* >() );

		Stream input_stream = Stream::open( arguments["input"].get_value< const char* >() );
		Stream output_stream = Stream::create( arguments["output"].get_value< const char* >(), input_stream.type() );

		Subtitle subtitle;
		Timerange previous_range;
		size_t index = 0;

		while( !input_stream.end_reached() )
		{
			previous_range = subtitle.range();

			if( !subtitle.read( input_stream ) )
				break;

			index++;

			switch( output_format )
			{
				case FORMAT_SUBRIP :
					output_stream.write( L"%u\n", index );
					subtitle.range().write( output_stream );
					output_stream.write( L"%ls\n", subtitle.text() );
					break;

				case FORMAT_GOOGLE :
					if( previous_range.stop().to_integer() < subtitle.range().start().to_integer() )
					{
						if( index > 1 )
							output_stream.write( L" \n" );
						
						subtitle.range().start().write( output_stream );
					}
					output_stream.write( L"%ls", subtitle.text() );
					subtitle.range().stop().write( output_stream );
					break;
			}
		}

		return 0;
	}

} // subfit
