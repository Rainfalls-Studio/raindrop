#ifndef __RAINDROP_CORE_TIME_DURATION_HPP__
#define __RAINDROP_CORE_TIME_DURATION_HPP__

#include "Core/Time/Time.hpp"

namespace Raindrop::Core::Time{
	class RAINDROP_API Duration{
		public:
			Duration(time duration);
			Duration(const Duration &other);
			Duration(Duration &&other);

			/**
			 * @brief get the duration as a tick value
			 * @return time 
			 */
			time duration() const;

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
			time _duration;
	};
}

#endif