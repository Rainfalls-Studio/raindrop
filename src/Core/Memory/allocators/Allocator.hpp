#ifndef __RAINDROP_CORE_MEMORY_ALLOCATE_HPP__
#define __RAINDROP_CORE_MEMORY_ALLOCATE_HPP__

#include "../../common.hpp"
#include "../../Debug/debug.hpp"
#include "../../Debug/profiler.hpp"

#include <new>

namespace Raindrop::Core::Memory{
	class Allocator{
		public: 

			/**
			 * @brief constructure of the base allocator class
			 * 
			 * @param size the total size of the allocator
			 * @param start the pointer to the allocator's memory chunk
			 */
			Allocator(usize size, void* start){
				RAINDROP_profile_function();
				this->start = start;
				this->size = size;
				usedMemory = 0;
				allocationCount = 0;
			}
			
			virtual ~Allocator(){
				RAINDROP_profile_function();
				RAINDROP_assert(allocationCount == 0 && usedMemory == 0 && "MEMORY LEAK !");
				start = nullptr;
				size = 0;
			}
			
			virtual void* allocate(usize size, uint8 alignment = 4) = 0;
			virtual void deallocate(void* p) = 0;

			void* getStart() const {return start;}
			usize getSize() const {return size;}
			usize getUsedMemory() const {return usedMemory;}
			usize getAllocationCount() const {return allocationCount;}
			
		protected: 
			void* start;
			usize size;
			usize usedMemory;
			usize allocationCount;
	};

	template <typename T, typename... Args>
	T* allocateNew(Allocator& allocator, Args... args) {
		RAINDROP_profile_function();
		T* p = (T*)allocator.allocate(sizeof(T), alignof(T));
		if (!p) return nullptr;
		return new (p) T(args...);
	}

	template <typename T> void deallocateDelete(Allocator& allocator, T& object){
		RAINDROP_profile_function();
		object.~T(); 
		allocator.deallocate(&object);
	}

	template<typename T, typename... Args>
	T* allocateArray(Allocator& allocator, usize length, Args... args){
		RAINDROP_profile_function();
		RAINDROP_assert(length != 0); 
		//Allocate extra space to store array length in the bytes before the array 
		T* p = ((T*)allocator.allocate(sizeof(T)*length, alignof(T)));
		if (!p) return nullptr;

		for (usize i=0; i<length; i++)
			new (&p[i]) T(args...); 

		return p;
	}

	template<typename T>
	void deallocateArray(Allocator& allocator, T* array, usize length){
		RAINDROP_profile_function();
		RAINDROP_assert(array != nullptr);
		for (usize i=0; i<length; i++) array[i].~T();
		allocator.deallocate(array); 
	}
}

#endif