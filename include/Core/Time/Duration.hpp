#ifndef __RAINDROP_CORE_TIME_DURATION_HPP__
#define __RAINDROP_CORE_TIME_DURATION_HPP__

#include "Core/Time/Time.hpp"

namespace Raindrop::Core::Time{
	class Duration{
		public:
			Duration(time64 duration);
			Duration(const Duration &other);
			Duration(Duration &&other);

			/**
			 * @brief get the duration as a tick value
			 * @return time64 
			 */
			time64 duration() const;

			/**
			 * @brief convert the duration into nanoseconds
			 * @return timef64 
			 */
			timef64 toNanoseconds() const;

			/**
			 * @brief convert the duration into microseconds
			 * @return timef64 
			 */
			timef64 toMicroseconds() const;

			/**
			 * @brief convert the duration into milliseconds
			 * @return timef64 
			 */
			timef64 toMilliseconds() const;

			/**
			 * @brief convert the duration into seconds
			 * @return timef64 
			 */
			timef64 toSeconds() const;

			/**
			 * @brief convert the duration into minutes
			 * @return timef64 
			 */
			timef64 toMinutes() const;

			/**
			 * @brief convert the duration into hours
			 * @return timef64 
			 */
			timef64 toHours() const;

			/**
			 * @brief convert the duration into days
			 * @return timef64 
			 */
			timef64 toDays() const;

			/**
			 * @brief create a new duration from this with a multiplication factor of 'x'
			 * @param x coeficient of the product
			 * @return Duration 
			 */
			Duration operator*(int32 x);
			
			/**
			 * @brief create a new duration from this with a division factor of 'x'
			 * @param x coeficient of the product
			 * @return Duration 
			 */
			Duration operator/(int32 x);

			/**
			 * @brief create a new duration from the addition between this and 'other'
			 * @param other 
			 * @return Duration 
			 */
			Duration operator+(const Duration &other);

			/**
			 * @brief create a new duration from the difference between this and 'other'
			 * @param other 
			 * @return Duration 
			 */
			Duration operator-(const Duration &other);

			Duration& operator*=(int32 x);
			Duration& operator/=(int32 x);
			Duration& operator+=(const Duration &other);
			Duration& operator-=(const Duration &other);
		
		private:
			time64 _duration;
	};
}

#endif