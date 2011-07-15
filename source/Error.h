#ifndef SUBFIT_ERROR_H
#define SUBFIT_ERROR_H

#include <exception>
#include <ostream>

namespace subfit
{

	class Error : public std::exception
	{
		friend std::ostream& operator<<( std::ostream& ostream, const Error& error );

	private: // Callback
		virtual void write( std::ostream& stream ) const
		{
			stream << what();
		}
	};

	template< typename T >
	class Invalid_Argument_Value : public Error
	{
	public:
		Invalid_Argument_Value(
			const char* function_name, const char* argument_name, const T& value ) :
			function_name_( function_name ),
			argument_name_( argument_name ),
			value_( value )
		{
		}

	private: // Callback
		virtual void write( std::ostream& stream ) const
		{
			stream
				<< "Invalid argument '"
				<< argument_name_
				<< "' value ' "
				<< value_
				<< " in function "
				<< function_name_;
		}

	private: // Data
		const char* function_name_;
		const char* argument_name_;
		T value_;
	};

	inline std::ostream& operator<<( std::ostream& ostream, const Error& error )
	{
		error.write( ostream );

		return ostream;
	}

} // namespace subfit

#endif // SUBFIT_ERROR_H
