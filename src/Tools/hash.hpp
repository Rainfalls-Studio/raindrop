#ifndef __RAINDROP_UTILS_HASH_HPP__
#define __RAINDROP_UTILS_HASH_HPP__

#include "../Core/core.hpp"
#include "../Core/common.hpp"

namespace Raindrop::Tools{
	
	template<typename T>
	ID64 hash64(const T& t);

	template<>
	ID64 hash64<const unsigned char*>(const unsigned char* const& t);

	template<>
	ID64 hash64<const char*>(const char* const& t);
}

#endif