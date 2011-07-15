#include "Timeranges.h"
#include "Subtitle.h"
#include <float.h>

namespace subfit
{

	class Timeranges::Read_Subtitle_Failed : public Error
	{
	public: // Initialization
		Read_Subtitle_Failed( size_t index ) :
			index_( index )
		{
		}

	private: // Callback
		virtual void write( std::ostream& stream ) const
		{
			stream << "Fail to read timestamp entry " << index_;
		}

	private: // Data
		size_t index_;
	};

	Timeranges::Timeranges( Stream& stream )
	{
		size_t index = 1;
		while( !stream.end_reached() )
		{
			Subtitle subtitle;

			if( index == 43 )
			{
				index = index == 43 ? index : 0;
			}

			if( !subtitle.read( stream ) )
			{
				if( !stream.end_reached() )
					throw Read_Subtitle_Failed( index );

				break;
			}

			ranges_.push_back( subtitle.range() );
			index++;
		}
	}

	const Timerange& Timeranges::operator[]( size_t index ) const
	{
		if( index >= ranges_.size() )
			throw Invalid_Argument_Value< size_t >( __FUNCTION__, "start_index", index );

		return ranges_[index];
	}

	size_t Timeranges::search_matching( const Timerange& range, size_t index )
	{
		if( index >= ranges_.size() )
			throw Invalid_Argument_Value< size_t >( __FUNCTION__, "start_index", index );

		size_t best_distance = FLT_MAX;
		size_t best_index = index;

		while( index < ranges_.size() )
		{
			float distance = ranges_[index].get_distance( range );
			if( distance < best_distance )
			{
				best_distance = distance;
				best_index = index;
			}

			if( distance > 1.0f )
			{
				if( distance > best_distance )
					break;

				index++;
			}
			else
			{
				if( index > 0 )
				{
					float before_distance = ranges_[index - 1].get_distance( range );

					if( before_distance < distance )
						index--;
				}

				if( index < ranges_.size() - 1 )
				{
					float after_distance = ranges_[index + 1].get_distance( range );

					if( after_distance < distance )
						index++;
				}

				best_index = index;

				break;
			}
		}

		return best_index;
	}

} // namespace subfit
