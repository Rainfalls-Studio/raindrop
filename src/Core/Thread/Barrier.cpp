#include "Barrier.hpp"
#include "../Debug/profiler.hpp"
#include <pthread.h>

namespace Raindrop::Core::Thread{
	Barrier::Barrier(Memory::Allocator& allocator, uint32 count) : _allocator(&allocator), _count(count){
		RAINDROP_profile_function();
		pthread_barrier_init((pthread_barrier_t*)&_barrier, nullptr, count);
	}

	Barrier::~Barrier(){
		RAINDROP_profile_function();
		pthread_barrier_destroy((pthread_barrier_t*)&_barrier);
		_barrier = nullptr;
	}

	void Barrier::wait() const{
		RAINDROP_profile_function();
		pthread_barrier_wait((pthread_barrier_t*)&_barrier);
	}
}