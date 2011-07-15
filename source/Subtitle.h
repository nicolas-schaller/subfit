#ifndef SRT_SUBTITLE_H
#define SRT_SUBTITLE_H

#include "Timerange.h"
#include "Stream.h"

namespace subfit
{

	struct Subtitle
	{
	public: // Command
		bool read( Stream& stream );

	public: // Status
		const Timerange& range() const { return range_; }
		const wchar_t* text() const { return text_; }

	private: // Helper
		bool is_empty( const wchar_t* text, size_t length ) const;

	private: // Data
		wchar_t text_[1024];
		Timerange range_;
	};

} // namespace subfit

#endif // SRT_SUBTITLE_H
