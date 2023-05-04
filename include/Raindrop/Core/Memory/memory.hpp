#ifndef __RAINDROP_CORE_MEMORY_HPP__
#define __RAINDROP_CORE_MEMORY_HPP__

#include <common.hpp>
#include <Core/Memory/containers.hpp>
#include <Core/Memory/pointers.hpp>
#include <Core/Debug/debug.hpp>

namespace Raindrop::Core::Memory{
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
		RAINDROP_assert(align > 0 && "Alignment must be greater than zero");
		usize adjustment = align - (reinterpret_cast<usize>(ptr) + header_size) % align;
		if (adjustment == align) {
			return 0;
		}
		return adjustment;
	}
}

#endif