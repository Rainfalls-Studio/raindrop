#ifndef __RAINDROP_CORE_THREAD_BARRIER_HPP__
#define __RAINDROP_CORE_THREAD_BARRIER_HPP__

#include "Core/common.hpp"

namespace Raindrop::Core::Thread{
	class Barrier{
		public:

			/**
			 * @brief conostruct a new barrier object. A barrier a synchronisation object.
			 * @param count the count of threads to wait before releasing them
			 */
			Barrier(uint32 count);
			~Barrier();

			/**
			 * @brief 
			 * 
			 */
			void wait() const;

			uint32 count() const;

		private:
			uint32 _count;
			void* _barrier;
	};
}

#endif