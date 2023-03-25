#include "LinearAllocator.hpp"
#include "../memory.hpp"
#include "../../Debug/profiler.hpp"

namespace Raindrop::Core::Memory{
	LinearAllocator::LinearAllocator(usize size, void* start) : Allocator(size, start){
		RAINDROP_profile_function();
		pos = start;
	}

	LinearAllocator::~LinearAllocator(){
		RAINDROP_profile_function();
		pos = nullptr;
	}

	void* LinearAllocator::allocate(usize size, uint8 alignment){
		RAINDROP_profile_function();
		RAINDROP_assert(size != 0); 
		usize adjustment = alignAdjustment(pos, alignment); 
		
		if(usedMemory + adjustment + size > this->size){
			RAINDROP_log(ERROR, MEMORY, "not enought memory in the linear allocator");
			return nullptr; 
		}
		
		uptr aligned_address = (uptr)pos + adjustment; 
		pos = (void*)(aligned_address + size); 
		usedMemory += size + adjustment; 
		allocationCount++;

		return (void*)aligned_address;
	}

	void LinearAllocator::deallocate(void* p){
		RAINDROP_profile_function();
		// RAINDROP_log(WARNING, MEMORY, "linear allocator does not support deallocate, use clear instead", "");
	}

	void LinearAllocator::clear(){
		RAINDROP_profile_function();
		allocationCount = 0;
		usedMemory = 0;
		pos = start;
	}
}