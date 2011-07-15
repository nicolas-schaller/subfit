#ifndef SUBFIT_ARGUMENTS_H
#define SUBFIT_ARGUMENTS_H

#include <stddef.h>
#include <ostream>
#include "Type.h"
#include "Error.h"

namespace subfit
{

	class Arguments
	{
	public: // Type definition
		struct Argument
		{
		public: // Type definition;
			class Bad_Value_Type : public Error
			{
			public: // Initialization
				Bad_Value_Type( Type actual, Type requested );

			private: // Callback
				virtual void write( std::ostream& stream ) const;

			private: // Data
				Type actual_;
				Type requested_;
			};

			class Cast_Failed;

		public: // Status
			template< typename T > T get_value() const
			{
				if( type != get_type< T >::type ) throw Bad_Value_Type( type, get_type< T >::type );
				return get_value_internal< T >();
			}

		public: // Relation
			friend std::ostream& operator<<( std::ostream& stream, const Argument& argument );

		public: // Data
			const char* name;
			const char* description;
			Type type;
			const char* tag;
			const bool required;
			const char* data;

		private: // Helper
			template< typename T > T get_value_internal() const;
		};

		class Argument_Missing;

	public: // Construction
		Arguments( Argument list[], size_t count );

	public: // Status
		size_t size() const { return size_; }
		const Argument& operator[]( size_t index ) const;
		const Argument& operator[]( const char* argument_name ) const;

	public: // Command
		void parse( int argc, char* const argv[], bool quiet = false );

	public: // Command
		friend std::ostream& operator<<( std::ostream& stream, const Arguments& arguments );

	private: // Data
		Argument* table_;
		size_t size_;
	};

} // namespace subfit

#endif // SUBFIT_ARGUMENTS_H
