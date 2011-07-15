#ifndef SRT_TYPE_H
#define SRT_TYPE_H

#include <stddef.h>


namespace subfit
{

	enum Type
	{
		TYPE_BOOL,
		TYPE_CHAR,
		TYPE_UNSIGNED_CHAR,
		TYPE_SIGNED_CHAR,
		TYPE_UNSIGNED_SHORT,
		TYPE_SIGNED_SHORT,
		TYPE_UNSIGNED_LONG,
		TYPE_SIGNED_LONG,
		TYPE_UNSIGNED_LONG_LONG,
		TYPE_SIGNED_LONG_LONG,
		TYPE_UNSIGNED_INT,
		TYPE_SIGNED_INT,
		TYPE_FLOAT,
		TYPE_DOUBLE,
		TYPE_CSTRING
	};

	template< typename T > struct get_type
	{
		static const Type type = TYPE_CSTRING;
	};

	template<> struct get_type< bool > { static const Type type = TYPE_BOOL; };
	template<> struct get_type< char > { static const Type type = TYPE_CHAR; };
	template<> struct get_type< signed char > { static const Type type = TYPE_SIGNED_CHAR; };
	template<> struct get_type< unsigned char > { static const Type type = TYPE_UNSIGNED_CHAR; };
	template<> struct get_type< signed short > { static const Type type = TYPE_SIGNED_SHORT; };
	template<> struct get_type< unsigned short > { static const Type type = TYPE_UNSIGNED_SHORT; };
	template<> struct get_type< signed long > { static const Type type = TYPE_SIGNED_LONG; };
	template<> struct get_type< unsigned long > { static const Type type = TYPE_UNSIGNED_LONG; };
	template<> struct get_type< signed long long > { static const Type type = TYPE_SIGNED_LONG_LONG; };
	template<> struct get_type< unsigned long long > { static const Type type = TYPE_UNSIGNED_LONG_LONG; };
	template<> struct get_type< signed int > { static const Type type = TYPE_SIGNED_INT; };
	template<> struct get_type< unsigned int > { static const Type type = TYPE_UNSIGNED_INT; };
	template<> struct get_type< float > { static const Type type = TYPE_FLOAT; };
	template<> struct get_type< double > { static const Type type = TYPE_DOUBLE; };

	const char* get_type_name( Type type );

} // namespace subfit

#endif // SRT_TYPE_H
