#ifndef __RAINDROP_CORE_MEMORY_POOL_HPP__
#define __RAINDROP_CORE_MEMORY_POOL_HPP__

#include "../allocators/Allocator.hpp"
#include "../../Debug/logger.hpp"
#include "../../Debug/profiler.hpp"
#include "../../Maths/Maths.hpp"
#include <memory.h>

namespace Raindrop::Core::Memory{
	template<typename T>
	class Pool{
		public:
			Pool(usize size, Allocator &allocator);
			~Pool();

			T* get();
			void push(T* t);

			/**
			 * @brief the remaining elements in the pool
			 * @return usize 
			 */
			usize size() const;

			/**
			 * @brief check if there a no remaining elements in the pool
			 * 
			 * @return true 
			 * @return false 
			 */
			bool empty() const;

			/**
			 * @brief 
			 * 
			 * @param size 
			 */
			void reserve(usize size);

		private:

			union Node{
				T _value;
				Node* _next;
			};

			Allocator* _allocator;
			Node* _begin;
			Node* _ptr;
			usize _size;
	};

	template<typename T>
	Pool<T>::Pool(usize size, Allocator &allocator) : _allocator{&allocator}, _size{size}{
		RAINDROP_profile_function();
		_ptr = allocateArray<Node>(*_allocator, size);

		for (usize i=1; i<size; i++){
			_ptr[i-1]._next = &_ptr[i];
		}

		_ptr[size-1]._next = nullptr;
		_begin = _ptr;
	}

	template<typename T>
	Pool<T>::~Pool(){
		RAINDROP_profile_function();
		deallocateArray(*_allocator, _ptr, _size);
		_allocator = nullptr;
		_begin = nullptr;
		_ptr = nullptr;
		_size = 0;
	}
	
	template<typename T>
	T* Pool<T>::get(){
		RAINDROP_profile_function();
		if (_begin){
			Node* node = _begin;
			_begin = _begin->_next;
			_size--;
			return &node->_value;
		}
		RAINDROP_log(WARNING, MEMORY, "the pool is empty");
		return nullptr;
	}

	template<typename T>
	void Pool<T>::push(T* t){
		RAINDROP_profile_function();
		Node* node = (Node*)t;

		node->_next = _begin;
		_begin = node;
		_size++;
	}

	template<typename T>
	usize Pool<T>::size() const{
		RAINDROP_profile_function();
		return _size;
	}

	template<typename T>
	bool Pool<T>::empty() const{
		RAINDROP_profile_function();
		return _size == 0;
	}

	template<typename T>
	void Pool<T>::reserve(usize size){
		RAINDROP_log(ERROR, MEMORY, "reserve is not available for now in pool containsers");
	}
};

#endif