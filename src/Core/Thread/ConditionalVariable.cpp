#include "Core/Thread/ConditionalVariable.hpp"
#include "Core/Debug/profiler.hpp"
#include <pthread.h>

namespace Raindrop::Core::Thread{
	ConditionalVariable::ConditionalVariable(Memory::Allocator& allocator){
		RAINDROP_profile_function();
		pthread_cond_init((pthread_cond_t*)&_conditionalVariable, nullptr);
	}

	ConditionalVariable::~ConditionalVariable(){
		RAINDROP_profile_function();
		pthread_cond_destroy((pthread_cond_t*)&_conditionalVariable);
	}
	
	void ConditionalVariable::signal(){
		RAINDROP_profile_function();
		pthread_cond_signal((pthread_cond_t*)&_conditionalVariable);
	}

	void ConditionalVariable::broadcast(){
		RAINDROP_profile_function();
		pthread_cond_broadcast((pthread_cond_t*)&_conditionalVariable);
	}

	void ConditionalVariable::wait(Mutex &mutex){
		RAINDROP_profile_function();
		pthread_cond_wait((pthread_cond_t*)&_conditionalVariable, (pthread_mutex_t*)mutex._mutex);
	}

	void ConditionalVariable::timedWait(Mutex &mutex, float64 duration){
		RAINDROP_profile_function();
		timespec specs;
		specs.tv_sec = duration;
		specs.tv_nsec = duration * 10e9;
		pthread_cond_timedwait((pthread_cond_t*)&_conditionalVariable, (pthread_mutex_t*)mutex._mutex, &specs);
	}
}