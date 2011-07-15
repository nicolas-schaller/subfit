#ifndef SUBFIT_FORMAT_H
#define SUBFIT_FORMAT_H

#include "Arguments.h"

namespace subfit
{

	enum Format
	{
		FORMAT_SUBRIP,
		FORMAT_GOOGLE,
		FORMAT_COUNT
	};

	Format format_code( const char* name );
	const char* format_name( Format format );

} // subfit

#endif // SUBFIT_FORMAT_H
