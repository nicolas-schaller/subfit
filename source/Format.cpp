#include "Format.h"
#include "Error.h"
#include <string.h>

namespace subfit
{

	static const char* FORMAT_NAME[] =
	{
		"subrip",
		"google"
	};

	Format format_code( const char* name )
	{
		for( size_t i = 0; i < sizeof( FORMAT_NAME ) / sizeof( char* ); i ++ )
		{
			if( !strcmp( name, FORMAT_NAME[i] ) )
				return (Format)i;
		}

		return FORMAT_COUNT;
	}

	const char* format_name( Format format )
	{
		if( (size_t)format >= FORMAT_COUNT )
			throw Invalid_Argument_Value< Format >( __FUNCTION__, "format", format );

		return FORMAT_NAME[(size_t)format];
	}

} // subfit
