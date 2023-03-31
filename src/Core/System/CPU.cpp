#include "Core/System/CPU.hpp"
#include "Core/Debug/profiler.hpp"
#include <time.h>

#if defined(RAINDROP_WINDOWS)
	#include <intrin.h>
	#include <windows.h>
	#include <TCHAR.h>
	#include <pdh.h>
#endif

namespace Raindrop::Core::System{
	#ifdef RAINDROP_WINDOWS
		struct Utils{
			Utils();

			// ::PDH_HQUERY cpuQuery;
			// ::PDH_HCOUNTER cpuTotal;
			::ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
			int numProcessors;
			::HANDLE self;
		};

		static Utils utils;

		Utils::Utils(){
			RAINDROP_profile_function();
			// PdhOpenQuery(NULL, NULL, &cpuQuery);
			// // You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
			// PdhAddEnglishCounter(cpuQuery, (LPCSTR)"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
			// ::PdhCollectQueryData(cpuQuery);
			
			// SYSTEM_INFO sysInfo;
			// FILETIME ftime, fsys, fuser;

			// ::GetSystemInfo(&sysInfo);
			// numProcessors = sysInfo.dwNumberOfProcessors;

			// ::GetSystemTimeAsFileTime(&ftime);
			// memcpy(&lastCPU, &ftime, sizeof(FILETIME));

			// self = ::GetCurrentProcess();
			// ::GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
			// memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
			// memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
		};

		uint64 CPU::getTicks(){
			RAINDROP_profile_function();
			return (uint64)::GetTickCount64();
		}

		uint64 CPU::getClockSpeed(){
			RAINDROP_profile_function();
			::LARGE_INTEGER liFrequency;
			::QueryPerformanceFrequency(&liFrequency);
			return (uint64)liFrequency.QuadPart;
		}

		uint32 CPU::getCoreCount(){
			RAINDROP_profile_function();
			::SYSTEM_INFO info;
			::GetSystemInfo(&info);
			return (uint32)info.dwNumberOfProcessors;
		}

		uint32 CPU::getLevel(){
			RAINDROP_profile_function();
			::SYSTEM_INFO info;
			::GetNativeSystemInfo(&info);
			return (uint32)info.wProcessorLevel;
		}

		const char* CPU::getArchitecture(){
			RAINDROP_profile_function();
			::SYSTEM_INFO info;
			::GetNativeSystemInfo(&info);

			switch (info.wProcessorArchitecture){
				case PROCESSOR_ARCHITECTURE_AMD64: return "x64 AMD or Intel";
				case PROCESSOR_ARCHITECTURE_ARM: return "ARM";
				case PROCESSOR_ARCHITECTURE_ARM64: return "ARM64";
				case PROCESSOR_ARCHITECTURE_IA64: return "Intel Itanium-based";
				case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
			}

			return "unknown architecture";
		}

		float32 CPU::getUsage(){
			RAINDROP_profile_function();
			// ::PDH_FMT_COUNTERVALUE counterVal;

			// ::PdhCollectQueryData(utils.cpuQuery);
			// ::PdhGetFormattedCounterValue(utils.cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
			// return counterVal.doubleValue;
		}

		float32 CPU::getUsageByProcess(){
			RAINDROP_profile_function();
			// ::FILETIME ftime, fsys, fuser;
			// ::ULARGE_INTEGER now, sys, user;
			// double percent;

			// ::GetSystemTimeAsFileTime(&ftime);
			// memcpy(&now, &ftime, sizeof(FILETIME));

			// ::GetProcessTimes(utils.self, &ftime, &ftime, &fsys, &fuser);
			// memcpy(&sys, &fsys, sizeof(FILETIME));
			// memcpy(&user, &fuser, sizeof(FILETIME));
			// percent = (sys.QuadPart - utils.lastSysCPU.QuadPart) + (user.QuadPart - utils.lastUserCPU.QuadPart);
			// percent /= (now.QuadPart - utils.lastCPU.QuadPart);
			// percent /= utils.numProcessors;
			// utils.lastCPU = now;
			// utils.lastUserCPU = user;
			// utils.lastSysCPU = sys;

			// return percent * 100;
		}
	#endif
}