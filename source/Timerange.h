#ifndef SRT_TIMERANGE_H
#define SRT_TIMERANGE_H

#include "Timestamp.h"
#include "Stream.h"

namespace subfit
{

	class Timerange
	{
	public: // Initialization
		Timerange();
		Timerange( const Timerange& other );

	public: // Status
		const Timestamp& start() const { return start_; }
		const Timestamp& stop() const { return stop_; }
		float get_distance( const Timerange& other ) const;

	public: // Command
		bool read( Stream& stream );
		void write( Stream& stream ) const;

	private: // Data
		Timestamp start_;
		Timestamp stop_;
	};

} // namespace subfit

#endif // SRT_TIMERANGE_H
