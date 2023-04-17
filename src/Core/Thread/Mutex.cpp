#include <Core/Thread/Mutex.hpp>
#include "Core/Debug/profiler.hpp"
#include <pthread.h>

namespace Raindrop::Core::Thread{
	RAINDROP_API Mutex::Mutex(){
		RAINDROP_profile_function();
		int result = pthread_mutex_init((pthread_mutex_t*)&_mutex, nullptr);
	}

	RAINDROP_API Mutex::Mutex(const Mutex& other){
		RAINDROP_profile_function();
		_mutex = other._mutex;
	}

	RAINDROP_API Mutex::~Mutex(){
		RAINDROP_profile_function();
		pthread_mutex_destroy((pthread_mutex_t*)&_mutex);
	}

	RAINDROP_API void Mutex::lock(){
		RAINDROP_profile_function();
		pthread_mutex_lock((pthread_mutex_t*)&_mutex);
	}

	RAINDROP_API bool Mutex::tryLock(){
		RAINDROP_profile_function();
		return pthread_mutex_trylock((pthread_mutex_t*)&_mutex) == 0;
	}

	RAINDROP_API void Mutex::unlock(){
		RAINDROP_profile_function();
		pthread_mutex_unlock((pthread_mutex_t*)&_mutex);
	}

	RAINDROP_API bool Mutex::tryLockFor(Time::tick time){
		RAINDROP_profile_function();
		timespec specs;
		specs.tv_sec = 0;
		specs.tv_nsec = time * 1e6;
		return pthread_mutex_timedlock((pthread_mutex_t*)&_mutex, &specs) != ETIMEDOUT;
	}
}