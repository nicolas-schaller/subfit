#include "Error.h"
#include "Arguments.h"
#include "synchronize.h"
#include "convert.h"
#include "help.h"
#include <iostream>

	using namespace subfit;

	int main( int argc, char* const argv[] )
	{
		try
		{
			typedef int (*Callback)( const Arguments& arguments );
			Callback callback_list[] = { &synchronize, &convert, &help };

			Arguments::Argument command_list[] =
			{
				{ "sync",					"perform timestamp synchronization",			get_type< bool >::type,					"sync",					false,	"false" },
				{ "conv",					"perform format conversion",							get_type< bool >::type,					"conv",					false,	"false" },
				{ "help",					"display help",														get_type< bool >::type,					"help",					false,	"false" }
			};

			Arguments::Argument parameter_list[] =
			{
				{ "timecode",				"set file used for timecode reference",		get_type< const char* >::type,	"-timecode:",				true,		0 },
				{ "input",					"set file used to get subtitles from",		get_type< const char* >::type,	"-input:",					true,		0 },
				{ "output",					"set file used to store the result",			get_type< const char* >::type,	"-output:",					true,		0 },
				{ "output_format",	"set output format",											get_type< const char* >::type,	"-output_format:",	true,		"subrip" },
				{ "heuristic",			"specify heuristic threshold",						get_type< float >::type,				"-heuristic:",			false,	"1.0" },
				{ "warning",				"specify warning threshold",							get_type< float >::type,				"-warning:",				false,	"1.0" },
				{ "fill_empty",			"fill empty output index with marker",		get_type< bool >::type,					"-fill_empty",			false,	"false" },
				{ "fill_marker",		"marker used when fill_empty is enabled",	get_type< const char* >::type,	"-fill_marker:",		false,	"[?]" },
				{ "log",						"print process's execution messages",			get_type< bool >::type,					"-log",							false,	"false" },
				{ "statistics",			"print statistics",												get_type< bool >::type,					"-statistics",			false,	"false" },
				{ "message_xml",		"format messages into xml",								get_type< bool >::type,					"-message_xml",			false,	"false" }
			};
			Arguments commands( command_list, sizeof( command_list ) / sizeof( Arguments::Argument ) );

			static const size_t default_action = 0;
			size_t command_index = default_action;
			size_t arg_offset = 1;

			if( argc > 0 )
			{
				commands.parse( 1, argv + arg_offset, true );
				while( command_index < commands.size() )
				{
					if( commands[command_index].get_value< bool >() )
						break;
					 command_index++;
				}
			}

			if( command_index == commands.size() )
				command_index = default_action;
			else
				arg_offset++;

			Arguments parameters( parameter_list, sizeof( parameter_list ) / sizeof( Arguments::Argument ) );

			parameters.parse( argc - arg_offset, argv + arg_offset, !commands[command_index].required );

			callback_list[command_index]( parameters );
		}
		catch( Error& error )
		{
			std::cerr << "Error: " << error << std::endl;
		}

		return 0;
	}
