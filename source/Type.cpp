#include "Type.h"
#include "Error.h"

namespace subfit
{

	const char* get_type_name( Type type )
	{
		const char* name = "unknown type";

		switch( type )
		{
			case TYPE_BOOL               : name = "BOOL"; break;
			case TYPE_CHAR               : name = "CHAR"; break;
			case TYPE_UNSIGNED_CHAR      : name = "UNSIGNED CHAR"; break;
			case TYPE_SIGNED_CHAR        : name = "SIGNED CHAR"; break;
			case TYPE_UNSIGNED_SHORT     : name = "UNSIGNED SHORT"; break;
			case TYPE_SIGNED_SHORT       : name = "SIGNED SHORT"; break;
			case TYPE_UNSIGNED_LONG      : name = "UNSIGNED LONG"; break;
			case TYPE_SIGNED_LONG        : name = "SIGNED LONG"; break;
			case TYPE_UNSIGNED_LONG_LONG : name = "UNSIGNED LONG LONG"; break;
			case TYPE_SIGNED_LONG_LONG   : name = "SIGNED LONG LONG"; break;
			case TYPE_UNSIGNED_INT       : name = "UNSIGNED INT"; break;
			case TYPE_SIGNED_INT         : name = "SIGNED INT"; break;
			case TYPE_FLOAT              : name = "FLOAT"; break;
			case TYPE_DOUBLE             : name = "DOUBLE"; break;
			case TYPE_CSTRING            : name = "STRING"; break;
			default:
				throw Invalid_Argument_Value< Type >( __FUNCTION__, "type", type );
		}

		return name;
	}

} // namespace subfit
