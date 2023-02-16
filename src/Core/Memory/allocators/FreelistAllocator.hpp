#ifndef __RAINDROP_CORE_MEMORY_ALLOCATORS_FREELIST_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_ALLOCATORS_FREELIST_ALLOCATOR_HPP__

#include "Allocator.hpp"

namespace Raindrop::Core::Memory{
	class FreelistAllocator : public Allocator{
		public:
			FreelistAllocator(usize size, void* start);
			~FreelistAllocator();

			virtual void* allocate(usize size, uint8 alignement = 4) override;
			virtual void deallocate(void* p) override;

			void clear();

		private:
			struct FreeBlock{
				FreeBlock* _next;
				usize _size;
			};

			struct Header{
				usize _size;
			};

			FreeBlock* _begin = nullptr;
			FreeBlock* _end = nullptr;
			usize _size;

			FreeBlock* push(void* ptr, usize size);
			FreeBlock* popBestNode(usize size);
			bool validateNode(Header* header);
	};
}

#endif