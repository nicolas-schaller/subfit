#ifndef SRT_TIMESTAMP_H
#define SRT_TIMESTAMP_H

#include <stddef.h>
#include "Stream.h"

namespace subfit
{

	struct Timestamp
	{
	public: // Initialization
		Timestamp();

	public: // Command
		bool read( Stream& stream );
		void write( Stream& stream ) const;

	public: // Conversion
		size_t to_integer() const;

	public: // Data
		size_t hour;
		size_t minute;
		size_t second;
		size_t msecond;
	};

} // namespace subfit

#endif // SRT_TIMESTAMP_H
