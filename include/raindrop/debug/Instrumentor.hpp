/**
 * @brief this file contain the debug instrumentor, only used in debug mode
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @attention IF YOU DO ANY CHANGE IN THIS FILE, MAKE SURE THAT THE WHOLE PROJECT IS RECOMPILED
 * @author TheCherno (https://github.com/TheCherno)
 * @link https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e
 */

//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//

#ifndef __RAINDROP_DEBUG_INSTRUMENTOR_HPP__
#define __RAINDROP_DEBUG_INSTRUMENTOR_HPP__

#include "core.hpp"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>

namespace rnd::debug{
	struct ProfileResult{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession{
		std::string Name;
	};

	class Instrumentor{
		public:
			Instrumentor();

			void BeginSession(const std::string& name, const std::string& filepath = "results.json");
			void EndSession();

			void WriteProfile(const ProfileResult& result);
			void WriteHeader();
			void WriteFooter();

			void newFrame();
			void setFrameTarget(uint64_t frameCount);

			static Instrumentor& Get();
			
		
		private:
			InstrumentationSession* m_CurrentSession;
			std::ofstream m_OutputStream;
			int m_ProfileCount;

			uint64_t frameCount = 0;
			uint64_t frameTarget = 0;

			std::mutex mutex;

			static Instrumentor instance;
	};

	class InstrumentationTimer{
		public:
			InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false){
				m_StartTimepoint = std::chrono::high_resolution_clock::now();
			}

			~InstrumentationTimer(){
				if (!m_Stopped) Stop();
			}

			void Stop(){
				auto endTimepoint = std::chrono::high_resolution_clock::now();

				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

				uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
				Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

				m_Stopped = true;
			}

		private:
			const char* m_Name;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
			bool m_Stopped;
	};
}

#define COMBINE(x, y) x  ## y
#define LEVEL1(x,y) COMBINE(x,y)

#if defined(PROFILE) && PROFILE == true
	#define PROFILE_BEGIN_SESSION(name, filepath) ::rnd::debug::Instrumentor::Get().BeginSession(name, filepath)
	#define PROFILE_END_SESSION() ::rnd::debug::Instrumentor::Get().EndSession()
	#define PROFILE_SCOPE(name) LEVEL1(::rnd::debug::InstrumentationTimer timer, __LINE__) (name)
	#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
	#define PROFILE_RECORD() ::rnd::debug::Instrumentor::Get().setFrameTarget(UINT64_MAX)
	#define PROFILE_STOP_RECORD() ::rnd::debug::Instrumentor::Get().setFrameTarget(0)
	#define PROFILE_RECORD_FRAMES(frameCount) ::rnd::debug::Instrumentor::Get().setFrameTarget(frameCount)
	#define PROFILE_UPDATE_FRAME() ::rnd::debug::Instrumentor::Get().newFrame()
#else
    #define PROFILE_BEGIN_SESSION(name, filepath)
    #define PROFILE_END_SESSION()
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNCTION()
    #define PROFILE_RECORD()
    #define PROFILE_STOP_RECORD()
    #define PROFILE_RECORD_FRAMES(frameCount)
    #define PROFILE_UPDATE_FRAME()
#endif

#endif