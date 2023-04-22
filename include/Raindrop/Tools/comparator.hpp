#ifndef __RAINDROP_UTILS_COMPArATOR_HPP__
#define __RAINDROP_UTILS_COMPArATOR_HPP__

#include <common.hpp>

namespace Raindrop::Tools{
	template<typename T>
	RAINDROP_API bool equal(const T& a, const T& b);

	template<>
	RAINDROP_API bool equal<const char*>(const char* const& a, const char* const& b);
}

#endif