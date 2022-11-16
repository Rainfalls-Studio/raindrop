/**
 * @brief this file contain forward linked list structure. (First in, first out)
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING.
 * @authors @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

/**
 * MIT License
 * Copyright (c) 2022 Rainfalls studio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 

 * 
 */

#ifndef __RAINDROP_MEMORY_FORWARD_LINKED_LIST_HPP__
#define __RAINDROP_MEMORY_FORWARD_LINKED_LIST_HPP__

#include "core.hpp"
#include "debug/Instrumentor.hpp"
#include "allocators/Allocator.hpp"
#include <pthread.h>

namespace rnd::memory{

	template<typename T>
	struct ForwardLinkedListNode{
		T data;
		ForwardLinkedListNode* next = nullptr;
	};

	/**
	 * @brief a forward linked list, useful in cases where a lot of pushes and pops are made and whene indexing is not needed
	 * 
	 * @tparam T the type to store
	 * @tparam threadSafe true if threadsafe is activated for this list
	 * @tparam Allocator the allocator, can accelerate the push and pop methodes, the allocator has to be for ForwardLinkedListNode<T> type
	 */
	template<typename T, template<typename A, bool> typename Allocator = allocators::Allocator, bool threadSafe = true>
	class ForwardLinkedList{
		public:
			using FullAllocator = Allocator<ForwardLinkedListNode<T>, threadSafe>;

			ForwardLinkedList() = default;
			~ForwardLinkedList() = default;

			template<typename ...Args>
			void init(Args... args){
				PROFILE_FUNCTION();

				externalAllocator = false;
				this->allocator = (FullAllocator*)malloc(sizeof(FullAllocator));
				this->allocator->init(args...);

				if constexpr(threadSafe) pthread_mutex_init(&lock, nullptr);
			}

			void init(FullAllocator *allocator){
				PROFILE_FUNCTION();
				externalAllocator = true;
				this->allocator = allocator;

				if constexpr(threadSafe) pthread_mutex_init(&lock, nullptr);
			}

			void shutdown(){
				PROFILE_FUNCTION();

				if (!externalAllocator){
					free(allocator);
				}
				allocator = nullptr;

				if constexpr(threadSafe) pthread_mutex_destroy(&lock);
			}

			void push(const T &t){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				Node& node = allocateNode();
				node.next = nullptr;

				if (_back){
					_back->next = &node;
				} else {
					_front = &node;
				}

				_back = &node;
				count++;
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

			void pop(){
				PROFILE_FUNCTION();
				
				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				if (_front){
					Node* node = _front;
					_front = _front->next;
					allocator->deallocate(*node);
					count--;
				}
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);

			}

			T& front(){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				T& data = _front->data;
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
				return data;
			}

			T& back(){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				T& data = _back->data;
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);

				return data;
			}

			uint32_t size() const{
				PROFILE_FUNCTION();
				return count;
			}

			bool empty() const{
				PROFILE_FUNCTION();
				return size() == 0;
			}

			void clear(){
				PROFILE_FUNCTION();
				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				while (!empty()){
					pop();
				}
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

		private:
			using Node = ForwardLinkedListNode<T>;

			Node* _front = nullptr;
			Node* _back = nullptr;
			uint32_t count = 0;

			FullAllocator* allocator = nullptr;
			bool externalAllocator = false;

			pthread_mutex_t lock;

			Node& allocateNode(){
				PROFILE_FUNCTION();
				Node &node = allocator->allocate();
				node.next = nullptr;
				return node;
			}
	};

}

#endif