#ifndef __RAINDROP_CORE_MEMORY_ALLOCATORS_FREELIST_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_ALLOCATORS_FREELIST_ALLOCATOR_HPP__

#include "Allocator.hpp"

namespace Raindrop::Core::Memory{
	class FreelistAllocator : public Allocator{
		public:
			FreelistAllocator(usize size, void* start);
			~FreelistAllocator();

			virtual void* allocate(usize size, uint8 alignment = 4) override;
			virtual void deallocate(void* ptr) override;

		private:
			struct Block {
				usize size;
				Block* next;
			};

			struct Header {
				usize size;
				uint8 alignment;
			};

			void* start_;
			usize size_;
			Block* free_list_;
	};
}

#endif