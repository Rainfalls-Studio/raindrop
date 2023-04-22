#include "Core/Memory/allocators/ThreadSafeAllocator.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Memory{
	RAINDROP_API ThreadSafeAllocator::ThreadSafeAllocator(Allocator& allocator) : Allocator(allocator.getSize(), allocator.getStart()), _allocator(&allocator){
		RAINDROP_profile_function();
	}

	RAINDROP_API ThreadSafeAllocator::~ThreadSafeAllocator(){
		RAINDROP_profile_function();
	}

	RAINDROP_API void* ThreadSafeAllocator::allocate(usize size, uint8 alignment){
		RAINDROP_profile_function();
		RAINDROP_assert(size != 0);
		_mutex.lock();
		void* p = _allocator->allocate(size, alignment);
		_mutex.unlock();
		return p;
	}

	RAINDROP_API void ThreadSafeAllocator::deallocate(void* p){
		RAINDROP_profile_function();
		RAINDROP_assert(p != nullptr);
		_mutex.lock();
		_allocator->deallocate(p);
		_mutex.unlock();
	}

}