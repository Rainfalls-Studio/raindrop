#include "Core/Memory/allocators/StackAllocator.hpp"
#include "Core/Memory/memory.hpp"
#include "Core/Debug/logger.hpp"

namespace Raindrop::Core::Memory{
	StackAllocator::StackAllocator(usize size, void* start) : Allocator(size, start){
		RAINDROP_profile_function();
		pos = start;
		
		#ifdef _DEBUG
			previousPos = nullptr;
		#endif
	}

	StackAllocator::~StackAllocator(){
		pos = nullptr;
		
		#ifdef _DEBUG
			previousPos = nullptr;
		#endif
	}

	void* StackAllocator::allocate(usize size, uint8 alignment){
		RAINDROP_profile_function();
		RAINDROP_assert(size != 0);

		uint8 adjustment = alignAdjustmentHeader(pos, alignment, sizeof(AllocationHeader)); 
  
  		if(usedMemory + adjustment + size > this->size) return nullptr; 
		
		void* aligned_address = (void*)((uptr)pos + adjustment); 
		
		// Add Allocation Header 
		AllocationHeader* header = (AllocationHeader*)((uptr)aligned_address - sizeof(AllocationHeader)); 
		header->adjustment = adjustment; 

		#if _DEBUG 
			header->previousAdress = previousPos;
			previousPos = aligned_address; 
		#endif 
  
		
		pos = (void*)((uptr)aligned_address + size); 
		usedMemory += size + adjustment; 
		allocationCount++; 
		
		return aligned_address; 
	}

	void StackAllocator::deallocate(void* p){
		RAINDROP_profile_function();
		#if _DEBUG
			RAINDROP_assert(p != previousPos); 
		#endif
  
		//Access the AllocationHeader in the bytes before p 
		AllocationHeader* header = (AllocationHeader*)((uptr)p - sizeof(AllocationHeader)); 
		usedMemory -= (uptr)pos - (uptr)p + header->adjustment; 
		pos = (void*)((uptr)p - header->adjustment); 
		
		#if _DEBUG 
			previousPos = header->previousAdress; 
		#endif 
		
		allocationCount--;
	}

	void StackAllocator::clear(){
		RAINDROP_profile_function();
		allocationCount = 0;
		usedMemory = 0;
		pos = start;

		#if _DEBUG
			previousPos = nullptr;
		#endif
	}
}