#ifndef __RAINDROP_CORE_MEMORY_QUEUE_HPP__
#define __RAINDROP_CORE_MEMORY_QUEUE_HPP__

#include "Core/Memory/containers/Array.hpp"
#include "Core/Memory/allocators/Allocator.hpp"
#include "Core/Debug/logger.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Memory{
	template<class T>
	class Queue{
		public:
			Queue(Allocator& allocator);
			~Queue();

			void push(T value);

			T& front();
			void pop();

			bool reserve(usize capacity);

			bool empty() const;
			usize size() const;
			usize capacity() const;

		private:
			void increaseCapacity(usize newCapacity);
			void grow(usize minCapacity = 0);

			Allocator& _allocator;
			Array<T> _data;
			usize _start;
			usize _size;
	};

	template<class T>
	Queue<T>::Queue(Allocator& allocator) : _allocator(allocator), _data(allocator), _start(0), _size(0){
		RAINDROP_profile_function();
	}

	template<class T>
	Queue<T>::~Queue(){
		RAINDROP_profile_function();
	}

	template<class T>
	void Queue<T>::push(T value){
		RAINDROP_profile_function();
		if(_size == _data.size())
			grow();

		usize end = (_start + _size) % _data.size();
		_data[end] = value;

		_size++;
	}

	template<class T>
	T& Queue<T>::front(){
		RAINDROP_profile_function();
		return _data[_start];
	}

	template<class T>
	void Queue<T>::pop(){
		RAINDROP_profile_function();
		_start = (_start + 1) % _data.size();
		_size--;
	}

	template<class T>
	bool Queue<T>::reserve(usize capacity){
		RAINDROP_profile_function();
		if(capacity > _data.size())
			increaseCapacity(capacity);

		return true;
	}

	template<class T>
	bool Queue<T>::empty() const{
		RAINDROP_profile_function();
		return _size == 0;
	}

	template<class T>
	usize Queue<T>::size() const{
		RAINDROP_profile_function();
		return _size;
	}

	template<class T>
	usize Queue<T>::capacity() const{
		RAINDROP_profile_function();
		return _data.size();
	}

	template<typename T> 
	void Queue<T>::increaseCapacity(usize new_capacity){
		RAINDROP_profile_function();
		usize end = _data.size();
		_data.resize(new_capacity);

		// eg:
		// |3|4|-|1|2| --> |3|4|-|1|2|-|-|-| --> |3|4|-|-|-|-|1|2|

		if (_start + _size > end){
			usize end_items = end - _start;
			memmove(&_data[0] + new_capacity - end_items, &_data[0] + _start, end_items * sizeof(T));
			_start += new_capacity - end;
		}
	}

	template<typename T> 
	void Queue<T>::grow(usize min_capacity){
		RAINDROP_profile_function();
		usize new_capacity = _data.size() * 2 + 8;

		if (new_capacity < min_capacity)
			new_capacity = min_capacity;

		increaseCapacity(new_capacity);
	}

	//-----------------------------------------------------------------
	//-- FIXED SIZE QUEUE
	//-----------------------------------------------------------------

	template<class T, usize size>
	class FixedSizeQueue{
		public:
			FixedSizeQueue();
			~FixedSizeQueue();

			bool empty() const;
			void push(T value);
			T pop();

			usize count() const;

		private:
			usize _start;
			usize _count;
			T _data[size];
	};

	template<class T, usize size>
	FixedSizeQueue<T, size>::FixedSizeQueue() : _start(0), _count(0){
		RAINDROP_profile_function();
	}

	template<class T, usize size>
	FixedSizeQueue<T, size>::~FixedSizeQueue(){
		RAINDROP_profile_function();
	}

	template<class T, usize size>
	bool FixedSizeQueue<T, size>::empty() const{
		RAINDROP_profile_function();
		return _count == 0;
	}

	template<class T, usize size>
	void FixedSizeQueue<T, size>::push(T value){
		RAINDROP_profile_function();
		usize end = (_start + _count) % size;
		_data[end] = value;

		if (_count == size){
			_start = (_start + 1) % size; /* full, overwrite */
			RAINDROP_log(WARNING, MEMORY, "the queue is full, overwriting the last element");
		}
		else
			_count++;
	}

	template<class T, usize size>
	T FixedSizeQueue<T, size>::pop(){
		RAINDROP_profile_function();
		T& value = _data[_start];
		_start = (_start + 1) % size;
		_count--;

		return value;
	}

	template<class T, usize size>
	usize FixedSizeQueue<T, size>::count() const{
		RAINDROP_profile_function();
		return _count;
	}
};

#endif