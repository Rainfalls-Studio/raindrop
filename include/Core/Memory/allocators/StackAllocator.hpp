#ifndef __RAINDROP_CORE_MEMORY_STACK_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_STACK_ALLOCATOR_HPP__

#include "Core/Memory/allocators/Allocator.hpp"

namespace Raindrop::Core::Memory{
	class StackAllocator : public Allocator{
		public:
			StackAllocator(usize size, void* start);
			~StackAllocator();

			void* allocate(usize size, uint8 alignment) override;
			void deallocate(void* p) override;
			void clear();

		private:
			struct AllocationHeader{	
				#ifdef _DEBUG
					void* previousAdress = nullptr;
				#endif

				uint8 adjustment;
			};

			void* pos;

			#ifdef _DEBUG
				void* previousPos;
			#endif
	};
}

#endif