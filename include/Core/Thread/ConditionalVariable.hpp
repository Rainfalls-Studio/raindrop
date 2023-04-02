#ifndef __RAINDROP_CORE_THREAD_CONDITIONAL_VARIABLE_HPP__
#define __RAINDROP_CORE_THREAD_CONDITIONAL_VARIABLE_HPP__

#include "Core/common.hpp"
#include "Core/Memory/allocators/Allocator.hpp"
#include "Core/Thread/Mutex.hpp"

namespace Raindrop::Core::Thread{
	class ConditionalVariable{
		public:
			ConditionalVariable();
			~ConditionalVariable();
			
			void signal();
			void broadcast();
			void wait(Mutex &mutex);
			void timedWait(Mutex &mutex, float64 duration);

		private:
			void* _conditionalVariable;
	};
}

#endif