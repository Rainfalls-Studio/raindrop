
#include <Core/Debug/profiler.hpp>
#include <Core/Thread/Thread.hpp>
#include <pthread.h>
#include <stdarg.h>
#include <tuple>

namespace Raindrop::Core::Thread{
	RAINDROP_API Thread::Thread(void *thread) : _thread{thread}{
		RAINDROP_profile_function();
	}

	template<typename T, typename... Args>
	RAINDROP_API Thread::Thread(T& t, Args&&... args){
		RAINDROP_profile_function();

		// auto wrapper_function = [](void* p) -> void*
		// {
		// 	auto& data = *reinterpret_cast<std::tuple<T, Args...>*>(p);
		// 	std::apply(std::get<0>(data), std::make_tuple(std::get<1>(data)...));
		// 	return nullptr;
		// };
		// _args = new std::tuple<T, Args...>(t, std::forward<Args>(args)...);
		// pthread_create(&(pthread_t)_thread, nullptr, wrapper_function, _args);
	}

	RAINDROP_API Thread::~Thread(){
		RAINDROP_profile_function();
	}
	
	RAINDROP_API void Thread::detach(){
		RAINDROP_profile_function();
		pthread_detach((pthread_t)_thread);
	}
	
	// template<typename T>
	// RAINDROP_API T Thread::join(){
	// 	void* res;
	// 	pthread_join((pthread_t)_thread, &res);

	// 	if constexpr (!std::is_same<void, T>){
	// 		return *reinterpret_cast<T*>(res);
	// 	}
	// 	return;
	// }

	RAINDROP_API void Thread::kill(){
		RAINDROP_profile_function();
		pthread_kill((pthread_t)_thread, SIGTERM);
	}

	RAINDROP_API void Thread::cancel(){
		RAINDROP_profile_function();
		pthread_cancel((pthread_t)_thread);
	}

	RAINDROP_API ThreadID Thread::id() const{
		RAINDROP_profile_function();
		return (ThreadID)(pthread_t*)_thread;
	}

	RAINDROP_API Thread Thread::self(){
		RAINDROP_profile_function();
		return Thread((void*)pthread_self());
	}

	RAINDROP_API bool Thread::operator==(const Thread& other){
		RAINDROP_profile_function();
		return other._thread == _thread;
	}

}