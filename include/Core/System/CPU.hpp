#ifndef __RAINDROP_CORE_SYSTEM_CPU_HPP__
#define __RAINDROP_CORE_SYSTEM_CPU_HPP__

#include "../core.hpp"
#include "../common.hpp"

namespace Raindrop::Core::System{
	class CPU{
		public:
			static uint64 getTicks();
			static uint64 getClockSpeed();
			static uint32 getCoreCount();
			static uint32 getLevel();
			static float32 getUsage();
			static float32 getUsageByProcess();
			static const char* getArchitecture();
	};
}

#endif