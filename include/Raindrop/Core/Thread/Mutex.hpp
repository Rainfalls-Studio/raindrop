#ifndef __RAINDROP_CORE_THREAD_MUTEX_HPP__
#define __RAINDROP_CORE_THREAD_MUTEX_HPP__

#include <Core/Thread/common.hpp>
#include <Core/Time/common.hpp>

namespace Raindrop::Core::Thread{
	class RAINDROP_API Mutex{

		/**
		 * @brief the conditional varibal object needs the native mutext handle to work
		 * 
		 */
		friend class ConditionalVariable;
		public:
			Mutex();
			~Mutex();

			/**
			 * @brief Construct a Mutex from another one.
			 * @param other the mutex to copy
			 * 
			 * @warning This can cause issues once the mutex is destroyed. The copy of the mutex is then invalid and shouldn't be used
			 */
			Mutex(const Mutex& other);

			/**
			 * @brief lock the mutex. Any other thread than want to lock it has to wait until the mutex is unlocked.
			 * 
			 */
			void lock();

			/**
			 * @brief try to lock the mutex. 
			 * 
			 * @return true on success. False if the mutex is already locked by another thread
			 */
			bool tryLock();

			/**
			 * @brief Like tryLock() The thread will try to lock the mutex. If it cannot it will try for <time>. If the thread fails to lock the mutex after <time> amount of times. It gives up and return false.
			 * 
			 * @param time 
			 * @return true 
			 * @return false 
			 */
			bool tryLockFor(Time::tick time);

			/**
			 * @brief unlock the mutex and let another thread to lock it.
			 * 
			 */
			void unlock();

		private:
			void* _mutex;
	};
}

#endif