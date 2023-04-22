#include <Tools/comparator.hpp>
#include <cstring>

namespace Raindrop::Tools{
	template<typename T>
	RAINDROP_API bool equal(const T& a, const T& b){
		return a == b;
	}

	template<>
	RAINDROP_API bool equal<const char*>(const char* const& a, const char* const& b){
		return strcmp(a, b) == 0;
	}
}