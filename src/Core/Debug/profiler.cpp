#include "profiler.hpp"

#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <fstream>

namespace Raindrop::Core::Debug::Profiler{
	struct Profiler_t{
		std::ofstream out;
		uint32_t profileCount;
		std::mutex mutex;
		bool profile = false;
	};

	static Profiler_t __profiler;

	Profile::Profile(const char* scope){
		__profiler.mutex.lock();
		if (__profiler.profile && __profiler.out.is_open()){
			__profiler.out << "s," << scope << "," << std::this_thread::get_id() << "," << std::chrono::steady_clock::now().time_since_epoch().count() << '\n';
			__profiler.out.flush();
		}
		__profiler.mutex.unlock();
	}

	Profile::~Profile(){
		__profiler.mutex.lock();
		if (__profiler.profile && __profiler.out.is_open()){
			__profiler.out << "e," << std::this_thread::get_id() << "," << std::chrono::steady_clock::now().time_since_epoch().count() << ",\n";
			__profiler.out.flush();
		}
		__profiler.mutex.unlock();
	}

	void beginProfile(const char* name, const char* filepath){
		__profiler.out.open(filepath);
		__profiler.out << name << ",,,\n";
		__profiler.out.flush();
	}

	void endProfile(){
		__profiler.out.close();
	}

	void profile(bool profile){
		__profiler.profile = profile;
	}
}