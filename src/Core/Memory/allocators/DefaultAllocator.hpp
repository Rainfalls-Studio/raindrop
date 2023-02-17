#ifndef __RAINDROP_CORE_MEMORY_ALLOCATORS_DEFAULT_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_ALLOCATORS_DEFAULT_ALLOCATOR_HPP__

#include "Allocator.hpp"

namespace Raindrop::Core::Memory{
	class DefaultAllocator : public Allocator{
		public:
			DefaultAllocator();
			~DefaultAllocator();

			virtual void* allocate(usize size, uint8 alignement = 4) override;
			virtual void deallocate(void* p) override;

			static DefaultAllocator& get();
	};
}

#endif