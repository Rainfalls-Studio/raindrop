#ifndef __RAINDROP_CORE_DEBUG_DEBUG_HPP__
#define __RAINDROP_CORE_DEBUG_DEBUG_HPP__

#include "logger.hpp"
#include "stdlib.h"

#ifdef RAINDROP_ASSERTS
	#define RAINDROP_assert(x) if (!(x)){RAINDROP_log(ERROR, ASSERT, #x, "assert failed"); exit(1);}
#else
	#define RAINDROP_assert(x)
#endif

#endif