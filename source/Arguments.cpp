#include "Arguments.h"
#include "Error.h"
#include <iostream>

namespace subfit
{

	std::ostream& operator<<( std::ostream& stream, const Arguments::Argument& argument )
	{
		stream << "\t" << argument.tag;
		int offset = strlen( argument.tag );
		if( argument.type != get_type< bool >::type)
		{
			stream << "[" << get_type_name( argument.type ) << " VALUE]";
			offset += strlen( get_type_name( argument.type ) ) + 8;
		}
		while( offset++ < 30 ) std::cout << " ";
		stream << argument.description;
		if( argument.data != 0 )
		{
			offset += strlen( argument.description );
			while( offset++ < 72 ) stream << " ";
			stream << "(current = " << argument.data << ")";
		}

		return stream;
	}

	std::ostream& operator<<( std::ostream& stream, const Arguments& arguments )
	{
		stream << std::endl << "Parameters" << std::endl;
		for( size_t i = 0; i < arguments.size(); i++ )
			if( arguments[i].required )
				stream << arguments[i] << std::endl;

		stream << std::endl << "Options" << std::endl;
		for( size_t i = 0; i < arguments.size(); i++ )
			if( !arguments[i].required )
				stream << arguments[i] << std::endl;

		return stream;
	}

	class Arguments::Argument_Missing : public Error
	{
	public: // Initialization
		Argument_Missing( const char* name ) : name_( name ) {}

	private: // Callback
		virtual void write( std::ostream& stream ) const
		{
			stream << "Argument '" << name_ << "' must be provided";
		}

	private: // Data
		const char* name_;
	};

	class Arguments::Argument::Cast_Failed : public Error
	{
	public: // Initialization
		Cast_Failed( const char* data, Type type ) : data_( data ), type_( type ) {}

	private: // Callback
		virtual void write( std::ostream& stream ) const
		{
			stream << "Fail to interpret '" << data_ << "' as " << get_type_name( type_ );
		}

	private: // Data
		const char* data_;
		Type type_;
	};

	Arguments::Argument::Bad_Value_Type::Bad_Value_Type(
		Type actual, Type requested ) :
		actual_( actual ), requested_( requested )
	{
	}

	void Arguments::Argument::Bad_Value_Type::write( std::ostream& stream ) const
	{
		stream
			<< "Value type is "
			<< get_type_name( actual_ )
			<< " whereas "
			<< get_type_name( requested_)
			<< " has been requested";
	}

	void Arguments::parse( int argc, char* const argv[], bool quiet )
	{
		for( size_t i = 0; i < argc; i++ )
		{
			bool found = false;

			for( size_t j = 0; j < size_; j++ )
			{
				Argument& argument = table_[j];
				size_t tag_length = strlen( argument.tag );

				found = !strncmp( argv[i], argument.tag, tag_length );
				if( found )
				{
					argument.data = argument.type != get_type< bool >::type ?
						&argv[i][strlen( argument.tag )] : "true";
					break;
				}
			}

			if( !quiet && !found )
			{
				std::cout << "Unrecognized argument '" << argv[i] << "'" << std::endl;
			}
		}

		for( size_t j = 0; j < size_; j++ )
		{
			Argument& argument = table_[j];

			if( argument.required && argument.data == 0 )
				throw Argument_Missing( argument.name );
		}
	}

	Arguments::Arguments( Argument* table, size_t count ) :
		table_( table ),
		size_( count )
	{
	}

	const Arguments::Argument& Arguments::operator[]( size_t index ) const
	{
		if( index >= size_ ) throw Invalid_Argument_Value< size_t >( __FUNCTION__, "index", index );
		return table_[index];
	}

	const Arguments::Argument& Arguments::operator[]( const char* parameter_name ) const
	{
		for( int i = 0; i < size_; i++ )
		{
			if( !strcmp( parameter_name, table_[i].name ) )
			{
				return table_[i];
			}
		}

		throw Invalid_Argument_Value< const char* >( __FUNCTION__, "parameter_name", parameter_name );
	}

	template<> bool Arguments::Argument::get_value_internal< bool >() const
	{
		return !strcmp( data, "true" );
	}

	template<> char Arguments::Argument::get_value_internal< char >() const
	{
		char value;
		if( sscanf( data, "%c", &value ) != 1 ) throw Cast_Failed( data, type );
		return value;
	}

	template<> signed char Arguments::Argument::get_value_internal< signed char >() const
	{
		signed char value;
		if( sscanf( data, "%d", &value ) != 1 ) throw Cast_Failed( data, type );
		return (signed char)value;
	}

	template<> unsigned char Arguments::Argument::get_value_internal< unsigned char >() const
	{
		unsigned int value;
		if( sscanf( data, "%u", &value ) != 1 ) throw Cast_Failed( data, type );
		return (unsigned char)value;
	}

	template<> signed short Arguments::Argument::get_value_internal< signed short >() const
	{
		signed int value;
		if( sscanf( data, "%d", &value ) != 1 ) throw Cast_Failed( data, type );
		return (signed short)value;
	}

	template<> unsigned short Arguments::Argument::get_value_internal< unsigned short >() const
	{
		unsigned int value;
		if( sscanf( data, "%u", &value ) != 1 ) throw Cast_Failed( data, type );
		return (unsigned short)value;
	}

	template<> signed long Arguments::Argument::get_value_internal< signed long >() const
	{
		signed int value;
		if( sscanf( data, "%d", &value ) != 1 ) throw Cast_Failed( data, type );
		return (signed long)value;
	}

	template<> unsigned long Arguments::Argument::get_value_internal< unsigned long >() const
	{
		unsigned int value;
		if( sscanf( data, "%u", &value ) != 1 ) throw Cast_Failed( data, type );
		return (unsigned long)value;
	}

	template<> signed long long Arguments::Argument::get_value_internal< signed long long >() const
	{
		signed int value;
		if( sscanf( data, "%d", &value ) != 1 ) throw Cast_Failed( data, type );
		return (signed long long)value;
	}

	template<> unsigned long long Arguments::Argument::get_value_internal< unsigned long long >() const
	{
		unsigned int value;
		if( sscanf( data, "%u", &value ) != 1 ) throw Cast_Failed( data, type );
		return (unsigned long long)value;
	}

	template<> signed int Arguments::Argument::get_value_internal< signed int >() const
	{
		signed int value;
		if( sscanf( data, "%u", &value ) != 1 ) throw Cast_Failed( data, type );
		return value;
	}

	template<> unsigned int Arguments::Argument::get_value_internal< unsigned int >() const
	{
		unsigned int value;
		if( sscanf( data, "%u", &value ) != 1 ) throw Cast_Failed( data, type );
		return value;
	}

	template<> float Arguments::Argument::get_value_internal< float >() const
	{
		float value;
		if( sscanf( data, "%f", &value ) != 1 ) throw Cast_Failed( data, type );
		return value;
	}

	template<> double Arguments::Argument::get_value_internal< double >() const
	{
		float value;
		if( sscanf( data, "%u", &value ) != 1 ) throw Cast_Failed( data, type );
		return (double)value;
	}

	template<> const char* Arguments::Argument::get_value_internal< const char* >() const
	{
		return data;
	}

} // namespace subfit
