#ifndef __RAINDROP_CORE_MEMORY_LINEAR_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_LINEAR_ALLOCATOR_HPP__

#include <Core/Memory/allocators/Allocator.hpp>

namespace Raindrop::Core::Memory{
	class LinearAllocator : public Allocator{
		public:
			LinearAllocator(usize size, void* start);
			~LinearAllocator();

			void* allocate(usize size, uint8 alignment) override;
			void deallocate(void* p) override;
			void clear();

		private:
			void* pos;
	};
}

#endif