#ifndef __RAINDROP_CORE_TIME_TIME_HPP__
#define __RAINDROP_CORE_TIME_TIME_HPP__

#include <Core/Time/common.hpp>

namespace Raindrop::Core::Time{
	// "Pauses" the current thread for <time> milliseconds
	RAINDROP_API void sleep(tick time);

	// Returns the ammount of ticks elapsed since the execution of the program.
	RAINDROP_API tick ticks();
}

#endif