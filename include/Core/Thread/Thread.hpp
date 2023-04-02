#ifndef __RAINDROP_CORE_THREAD_THREAD_HPP__
#define __RAINDROP_CORE_THREAD_THREAD_HPP__

#include "Core/common.hpp"
#include "Core/String.hpp"

namespace Raindrop::Core::Thread{
	typedef ID64 ThreadID;

	class Thread{
		public:
			// TODO :: SEPARATE SIGNAL OUTPUT SYSTEM
			enum Signal{
				SIGNAL_INTERUPT,
				SIGNAL_ABORT,
				SIGNAL_FLOAT_POINT_EXCEPTION,
				SIGNAL_SEGMENTATION_VIOLATION,
				SIGNAL_TERMINATE,
			};

			Thread(void* (*fnc)(void*), void* args);
			~Thread();

			void detach();
			void join(void* res = nullptr);
			void kill(Signal signal);
			void cancel();

			ThreadID id() const;
			static Thread self();

			bool operator==(const Thread& other);

		private:
			Thread(void *thread);
			void* _thread;
	};

	inline Thread self(){
		return Thread::self();
	}
}

#endif