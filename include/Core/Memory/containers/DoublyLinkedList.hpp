#ifndef __RAINDROP_CORE_MEMORY_DOUBLY_LINKED_LIST_HPP__
#define __RAINDROP_CORE_MEMORY_DOUBLY_LINKED_LIST_HPP__

#include "Core/Memory/allocators/Allocator.hpp"
#include "Core/Debug/profiler.hpp"
#include <utility>

namespace Raindrop::Core::Memory{
    template<typename T>
    class DoublyLinkedList{
		public:
			struct Node{
				T _data;
				Node* _next;
				Node* _prev;
			};

			DoublyLinkedList(Allocator& allocator);
			~DoublyLinkedList();

			bool empty() const;
			usize lenght() const;

			void pushFront(const T& value);
			void pushBack(const T& value);

			void popFront();
			void popBack();
			void clear();
			void erase(Node* node);


			T& front();
			T& back();
			const T& front() const;
			const T& back() const;

			Node* begin();
			Node* end();

		private:
			Allocator& _allocator;
			Node* _begin;
			Node* _end;
			usize _lenght;

			Node* allocateNode();
			void deallocateNode(Node* node);
	};

	template<typename T>
	DoublyLinkedList<T>::DoublyLinkedList(Allocator& allocator) : _allocator{allocator}{
		RAINDROP_profile_function();
		_begin = nullptr;
		_end = nullptr;
		_lenght = 0;
	}

	template<typename T>
	DoublyLinkedList<T>::~DoublyLinkedList(){
		RAINDROP_profile_function();
		clear();
	}

	template<typename T>
	bool DoublyLinkedList<T>::empty() const{
		RAINDROP_profile_function();
		return _lenght == 0;
	}
	
	template<typename T>
	usize DoublyLinkedList<T>::lenght() const{
		RAINDROP_profile_function();
		return _lenght;
	}

	template<typename T>
	void DoublyLinkedList<T>::pushFront(const T& value){
		RAINDROP_profile_function();
		Node* newNode = allocateNode();
		newNode->_next = _begin;
		newNode->_prev = nullptr;
		newNode->_data = value;
		
		if (_begin){
			_begin->_prev = newNode;
		} else {
			_end = newNode;
		}

		_begin = newNode;
		_lenght++;
	}
	
	template<typename T>
	void DoublyLinkedList<T>::pushBack(const T& value){
		RAINDROP_profile_function();
		Node* newNode = allocateNode();
		newNode->_next = nullptr;
		newNode->_prev = _end;
		newNode->_data = value;
		
		if (_end){
			_end->_next = newNode;
		} else {
			_begin = newNode;
		}

		_end = newNode;
		_lenght++;
	}

	template<typename T>
	void DoublyLinkedList<T>::popFront(){
		RAINDROP_profile_function();
		if (!empty()){
			Node* oldBegin = _begin;
			_begin = oldBegin->_next;

			if (_begin){
				_begin->_prev = nullptr;
			} else {
				_end = nullptr;
			}

			deallocateNode(oldBegin);
			_lenght--;
		}
	}

	template<typename T>
	void DoublyLinkedList<T>::popBack(){
		RAINDROP_profile_function();
		if (!empty()){
			Node* oldEnd = _end;
			_end = oldEnd->_prev;

			if (_end){
				_end->_next = nullptr;
			} else {
				_begin = nullptr;
			}

			deallocateNode(oldEnd);
			_lenght--;
		}
	}

	template<typename T>
	T& DoublyLinkedList<T>::front(){
		RAINDROP_profile_function();
		return _begin->_data;
	}
	
	template<typename T>
	T& DoublyLinkedList<T>::back(){
		RAINDROP_profile_function();
		return _end->_data;
	}

	template<typename T>
	const T& DoublyLinkedList<T>::front() const{
		RAINDROP_profile_function();
		return _begin->_data;
	}

	template<typename T>
	const T& DoublyLinkedList<T>::back() const{
		RAINDROP_profile_function();
		return _end->_data;

	}

	template<typename T>
	typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::allocateNode(){
		RAINDROP_profile_function();
		return Memory::allocateNew<Node>(_allocator);
	}

	template<typename T>
	void DoublyLinkedList<T>::deallocateNode(Node* node){
		RAINDROP_profile_function();
		return Memory::deallocateDelete(_allocator, *node);
	}

	template<typename T>
	void DoublyLinkedList<T>::clear(){
		RAINDROP_profile_function();
		while (!empty()){
			popFront();
		}
	}

	template<typename T>
	void DoublyLinkedList<T>::erase(Node* node){
		if (!node) {
			return;
		}

		if (node == _begin) {
			popFront();
		}
		else if (node == _end) {
			popBack();
		}
		else {
			node->_prev->_next = node->_next;
			node->_next->_prev = node->_prev;
			deallocateNode(node);
			_lenght--;
		}
	}

	template<typename T>
	typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::begin(){
		return _begin;
	}

	template<typename T>
	typename DoublyLinkedList<T>::Node* DoublyLinkedList<T>::end(){
		return _end;
	}
}

#endif