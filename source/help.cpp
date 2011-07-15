#include "help.h"
#include "Error.h"
#include <iostream>

namespace subfit
{

	int help( const Arguments& arguments )
	{
		std::cout << std::endl << "Usage" << std::endl;
		std::cout << "\tsubfit [COMMAND|] -timecode:[FILE_PATH] -input:[FILE_PATH] -output:[FILE_PATH]" << std::endl;

		std::cout << arguments << std::endl;

		return 0;
	}

} // subfit
