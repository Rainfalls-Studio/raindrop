#include "Core/Memory/allocators/PoolAllocator.hpp"
#include "Core/Memory/memory.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Memory{
	PoolAllocator::PoolAllocator(size_t objectSize, uint8 objectAlignment, usize size, void* mem) : Allocator(size, mem), _objectSize(objectSize), _objectAlignment(objectAlignment){
		RAINDROP_profile_function();
		RAINDROP_assert(objectSize >= sizeof(void*)); 
		
		//Calculate adjustment needed to keep object correctly aligned 
		uint8 adjustment = alignAdjustment(mem, objectAlignment); 
		freeList = (void**)((uptr)mem + adjustment); 
		size_t numObjects = (size-adjustment)/objectSize; 
		void** p = freeList; 
		
		//Initialize free blocks list 
		for(size_t i = 0; i < numObjects-1; i++){
			*p = (void*)((uptr)p + objectSize);
			p = (void**)*p;
		}
		
		*p = nullptr;
	}

	PoolAllocator::~PoolAllocator(){
		RAINDROP_profile_function();
		freeList = nullptr;
		_objectAlignment = 0;
		_objectSize = 0;
	}

	void* PoolAllocator::allocate(usize size, uint8 alignment){
		RAINDROP_profile_function();
		RAINDROP_assert(size == _objectSize); 
		RAINDROP_assert(alignment == _objectAlignment);

		if(freeList == nullptr) return nullptr; 
		void* p = freeList;
		freeList = (void**)(*freeList);
		usedMemory += size;
		allocationCount++; 
		return p; 
	}

	void PoolAllocator::deallocate(void* p){
		RAINDROP_profile_function();
		*((void**)p) = freeList; 
		freeList = (void**)p; 
		usedMemory -= _objectSize; 
		allocationCount--;
	}
}