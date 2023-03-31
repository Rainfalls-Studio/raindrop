#ifndef __RAINDROP_CORE_TIME_TIME_HPP__
#define __RAINDROP_CORE_TIME_TIME_HPP__

#include "Core/core.hpp"
#include "Core/common.hpp"

namespace Raindrop::Core::Time{
	typedef int64 time64;
	typedef uint64 utime64;
	typedef float64 timef64;

	struct Date{
		utime64 y;
		utime64 d;
		utime64 s;
	};
	
	/**
	 * @brief stop the current thread for 'time' microseconds;
	 * @param time the time to stop for
	 */
	void sleep(utime64 time);

	/**
	 * @brief get the amount of ticks spent since the begingin of the program
	 * @return utime64 
	 */
	utime64 getTicks();

	/**
	 * @brief return the amount of ticks in one seconds
	 * @return utime64 
	 */
	utime64 getTicksPerSeconds();
	
	/**
	 * @brief convert ticks amount into nanoseconds
	 * @param time the tick amoount to convert
	 * @return timef64 
	 */
	timef64 toNanoseconds(time64 time);
	
	/**
	 * @brief convert ticks amount into microseconds
	 * @param time the tick amount to convert
	 * @return timef64 
	 */
	timef64 toMicroseconds(time64 time);
	
	/**
	 * @brief convert ticks amount into milliseconds
	 * @param time the tick amount to convert
	 * @return timef64 
	 */
	timef64 toMilliseconds(time64 time);
	
	/**
	 * @brief convert ticks amount into seconds
	 * @param time the tick amoount to convert
	 * @return timef64 
	 */
	timef64 toSeconds(time64 time);
	
	/**
	 * @brief convert ticks amount into minutes
	 * @param time the tick amoount to convert
	 * @return timef64 
	 */
	timef64 toMinutes(time64 time);
	
	/**
	 * @brief convert ticks amount into hours
	 * @param time the tick amount to convert
	 * @return timef64 
	 */
	timef64 toHours(time64 time);
	
	/**
	 * @brief convert ticks amount into days
	 * @param time the tick amount to convert
	 * @return timef64 
	 */
	timef64 toDays(time64 time);

	time64 operator ""_ns(timef64 time);
	time64 operator ""_mcs(timef64 time);
	time64 operator ""_ms(timef64 time);
	time64 operator ""_s(timef64 time);
	time64 operator ""_m(timef64 time);
	time64 operator ""_h(timef64 time);
	time64 operator ""_d(timef64 time);
	time64 operator ""_ns(utime64 time);
	time64 operator ""_mcs(utime64 time);
	time64 operator ""_ms(utime64 time);
	time64 operator ""_s(utime64 time);
	time64 operator ""_m(utime64 time);
	time64 operator ""_h(utime64 time);
	time64 operator ""_d(utime64 time);
}

#endif