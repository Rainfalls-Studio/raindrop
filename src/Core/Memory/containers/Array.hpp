#ifndef __RAINDROP_CORE_MEMORY_ARRAY_HPP__
#define __RAINDROP_CORE_MEMORY_ARRAY_HPP__

#include "../allocators/Allocator.hpp"
#include "../../Debug/profiler.hpp"
#include <utility>

namespace Raindrop::Core::Memory{
	template<typename T>
	class Array{
		public:
			Array(Allocator& allocator, usize size = 0);
			Array(const Array& other);
			Array(Array&& other);
			Array();

			void initialize(Allocator& allocator, usize size = 0);

			~Array();

			Array& operator=(Array other);

			template<typename A>
			friend void swap(Array<A>& x, Array<A>& y);

			T& operator[](usize i);
			const T& operator[](usize i) const;

			T* data(){
				return _data;
			}

			void push(T value);
			void push(Array<T>&& arr);
			void push(Array<T>& arr);
			void push(usize count, const T* values);

			void pop();
			void clear();

			bool reserve(usize new_capacity);
			bool resize(usize new_size);
			bool resize(usize new_size, const T& value);
			bool setCapacity(usize new_capacity);

			bool grow(usize min_capacity = 0);

			bool empty() const;
			usize size() const;
			usize capacity() const;

		private:
			Allocator* _allocator; //must use pointer instead of reference for swap to work
			T*         _data;
			usize	   _size;
			usize	   _capacity;
	};

	template<typename T>
	Array<T>::Array() : _allocator(nullptr), _data(nullptr), _size(0), _capacity(0){}

	template<typename T>
	Array<T>::Array(Allocator& allocator, usize size)
		: _allocator(&allocator),
		  _data(size > 0 ? (T*)allocator.allocate(sizeof(T)*size, alignof(T)) : nullptr),
		  _size(size), _capacity(size){
		RAINDROP_profile_function();
	}

	template<typename T>
	Array<T>::Array(const Array<T>& other) : _allocator(other._allocator),
		  _data(other._capacity > 0 ? (T*)other._allocator->allocate(sizeof(T)*other._capacity, alignof(T)) : nullptr),
		  _size(other._size), _capacity(other._capacity){
		RAINDROP_profile_function();
		memcpy(_data, other._data, _size);
	}

	template<typename T>
	Array<T>::Array(Array<T>&& other) : Array(*other._allocator, 0){
		RAINDROP_profile_function();
		swap(*this, other);
	}

	template<typename T>
	Array<T>::~Array(){
		RAINDROP_profile_function();
		if(_data != nullptr)
			deallocateArray(*_allocator, _data, _capacity);

		_data = nullptr;
	}

	template<typename T>
	void Array<T>::initialize(Allocator& allocator, usize size){
		RAINDROP_profile_function();
		RAINDROP_assert(_allocator == nullptr && "cannot initialize an array multiple times");
		Array(allocator, size);
	}


	template<typename T>
	Array<T>& Array<T>::operator=(Array<T> other){
		RAINDROP_profile_function();
		swap(*this, other);

		return *this;
	}

	template<typename T>
	void swap(Array<T>& x, Array<T>& y){
		RAINDROP_profile_function();
		using std::swap;

		swap(x._allocator, y._allocator);
		swap(x._data, y._data);
		swap(x._size, y._size);
		swap(x._capacity, y._capacity);
	}

	template<typename T>
	T& Array<T>::operator[](usize i){
		RAINDROP_profile_function();
		return _data[i];
	}

	template<typename T>
	const T& Array<T>::operator[](usize i) const{
		RAINDROP_profile_function();
		return _data[i];
	}

	template<typename T>
	void Array<T>::push(T value){
		RAINDROP_profile_function();
		if(_size + 1 > _capacity)
			grow();

		_data[_size] = std::move(value);
		_size++;
	}

	template<typename T>
	void Array<T>::push(Array<T>&& arr){
		RAINDROP_profile_function();
		push(arr.size(), arr.data());
	}
	
	template<typename T>
	void Array<T>::push(Array<T>& arr){
		RAINDROP_profile_function();
		push(arr.size(), arr.data());
	}

	template<typename T>
	void Array<T>::push(usize count, const T* values){
		RAINDROP_profile_function();
		if(_size + count > _capacity)
			grow();

		usize start_size = _size;

		for(usize i = 0; i < count; i++)
			_data[start_size + i] = values[i];

		_size += count;
	}

	template<typename T>
	void Array<T>::pop(){
		RAINDROP_profile_function();
		_size--;
	}

	template<typename T>
	void Array<T>::clear(){
		RAINDROP_profile_function();
		resize(0);
	}

	template<typename T>
	bool Array<T>::reserve(usize new_capacity){
		RAINDROP_profile_function();
		if(new_capacity > _capacity)
			return setCapacity(new_capacity);

		return true;
	}

	template<typename T>
	bool Array<T>::resize(usize new_size){
		RAINDROP_profile_function();
		if(new_size > _capacity){
			if(!grow(new_size))
				return false;
		}

		_size = new_size;

		return true;
	}

	template<typename T>
	bool Array<T>::resize(usize new_size, const T& value){
		RAINDROP_profile_function();
		usize size = _size;

		resize(new_size);

		for(usize i = size; i < _size; i++)
			_data[i] = value;

		return true;
	}

	template<typename T>
	bool Array<T>::setCapacity(usize new_capacity){
		RAINDROP_profile_function();
		if(new_capacity == _capacity)
			return true;

		if(new_capacity < _size)
			resize(new_capacity);

		T* new_data = nullptr;

		if(new_capacity > 0){
			new_data = allocateArray<T>(*_allocator, new_capacity);
			memcpy(new_data, _data, sizeof(T)*_size);
		}

		if(_data != nullptr)
			_allocator->deallocate(_data);

		_data     = new_data;
		_capacity = new_capacity;

		return true;
	}

	template<typename T>
	bool Array<T>::grow(usize min_capacity){
		RAINDROP_profile_function();
		usize new_capacity = _capacity * 2 + sizeof(T);

		if(new_capacity < min_capacity)
			new_capacity = min_capacity;

		return setCapacity(new_capacity);
	}

	template<typename T>
	bool Array<T>::empty() const{
		RAINDROP_profile_function();
		return _size == 0;
	}

	template<typename T>
	usize Array<T>::size() const{
		RAINDROP_profile_function();
		return _size;
	}

	template<typename T>
	usize Array<T>::capacity() const{
		RAINDROP_profile_function();
		return _capacity;
	}
}

#endif