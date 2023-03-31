#include "Tools/comparator.hpp"
#include <cstring>

namespace Raindrop::Tools{
	template<typename T>
	bool equal(const T& a, const T& b){
		return a == b;
	}

	template<>
	bool equal<const char*>(const char* const& a, const char* const& b){
		return strcmp(a, b) == 0;
	}
}