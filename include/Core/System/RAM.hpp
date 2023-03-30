#ifndef __RAINDROP_CORE_SYSTEM_RAM_HPP__
#define __RAINDROP_CORE_SYSTEM_RAM_HPP__

#include "../core.hpp"
#include "../common.hpp"

namespace Raindrop::Core::System{
	class RAM{
		public:
			static uint64 getTotalVirtualMemory();
			static uint64 getTotalVirtualMemoryUsed();
			static uint64 getTotalVirtualMemoryUsedByProcess();
			static uint64 getTotalPhysicalMemory();
			static uint64 getTotalPhysicalMemoryUsed();
			static uint64 getTotalPhysicalMemoryUsedByProcess();
	};
}

#endif