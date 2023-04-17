#include "Core/Thread/ConditionalVariable.hpp"
#include "Core/Debug/profiler.hpp"
#include <pthread.h>

namespace Raindrop::Core::Thread{
	RAINDROP_API ConditionalVariable::ConditionalVariable(const ConditionalVariable& other) : _conditionalVariable{other._conditionalVariable}{
		RAINDROP_profile_function();
	}

	RAINDROP_API ConditionalVariable& ConditionalVariable::operator=(const ConditionalVariable& other){
		RAINDROP_profile_function();
		_conditionalVariable = other._conditionalVariable;
		return *this;
	}

	RAINDROP_API ConditionalVariable::ConditionalVariable(){
		RAINDROP_profile_function();
		pthread_cond_init((pthread_cond_t*)&_conditionalVariable, nullptr);
	}

	RAINDROP_API ConditionalVariable::~ConditionalVariable(){
		RAINDROP_profile_function();
		pthread_cond_destroy((pthread_cond_t*)&_conditionalVariable);
	}
	
	RAINDROP_API void ConditionalVariable::signal(){
		RAINDROP_profile_function();
		pthread_cond_signal((pthread_cond_t*)&_conditionalVariable);
	}

	RAINDROP_API void ConditionalVariable::broadcast(){
		RAINDROP_profile_function();
		pthread_cond_broadcast((pthread_cond_t*)&_conditionalVariable);
	}

	RAINDROP_API void ConditionalVariable::wait(Mutex &mutex){
		RAINDROP_profile_function();
		pthread_cond_wait((pthread_cond_t*)&_conditionalVariable, (pthread_mutex_t*)&mutex._mutex);
	}

	RAINDROP_API bool ConditionalVariable::timedWait(Mutex &mutex, Time::tick time){
		RAINDROP_profile_function();
		timespec specs;
		specs.tv_sec = 0;
		specs.tv_nsec = time * 1e6;
		return pthread_cond_timedwait((pthread_cond_t*)&_conditionalVariable, (pthread_mutex_t*)mutex._mutex, &specs) != ETIMEDOUT;
	}
}