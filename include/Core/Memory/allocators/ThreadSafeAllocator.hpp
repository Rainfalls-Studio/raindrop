#ifndef __RAINDROP_CORE_MEMORY_ALLOCATOR_THREAD_SAFE_ALLOCATOR_HPP__
#define __RAINDROP_CORE_MEMORY_ALLOCATOR_THREAD_SAFE_ALLOCATOR_HPP__

#include "Core/Memory/allocators/Allocator.hpp"
#include "Core/Thread/Mutex.hpp"


namespace Raindrop::Core::Memory{
	class ThreadSafeAllocator : public Allocator{
		public:
			ThreadSafeAllocator(Allocator& allocator);
			~ThreadSafeAllocator();

			void* allocate(usize size, uint8 alignment) override; 
			void deallocate(void* p) override;
		private:
			Allocator* _allocator;
			Thread::Mutex _mutex;
	};
}

#endif