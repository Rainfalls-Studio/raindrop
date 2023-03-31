#ifndef __RAINDROP_UTILS_COMPArATOR_HPP__
#define __RAINDROP_UTILS_COMPArATOR_HPP__

#include "Core/core.hpp"
#include "Core/common.hpp"

namespace Raindrop::Tools{
	template<typename T>
	bool equal(const T& a, const T& b);

	template<>
	bool equal<const char*>(const char* const& a, const char* const& b);
}

#endif