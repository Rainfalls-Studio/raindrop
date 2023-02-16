#ifndef __RAINDROP_CORE_MEMORY_HPP__
#define __RAINDROP_CORE_MEMORY_HPP__

#include "../core.hpp"
#include "../common.hpp"

// align the num value to the next greater align multiple
#define RAINDROP_aling_forward(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

namespace Raindrop::Core::Memory{

	/**
	 * @brief  allocate a chunk of 'size' bytes of heap memory
	 * @param size the size of the chunk to allocate
	 * @return return the pointer to the allocated chunk of memory. Return nullptr on error
	 */
	void* allocate(usize size);

	/**
	 * @brief deallocate a chunk of heap memory. Has to be a valid pointer or the app might crash
	 * @param mem a pointer to the memory chunk to deallocate
	 */
	void deallocate(void* mem);

	#ifdef RAINDROP_MEMORY_DEBUG
		uint32 getAllocationCount();
		uint32 getDeallocationCount();
	#endif

	inline void* alignForward(void* address, uint8 alignment){
		return (void*)RAINDROP_aling_forward((uptr)address, alignment);
	}

	inline uint8 alignForwardAdjustment(const void* address, uint8 alignment){ 
		uint8 adjustment = alignment - ((uptr)(address) & (uptr)(alignment-1)); 
		if(adjustment == alignment) return 0; 
		return adjustment; 
	} 

	inline uint8 alignForwardAdjustmentWithHeader(const void* address, uint8 alignment, uint8 headerSize) {
		uint8 adjustment = alignForwardAdjustment(address, alignment);
		uint8 neededSpace = headerSize; 
		
		if(adjustment < neededSpace) {
			neededSpace -= adjustment; 
			
			//Increase adjustment to fit header 
			adjustment += alignment * (neededSpace / alignment); 
			if(neededSpace % alignment > 0) adjustment += alignment;
		}
		
		return adjustment;
	} 
}

#endif