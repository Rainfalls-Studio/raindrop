#include "Core/Thread/Mutex.hpp"
#include "Core/Debug/profiler.hpp"
#include <pthread.h>

/**
 * @brief the mutex store an allocator in case if the native mutex changes and it need to allocate a mutex
 */

namespace Raindrop::Core::Thread{
	Mutex::Mutex(Memory::Allocator& allocator) : _allocator(&allocator){
		RAINDROP_profile_function();
		int result = pthread_mutex_init((pthread_mutex_t*)&_mutex, nullptr);
	}

	Mutex::~Mutex(){
		RAINDROP_profile_function();
		pthread_mutex_destroy((pthread_mutex_t*)&_mutex);
	}

	void Mutex::lock(){
		RAINDROP_profile_function();
		pthread_mutex_lock((pthread_mutex_t*)&_mutex);
	}

	bool Mutex::tryLock(){
		RAINDROP_profile_function();
		return pthread_mutex_trylock((pthread_mutex_t*)&_mutex) == 0;
	}

	void Mutex::unlock(){
		RAINDROP_profile_function();
		pthread_mutex_unlock((pthread_mutex_t*)&_mutex);
	}

	bool Mutex::tryLockFor(float64 duration){
		RAINDROP_profile_function();
		timespec specs;
		specs.tv_sec = duration;
		specs.tv_nsec = duration * 10e9;
		return pthread_mutex_timedlock((pthread_mutex_t*)&_mutex, &specs) == 0;
	}
}