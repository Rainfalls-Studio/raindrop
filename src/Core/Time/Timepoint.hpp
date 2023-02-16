#ifndef __RAINDROP_CORE_TIME_TIMEPOINT_HPP__
#define __RAINDROP_CORE_TIME_TIMEPOINT_HPP__

#include "Time.hpp"
#include "Duration.hpp"

namespace Raindrop::Core::Time{
	class Timepoint{
		public:
			/**
			 * @brief construct a new Timepoint object from another one
			 * @param other the timepoint to copy
			 */
			Timepoint(const Timepoint &other);

			/**
			 * @brief construct a new Timepoint object from another one
			 * @param other the timepoint to copy
			 */
			Timepoint(Timepoint &&other);
			~Timepoint();

			/**
			 * @brief construct a new Timepoint representing the current point in time.
			 * @return Timepoint 
			 */
			static Timepoint now();

			/**
			 * @brief the amount of ticks 
			 * @return utime64 
			 */
			utime64 time() const;

			/**
			 * @brief represent the duration between this timepoint and another one
			 * @param other the other timepoint to compare to
			 * @return Duration 
			 */
			Duration operator-(const Timepoint &other);

			/**
			 * @brief convert this timepoint into nanoseconds
			 * @return timef64 
			 */
			timef64 toNanoseconds() const;

			/**
			 * @brief convert this timepoint into microseconds
			 * @return timef64 
			 */
			timef64 toMicroseconds() const;

			/**
			 * @brief convert this timepoint into milliseconds
			 * @return timef64 
			 */
			timef64 toMilliseconds() const;

			/**
			 * @brief convert this timepoint into seconds
			 * @return timef64 
			 */
			timef64 toSeconds() const;

			/**
			 * @brief convert this timepoint into minutes
			 * @return timef64 
			 */
			timef64 toMinutes() const;

			/**
			 * @brief convert this timepoint into hours
			 * @return timef64 
			 */
			timef64 toHours() const;

			/**
			 * @brief convert this timepoint into days
			 * @return timef64 
			 */
			timef64 toDays() const;

		private:
			utime64 _time;

			Timepoint(utime64 time);
	};

	/**
	 * @brief construct a new Timepoint representing the current point in time.
	 * @return Timepoint 
	 */
	inline Timepoint now(){
		return Timepoint::now();
	}
}

#endif