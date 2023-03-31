#ifndef __RAINDROP_CORE_MEMORY_HPP__
#define __RAINDROP_CORE_MEMORY_HPP__

#include "Core/core.hpp"
#include "Core/common.hpp"
#include "Core/Debug/debug.hpp"

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

	inline usize alignSize(usize size, usize align) {
		RAINDROP_assert(align > 0 && "alignment must be greater than zero");

		const usize alignment_mask = align - 1;
		usize aligned_size = (size + alignment_mask) & ~alignment_mask;

		return aligned_size;
	}

	inline void* alignPtr(const void* ptr, usize align) {
		RAINDROP_assert(align > 0 && "alignment must be greater than zero");

		const usize alignment_mask = align - 1;
		void* aligned_ptr = reinterpret_cast<void*>((reinterpret_cast<usize>(ptr) + alignment_mask) & ~alignment_mask);

		return aligned_ptr;
	}

	inline usize alignAdjustment(const void* ptr, usize align) {
		RAINDROP_assert(align > 0 && "Alignment must be greater than zero");

		const usize alignment_mask = align - 1;
		usize adjustment = (reinterpret_cast<usize>(ptr) + alignment_mask) & ~alignment_mask;
		adjustment -= reinterpret_cast<usize>(ptr);

		return adjustment;
	}

	inline usize alignAdjustmentHeader(const void* ptr, usize align, usize header_size) {
		usize adjustment = align - (reinterpret_cast<usize>(ptr) + header_size) % align;
		if (adjustment == align) {
			return 0;
		}
		return adjustment;
	}
}

#endif