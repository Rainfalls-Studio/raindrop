#ifndef __RAINDROP_UTILS_HASH_HPP__
#define __RAINDROP_UTILS_HASH_HPP__

#include <common.hpp>

namespace Raindrop::Tools{
	
	template<typename T>
	RAINDROP_API ID64 hash64(const T& t);

	template<>
	RAINDROP_API ID64 hash64<const unsigned char*>(const unsigned char* const& t);

	template<>
	RAINDROP_API ID64 hash64<const char*>(const char* const& t);
}

#endif