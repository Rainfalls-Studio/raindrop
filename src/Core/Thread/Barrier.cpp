#include <Core/Thread/Barrier.hpp>
#include <Core/Debug/profiler.hpp>
#include <pthread.h>

namespace Raindrop::Core::Thread{
	RAINDROP_API Barrier::Barrier(uint32 count) : _count(count){
		RAINDROP_profile_function();
		pthread_barrier_init((pthread_barrier_t*)&_barrier, nullptr, count);
	}

	RAINDROP_API Barrier::~Barrier(){
		RAINDROP_profile_function();
		pthread_barrier_destroy((pthread_barrier_t*)&_barrier);
		_barrier = nullptr;
	}

	RAINDROP_API void Barrier::wait() const{
		RAINDROP_profile_function();
		pthread_barrier_wait((pthread_barrier_t*)&_barrier);
	}

	RAINDROP_API uint32 Barrier::count() const{
		return _count;
	}

}