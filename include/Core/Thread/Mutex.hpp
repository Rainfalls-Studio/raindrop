#ifndef __RAINDROP_CORE_THREAD_MUTEX_HPP__
#define __RAINDROP_CORE_THREAD_MUTEX_HPP__

#include "Core/common.hpp"

namespace Raindrop::Core::Thread{
	class Mutex{
		friend class ConditionalVariable;
		public:
			Mutex();
			~Mutex();

			Mutex(const Mutex& other);

			void lock();
			bool tryLock();
			bool tryLockFor(float64 duration);
			void unlock();

		private:
			void* _mutex;
	};
}

#endif