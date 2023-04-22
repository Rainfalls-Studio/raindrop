#include "Core/Memory/allocators/ProxyAllocator.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Memory{
	RAINDROP_API ProxyAllocator::ProxyAllocator(Allocator& allocator) : Allocator(allocator.getSize(), allocator.getStart()), allocator(allocator){
		RAINDROP_profile_function();
	}

	RAINDROP_API ProxyAllocator::~ProxyAllocator(){
		RAINDROP_profile_function();
	}

	RAINDROP_API void* ProxyAllocator::allocate(usize size, uint8 alignment){
		RAINDROP_profile_function();
		RAINDROP_assert(size != 0); 
		allocationCount++; 
		usize mem = allocator.getUsedMemory(); 

		void* p = allocator.allocate(size, alignment);
		usedMemory += allocator.getUsedMemory() - mem;
		return p; 
	}

	RAINDROP_API void ProxyAllocator::deallocate(void* p){
		RAINDROP_profile_function();
		allocationCount--; 
		size_t mem = allocator.getUsedMemory(); 
		allocator.deallocate(p); 
		usedMemory -= mem - allocator.getUsedMemory();
	}
}