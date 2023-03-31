#ifndef __RAINDROP_CORE_MEMORY_LIST_HPP__
#define __RAINDROP_CORE_MEMORY_LIST_HPP__

#include "Core/Memory/allocators/Allocator.hpp"
#include "Core/Debug/profiler.hpp"
#include <utility>

namespace Raindrop::Core::Memory{
	template<typename T>
	class List{
		private:
			struct Node{
				Node* next;
				T data;
			};

		public:
			class Iterator{
				friend List;
				public:
					Iterator(Iterator &other);

					Iterator& operator++();
					Iterator operator++(int x);

					operator bool() const;
					bool operator==(const Iterator &other) const;

					T& operator*();
					T* operator->();
					const T& operator*() const;
					const T* operator->() const;

				private:
					Node* _node;

					Node* node() const;
					Iterator(Node* node);
			};

			List(Allocator& allocator);
			List(const List& other);
			List(List&& other);

			~List();

			List& operator=(List other);

			void push(T value);
			void push(const List& other);
			void pop();
			void clear();
			void remove(Iterator node);

			usize size() const;
			bool empty() const;

			template<typename A>
			friend void swap(List<A>& x, List<A>& y);

			Iterator front();
		
		private:
			Allocator* _allocator;
			usize _size;
			Node* _begin;

			Node* allocateNode();
			void deallocateNode(Node* node);
			void pushNode(Node* node);
	};

	template<typename T>
	List<T>::List(Allocator& allocator) : _allocator(&allocator){
		RAINDROP_profile_function();
		_size = 0;
		_begin = nullptr;
	}

	template<typename T>
	List<T>::List(const List& other) : _allocator(other._allocator){
		RAINDROP_profile_function();
		push(other);
	}

	template<typename T>
	List<T>::List(List&& other) : _allocator(other._allocator){
		RAINDROP_profile_function();
		push(other);
	}

	template<typename T>
	List<T>::~List(){
		RAINDROP_profile_function();
		clear();
	}
	
	template<typename T>
	usize List<T>::size() const{
		RAINDROP_profile_function();
		return _size;
	}

	template<typename T>
	bool List<T>::empty() const{
		RAINDROP_profile_function();
		return _size == 0;
	}

	template<typename T>
	void List<T>::push(T value){
		RAINDROP_profile_function();
		Node* node = allocateNode();
		node->data = value;
		pushNode(node);
	}
	
	template<typename T>
	void List<T>::push(const List& other){
		RAINDROP_profile_function();
		Node* node = other._begin;
		while (node){

			Node* copy = allocateNode();
			copy->data = node->data;
			pushNode(copy);

			node = node->next;
		}
	}
	
	template<typename T>
	void List<T>::pushNode(Node* node){
		RAINDROP_profile_function();
		node->next = _begin;
		_begin = node;
		_size++;
	}
	
	template<typename T>
	void List<T>::pop(){
		RAINDROP_profile_function();
		if (_begin){
			Node* before = _begin;
			_begin = _begin->next;
			_size--;
			deallocateNode(before);
		}
	}

	template<typename T>
	void List<T>::clear(){
		RAINDROP_profile_function();
		while (!empty()){
			pop();
		}

		// ensure that the size is 0
		_size = 0;
	}

	template<typename T>
	void List<T>::remove(Iterator it){
		RAINDROP_profile_function();
		Node* curr = _begin;
		Node* before = nullptr;
		while (curr){

			if (curr == it._node){
				if (before){
					before->next = curr->next;
				}
				_begin = curr->next;
				_size--;
				deallocateNode(curr);
				break;
			}
			
			before = curr;
			curr = curr->next;
		}
	}


	template<typename T>
	typename List<T>::Iterator List<T>::front(){
		return Iterator(_begin);
	}

	template<typename T>
	typename List<T>::Node* List<T>::allocateNode(){
		RAINDROP_profile_function();
		Node* node = (Node*)_allocator->allocate(sizeof(Node), alignof(Node));
		node->next = nullptr;
		return node;
	}

	template<typename T>
	void List<T>::deallocateNode(Node* node){
		RAINDROP_profile_function();
		_allocator->deallocate(node);
	}

	template<typename T>
	void swap(List<T>& x, List<T>& y){
		RAINDROP_profile_function();
		using std::swap;
		swap(x._allocator, y._allocator);
		swap(x._begin, y._begin);
		swap(x._size, y._size);
	}
	
	template<typename T>
	List<T>::Iterator::Iterator(Iterator &other){
		RAINDROP_profile_function();
		_node = other._node;
	}

	template<typename T>
	List<T>::Iterator::Iterator(Node* node){
		RAINDROP_profile_function();
		_node = node;
	}
	
	template<typename T>
	typename List<T>::Iterator& List<T>::Iterator::operator++(){
		RAINDROP_profile_function();
		if (_node) _node = _node->next;
		return *this;
	}
	
	template<typename T>
	T& List<T>::Iterator::operator*(){
		RAINDROP_profile_function();
		return _node->data;
	}
	
	template<typename T>
	T* List<T>::Iterator::operator->(){
		RAINDROP_profile_function();
		return &_node->data;
	}

	template<typename T>
	const T& List<T>::Iterator::operator*() const{
		RAINDROP_profile_function();
		return _node->data;
	}
	
	template<typename T>
	const T* List<T>::Iterator::operator->() const{
		RAINDROP_profile_function();
		return &_node->data;
	}
	
	template<typename T>
	List<T>::Iterator::operator bool() const{
		RAINDROP_profile_function();
		return _node != nullptr;
	}
	
	template<typename T>
	bool List<T>::Iterator::operator==(const Iterator &other) const{
		RAINDROP_profile_function();
		return _node == other._node;
	}

	template<typename T>
	typename List<T>::Node* List<T>::Iterator::node() const{
		RAINDROP_profile_function();
		return _node;
	}

	template<typename T>
	typename List<T>::Iterator List<T>::Iterator::operator++(int x){
		return ++(*this);
	}
}

#endif