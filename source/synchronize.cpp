#include "Synchronize.h"
#include "Error.h"
#include "Stream.h"
#include "Subtitle.h"
#include "Timerange.h"
#include "Timeranges.h"
#include <iostream>
#include <math.h>

namespace subfit
{

	int statistics(
		size_t input_count, size_t input_lost_count,
		size_t output_count, size_t output_empty_count,
		float heuristic, bool xml_format = false )
	{
		if( xml_format )
		{
			printf( "<input_count value=\"%u\"/>\n", input_count );
			printf( "<input_lost_count value=\"%u\"/>\n", input_lost_count );
			printf( "<output_count value=\"%u\"/>\n", output_count );
			printf( "<output_empty_count value=\"%u\"/>\n", output_empty_count );
			if( input_count > 0 )
				printf( "<average_precision value=\"%.1f\"/>\n", ( 1.0f - ( heuristic / input_count ) ) * 100.0f );
		}
		else
		{
			if( input_count > 0 )
//				printf( "Average precision is %.1f%% for %u entries\n", log( heuristic ) / log( input_count ), input_count );
				printf( "Average precision is %.1f%% for %u entries\n", ( 1.0f - ( heuristic / input_count ) ) * 100.0f, input_count );
			if( output_empty_count > 0 )
				printf( "%u/%u output index empty\n", output_empty_count, output_count );
			if( input_lost_count > 0 )
				printf( "%u/%u input subtitle lost\n", input_lost_count, input_count );
		}

		return 0;
	}

	int synchronize( const Arguments& arguments )
	{
		bool log = arguments["log"].get_value< bool >();
		bool message_xml = arguments["message_xml"].get_value< bool >();
		Stream timing_stream = Stream::open( arguments["timecode"].get_value< const char* >() );
		Timeranges ranges( timing_stream );

		Stream input_stream = Stream::open( arguments["input"].get_value< const char* >() );
		Stream output_stream = Stream::create( arguments["output"].get_value< const char* >(), input_stream.type() );

		Subtitle subtitle;
		Timerange range;

		size_t input_index = 0, output_index = -1, empty_count = 0, lost_count = 0;
		size_t entry_length = 0;
		float heuristic = 0.0f;

		while( !input_stream.end_reached() )
		{
			if( !subtitle.read( input_stream ) )
				break;

			size_t match_index = ranges.search_matching(
				subtitle.range(), output_index != -1 ? output_index : 0 );

			if( match_index != output_index )
			{
				if( output_index != -1 )
				{
					output_stream.write( L"\n" );

					if( entry_length == 0 )
					{
						if( message_xml )
						{
							std::cout
								<< "<warning value=\"output at "
								<< ( output_index + 1 )
								<< " is empty\"/>"
								<< std::endl;
						}
						else
						{
							std::cout
								<< "Warning: output at "
								<< ( output_index + 1 )
								<< " is empty"
								<< std::endl;
						}
					}

				}

				empty_count += match_index - ( output_index + 1 );
				if( arguments["fill_empty"].get_value< bool >() )
				{
					for( int i = output_index + 1; i < match_index; i ++ )
					{
						range = Timerange( ranges[i] );
						output_stream.write( L"%u\n", i + 1 );
						range.write( output_stream );
						output_stream.write( L"%s\n\n", arguments["fill_marker"].get_value< const char* >() );
					}
				}

				output_index = match_index;
				range = Timerange( ranges[output_index] );
				output_stream.write( L"%u\n", output_index + 1 );
				range.write( output_stream );
				entry_length = 0;
			}

			float distance = range.get_distance( subtitle.range() );
			if( distance < arguments["heuristic"].get_value< float >() )
			{
				if( log && distance > arguments["warning"].get_value< float >() )
				{
					if( message_xml )
					{
						std::cout
							<< "<warning value=\"input at "
							<< ( input_index + 1 )
							<< " may not be at the expected timestamp (output at "
							<< ( output_index + 1 )
							<< ")\"/>"
							<< std::endl;
					}
					else
					{
						std::cout
							<< "Warning: input at "
							<< ( input_index + 1 )
							<< " may not be at the expected timestamp (output at "
							<< ( output_index + 1 )
							<< ")"
							<< std::endl;
					}
				}

				entry_length += wcslen( subtitle.text() );
				output_stream.write( L"%ls", subtitle.text() );
			}
			else
			{
				if( log )
				{
					if( message_xml )
					{
						std::cout
							<< "<warning value=\"input at "
							<< ( input_index + 1 )
							<< " is too far and has not been inserted\"/>"
							<< std::endl;
					}
					else
					{
						std::cout
							<< "Warning: input at "
							<< ( input_index + 1 )
							<< " is too far and has not been inserted"
							<< std::endl;
					}
				}

				lost_count++;
			}

			input_index++;
			heuristic += distance < 1.0f ? distance : 1.0f;
		}

		if( arguments["statistics"].get_value< bool >() )
			statistics(
				input_index, lost_count,
				output_index + 1,
				empty_count,
				heuristic,
				arguments["message_xml"].get_value< bool >() );

		return 0;
	}

} // subfit
