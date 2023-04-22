#include "Core/Memory/allocators/FreelistAllocator.hpp"
#include "Core/Memory/memory.hpp"
#include "Core/Debug/debug.hpp"
#include "Core/Debug/profiler.hpp"
#include "Core/Maths/Maths.hpp"

namespace Raindrop::Core::Memory{
	RAINDROP_API FreelistAllocator::FreelistAllocator(usize size, void* start) :
		Allocator(size, start),
		start_(start),
		size_(size),
		free_list_((Block*)start){
		RAINDROP_assert(size > sizeof(Block));

		free_list_->size = size;
		free_list_->next = nullptr;
	}

	RAINDROP_API FreelistAllocator::~FreelistAllocator(){
		free_list_ = nullptr;
		start_ = nullptr;
		size_ = 0;
	}

	RAINDROP_API void* FreelistAllocator::allocate(usize size, uint8 alignment) {
		if (size == 0) throw std::invalid_argument("cannot allocate an element with a null size");
		if (alignment == 0) throw std::invalid_argument("cannot allocate an element without a valid alignement");
		if (!Maths::isPowerOfTwo(alignment)) throw std::invalid_argument("the alignement has to be a power of two");


		Block* prev_free_block = nullptr;
		Block* free_block = free_list_;

		// Find a free block that is large enough and satisfies the alignment requirement
		while (free_block != nullptr) {
			usize adjustment = alignAdjustmentHeader(free_block, alignment, sizeof(Header));
			usize total_size = size + adjustment + sizeof(Header);

			if (free_block->size < total_size) {
				prev_free_block = free_block;
				free_block = free_block->next;
				continue;
			}

			// If the remaining size of the free block after allocation is too small to hold another block, then allocate the entire block
			if (free_block->size - total_size <= sizeof(Block)) {
				if (prev_free_block != nullptr) {
					prev_free_block->next = free_block->next;
				} else {
					free_list_ = free_block->next;
				}
				// Include the header adjustment in the returned pointer
				void* aligned_address = (void*)((uptr)free_block + adjustment);
				Header* header = (Header*)((uptr)aligned_address - sizeof(Header));
				header->size = size;
				header->alignment = adjustment;
				usedMemory += total_size;
				allocationCount++;

				// Update the next pointer of the allocated block to point to the next free block in the list
				Block* next_free_block = free_block->next;
				*((Block**)aligned_address - 1) = next_free_block;

				return aligned_address;
			} else {
				Block* next_block = (Block*)((uptr)free_block + total_size);
				next_block->size = free_block->size - total_size;
				next_block->next = free_block->next;

				if (prev_free_block != nullptr) {
					prev_free_block->next = next_block;
				} else {
					free_list_ = next_block;
				}
				// Include the header adjustment in the returned pointer
				void* aligned_address = (void*)((uptr)free_block + adjustment + sizeof(Header));
				Header* header = (Header*)((uptr)aligned_address - sizeof(Header));
				header->size = size;
				header->alignment = adjustment;
				usedMemory += total_size;
				allocationCount++;

				// Update the next pointer of the allocated block to point to the next free block in the list
				// Block* next_free_block = next_block->next;
				// *((Block**)aligned_address - 1) = next_free_block;

				return aligned_address;
			}
		}

		RAINDROP_log(ERROR, GRAPHICS, "failed to find suitable free memory block");
		throw std::bad_alloc();
	}

	RAINDROP_API void FreelistAllocator::deallocate(void* ptr){
		if (ptr == nullptr) throw std::invalid_argument("cannot deallocate a null pointer");

		Header* header = (Header*)((uptr)ptr - sizeof(Header));
		uptr block_start = (uptr)ptr - header->alignment - sizeof(Header);
		uptr block_end = block_start + header->size + header->alignment;

		Block* prev_free_block = nullptr;
		Block* free_block = free_list_;

		// Find the position where the block should be inserted into the free list
		while (free_block != nullptr) {
			if ((usize)free_block >= block_end) {
				break;
			}
			prev_free_block = free_block;
			free_block = free_block->next;
		}

		if (prev_free_block == nullptr) {
			prev_free_block = (Block*)block_start;
			prev_free_block->size = header->size + header->alignment;
			prev_free_block->next = free_list_;
			free_list_ = prev_free_block;
		} else if ((uptr)prev_free_block + prev_free_block->size == block_start) {
			prev_free_block->size += header->size + header->alignment;
		} else {
			Block* temp = (Block*)block_start;
			temp->size = header->size + header->alignment;
			temp->next = prev_free_block->next;
			prev_free_block->next = temp;
			prev_free_block = temp;
		}

		// Merge adjacent free blocks
		if (free_block != nullptr && (uptr)prev_free_block + prev_free_block->size == (uptr)free_block) {
			prev_free_block->size += free_block->size;
			prev_free_block->next = free_block->next;
		}

		usedMemory -= header->size + header->alignment;
		allocationCount--;
	}
}