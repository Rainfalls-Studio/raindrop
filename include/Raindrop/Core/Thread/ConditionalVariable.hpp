#ifndef __RAINDROP_CORE_THREAD_CONDITIONAL_VARIABLE_HPP__
#define __RAINDROP_CORE_THREAD_CONDITIONAL_VARIABLE_HPP__

#include <Core/Thread/common.hpp>
#include <Core/Thread/Mutex.hpp>
#include <Core/Time/common.hpp>

namespace Raindrop::Core::Thread{
	class RAINDROP_API ConditionalVariable{
		public:

			/**
			 * @brief create a new conditional variable
			 * 
			 */
			ConditionalVariable();
			~ConditionalVariable();

			// Copy
			ConditionalVariable(const ConditionalVariable& other);
			ConditionalVariable& operator=(const ConditionalVariable& other);
			
			/**
			 * @brief Wait for another thread to singal / broadcast a signal
			 * @param mutex The mutex 'linked' with the condition to check
			 */
			void wait(Mutex &mutex);

			
			/**
			 * @brief Wait for another thread to signal / broadact a signal. If the thread waits more than <time>. It stops waiting and returns false
			 * 
			 * @param mutex The mutex 'linked' with the condition to check.
			 * @param time The max amount of time to wait.
			 */
			bool timedWait(Mutex &mutex, Time::tick time);

			/**
			 * @brief Send to another waiting thread to continue. (Send the signal to only one wating thread)
			 * 
			 */
			void signal();

			/**
			 * @brief Send to all other wating threads to continue.
			 * 
			 */
			void broadcast();


		private:
			void* _conditionalVariable;
	};
}

#endif