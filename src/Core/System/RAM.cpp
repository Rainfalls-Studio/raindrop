#include "RAM.hpp"
#include "../Debug/profiler.hpp"

#ifdef RAINDROP_WINDOWS
	#include "windows.h"
	#include "psapi.h"
#endif

namespace Raindrop::Core::System{
	#ifdef RAINDROP_WINDOWS
		uint64 RAM::getTotalVirtualMemory(){  
			RAINDROP_profile_function();
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return (uint64)memInfo.ullAvailExtendedVirtual;
		}

		uint64 RAM::getTotalVirtualMemoryUsed(){
			RAINDROP_profile_function();
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return (uint64)(memInfo.ullTotalPageFile - memInfo.ullAvailPageFile);
		}

		uint64 RAM::getTotalVirtualMemoryUsedByProcess(){
			RAINDROP_profile_function();
			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			return (uint64)pmc.PrivateUsage;
		}

		uint64 RAM::getTotalPhysicalMemory(){
			RAINDROP_profile_function();
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPhys;
		}

		uint64 RAM::getTotalPhysicalMemoryUsed(){
			RAINDROP_profile_function();
			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		}

		uint64 RAM::getTotalPhysicalMemoryUsedByProcess(){
			RAINDROP_profile_function();
			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			return pmc.WorkingSetSize;
		}
	#else
		#error RAM works only with the windows API for now
	#endif
}