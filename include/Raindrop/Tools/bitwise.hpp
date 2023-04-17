#ifndef __RAINDROP_TOOLS_BITWISE_HPP__
#define __RAINDROP_TOOLS_BITWISE_HPP__

#include <common.hpp>

namespace Raindrop::Tools{

	// return the index of the first positive bit, (uint32)-1 if none
	template<typename T>
	inline uint32 getShiftOffset(T t){
		usize size = sizeof(T) * 8;
		for (usize i=0; i<size; i++){
			if ((t >> i) & 1ULL) return i;
		}
		return -1;
	}
}

#endif