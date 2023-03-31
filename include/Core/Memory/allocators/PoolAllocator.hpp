#ifndef __RAINDROP_CORE_MEMORY_POOL_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_POOL_ALLOCATOR_HPP__

#include "Core/Memory/allocators/Allocator.hpp"

namespace Raindrop::Core::Memory{
	class PoolAllocator : public Allocator{
		public: 
			PoolAllocator(size_t objectSize, uint8 objectAlignment, usize size, void* mem);
			~PoolAllocator();

			void* allocate(size_t size, uint8 alignment) override;
			void deallocate(void* p) override;
		
		private:

			usize _objectSize;
			uint8 _objectAlignment; 
			void** freeList;
	};
}

#endif