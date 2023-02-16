#include "../Debug/debug.hpp"
#include "../Debug/profiler.hpp"
#include "Thread.hpp"
#include <pthread.h>

namespace Raindrop::Core::Thread{

	int signalToPthreadSignal(Thread::Signal sig){
		RAINDROP_profile_function();
		switch (sig){
			case Thread::SIGNAL_ABORT: return SIGABRT;
			case Thread::SIGNAL_FLOAT_POINT_EXCEPTION: return SIGFPE;
			case Thread::SIGNAL_INTERUPT: return SIGINT;
			case Thread::SIGNAL_SEGMENTATION_VIOLATION: return SIGSEGV;
			case Thread::SIGNAL_TERMINATE: return SIGTERM;
		}
		RAINDROP_log(WARNING, THREAD, "invalid thread kill signal (signal id : %d)", sig);
		return SIGTERM;
	}

	Thread::Thread(void* (*fnc)(void*), void* args){
		RAINDROP_profile_function();
		pthread_create((pthread_t*)&_thread, nullptr, fnc, args);
	}

	Thread::Thread(void *thread) : _thread{thread}{
		RAINDROP_profile_function();
	}

	Thread::~Thread(){
		RAINDROP_profile_function();
	}
	
	void Thread::detatch(){
		RAINDROP_profile_function();
		pthread_detach((pthread_t)_thread);
	}

	void Thread::join(void* res){
		RAINDROP_profile_function();
		pthread_join((pthread_t)_thread, &res);
	}

	void Thread::kill(Signal signal){
		RAINDROP_profile_function();
		pthread_kill((pthread_t)_thread, signalToPthreadSignal(signal));
	}

	void Thread::cancel(){
		RAINDROP_profile_function();
		pthread_cancel((pthread_t)_thread);
	}

	ThreadID Thread::id() const{
		RAINDROP_profile_function();
		return (ThreadID)(pthread_t*)_thread;
	}

	Thread Thread::self(){
		RAINDROP_profile_function();
		return Thread((void*)pthread_self());
	}
}