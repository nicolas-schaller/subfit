#ifndef SRT_TIMERANGES_H
#define SRT_TIMERANGES_H

#include "Stream.h"
#include "Timerange.h"
#include <vector>

namespace subfit
{

	class Timeranges
	{
	public: // Type definition
		class Read_Subtitle_Failed;

	public: // Initialization
		Timeranges( Stream& stream );

	public: // Status
		const Timerange& operator[]( size_t index ) const;
		size_t search_matching( const Timerange& timerange, size_t start_index = 0 );

	private: // Data
		std::vector< Timerange > ranges_;
	};

} // namespace subfit

#endif // SRT_TIMERANGES_H
