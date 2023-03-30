#ifndef __RAINDROP_CORE_THREAD_MUTEX_HPP__
#define __RAINDROP_CORE_THREAD_MUTEX_HPP__

#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"

namespace Raindrop::Core::Thread{
	class Mutex{
		friend class ConditionalVariable;
		public:
			Mutex(Memory::Allocator& allocator);
			~Mutex();

			void lock();
			bool tryLock();
			bool tryLockFor(float64 duration);
			void unlock();

		private:
			Memory::Allocator* _allocator;
			void* _mutex;
	};
}

#endif