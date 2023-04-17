#ifndef __RAINDROP_CORE_TIME_TIMEPOINT_HPP__
#define __RAINDROP_CORE_TIME_TIMEPOINT_HPP__

#include <Core/Time/common.hpp>
#include <Core/Time/Time.hpp>
#include <Core/Time/Duration.hpp>

namespace Raindrop::Core::Time{
	class RAINDROP_API Timepoint{
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
			tick time() const;

			/**
			 * @brief represent the duration between this timepoint and another one
			 * @param other the other timepoint to compare to
			 * @return Duration 
			 */
			Duration operator-(const Timepoint &other);

		private:
			tick _time;

			Timepoint(tick time);
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