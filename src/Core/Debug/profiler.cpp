#include "Core/Debug/profiler.hpp"

#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <fstream>

namespace Raindrop::Core::Debug::Profiler{
	struct __Profiler{
		std::ofstream out;
		uint32_t profileCount;
		std::mutex mutex;
		bool profile = false;
	};

	static __Profiler* __profiler = nullptr;
	
	RAINDROP_API void init(){
		__profiler = new __Profiler();
	}

	RAINDROP_API void shutdown(){
		delete __profiler;
		__profiler = nullptr;
	}

	RAINDROP_API Profile::Profile(const char* scope){
		if (!__profiler) return;

		__profiler->mutex.lock();
		if (__profiler->profile && __profiler->out.is_open()){
			__profiler->out << "s," << scope << "," << std::this_thread::get_id() << "," << std::chrono::steady_clock::now().time_since_epoch().count() << '\n';
			__profiler->out.flush();
		}
		__profiler->mutex.unlock();
	}

	RAINDROP_API Profile::~Profile(){
		if (!__profiler) return;

		__profiler->mutex.lock();
		if (__profiler->profile && __profiler->out.is_open()){
			__profiler->out << "e," << std::this_thread::get_id() << "," << std::chrono::steady_clock::now().time_since_epoch().count() << ",\n";
			__profiler->out.flush();
		}
		__profiler->mutex.unlock();
	}

	RAINDROP_API void beginProfile(const char* name, const char* filepath){
		if (!__profiler) return;

		__profiler->out.open(filepath);
		__profiler->out << name << ",,,\n";
		__profiler->out.flush();
	}

	RAINDROP_API void endProfile(){
		if (!__profiler) return;
		__profiler->out.close();
	}

	RAINDROP_API void profile(bool profile){
		if (!__profiler) return;
		__profiler->profile = profile;
	}
}

RAINDROP_MODULE  void __RAINDROP_profile_setContext(Raindrop::Core::Debug::Profiler::__Profiler* profiler){
	Raindrop::Core::Debug::Profiler::__profiler = profiler;
}

RAINDROP_MODULE Raindrop::Core::Debug::Profiler::__Profiler* __RAINDROP_profile_getContext(){
	return Raindrop::Core::Debug::Profiler::__profiler;
}