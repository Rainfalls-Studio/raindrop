#include "ThreadSafeAllocator.hpp"
#include "../../Debug/profiler.hpp"

namespace Raindrop::Core::Memory{
	ThreadSafeAllocator::ThreadSafeAllocator(Allocator& allocator) : Allocator(allocator.getSize(), allocator.getStart()), _allocator(&allocator), _mutex(allocator){
		RAINDROP_profile_function();
	}

	ThreadSafeAllocator::~ThreadSafeAllocator(){
		RAINDROP_profile_function();
	}

	void* ThreadSafeAllocator::allocate(usize size, uint8 alignment){
		RAINDROP_profile_function();
		RAINDROP_assert(size != 0);
		_mutex.lock();
		void* p = _allocator->allocate(size, alignment);
		_mutex.unlock();
		return p;
	}

	void ThreadSafeAllocator::deallocate(void* p){
		RAINDROP_profile_function();
		RAINDROP_assert(p != nullptr);
		_mutex.lock();
		_allocator->deallocate(p);
		_mutex.unlock();
	}

}