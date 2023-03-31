#ifndef __RAINDROP_CORE_THREAD_BARRIER_HPP__
#define __RAINDROP_CORE_THREAD_BARRIER_HPP__

#include "Core/common.hpp"
#include "Core/Memory/allocators/Allocator.hpp"

namespace Raindrop::Core::Thread{
	class Barrier{
		public:

			/**
			 * @brief conostruct a new barrier object. A barrier a synchronisation object.
			 * @param allocator the allocator to allocate intern elements from
			 * @param count the count of threads to wait before releasing them
			 */
			Barrier(Memory::Allocator& allocator, uint32 count);
			~Barrier();

			/**
			 * @brief 
			 * 
			 */
			void wait() const;

		private:
			Memory::Allocator* _allocator;
			uint32 _count;
			void* _barrier;
	};
}

#endif