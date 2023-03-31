#include "Core/Memory/memory.hpp"
#include "Core/Debug/debug.hpp"
#include <stdlib.h>

namespace Raindrop::Core::Memory{
	#ifdef RAINDROP_MEMORY_DEBUG
		static uint32 allocationCount = 0;
		static uint32 deallocationCount = 0;
	#endif

	void* allocate(usize size){
		#ifdef RAINDROP_MEMORY_DEBUG
			allocationCount++;
		#endif
		void* ptr = ::malloc(size);
		RAINDROP_cond_log(ptr == nullptr, ERROR, MEMORY, "failed to allocate %d", size);
		return ptr;
	}

	void deallocate(void* mem){
		#ifdef RAINDROP_MEMORY_DEBUG
			deallocationCount++;
		#endif
		::free(mem);
	}
	
	#ifdef RAINDROP_MEMORY_DEBUG
		uint32 getAllocationCount(){
			return allocationCount;
		}

		uint32 getDeallocationCount(){
			return deallocationCount;
		}
	#endif
}