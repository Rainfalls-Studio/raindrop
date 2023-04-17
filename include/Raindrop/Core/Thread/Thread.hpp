#ifndef __RAINDROP_CORE_THREAD_THREAD_HPP__
#define __RAINDROP_CORE_THREAD_THREAD_HPP__

#include <Core/Thread/common.hpp>

namespace Raindrop::Core::Thread{
	class RAINDROP_API Thread{
		public:
			template<typename T, typename... Args>
			Thread(T& t, Args&&... args);
			~Thread();

			/**
			 * @brief Detach the thread from the main thread. It will no longer wait for it to end
			 * 
			 */
			void detach();

			/**
			 * @brief Wait for the thread to end
			 * @tparam T the type of return, if none, do not return anything
			 */
			template<typename T=void>
			T join();
			
			/**
			 * @brief Stop the thread, only use in extreme cases. Killing a thread can result in unwanted begaviors and leaked memory. Use only when the thread is locked and is the only solution.
			 * 
			 */
			void kill();

			/**
			 * @brief Stop the sexecution of the thread
			 * 
			 */
			void cancel();

			/**
			 * @brief returns the indentifier of a thread
			 * @return ThreadID 
			 */
			ThreadID id() const;

			/**
			 * @brief Returns the handle of the current thread
			 * @return Thread 
			 */
			static Thread self();

			/**
			 * @brief check if this thread and <other> are the same
			 * @param other the thread to compare to
			 * @return true if they are the same, false otherwise
			 */
			bool operator==(const Thread& other);

		private:
			Thread(void *thread);
			void* _thread;
	};

	/**
	 * @brief Returns the handle of the current thread
	 */
	inline Thread self(){
		return Thread::self();
	}
}

#endif