/**
 * @brief this file contain chunked array data structure.
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

#ifndef __RAINDROP_MEMORY_CHUNKED_ARRAY_HPP__
#define __RAINDROP_MEMORY_CHUNKED_ARRAY_HPP__

#include "core.hpp"
#include "debug/Instrumentor.hpp"
#include "allocators/Allocator.hpp"
#include <math.h>
#include <pthread.h>

namespace rnd::memory{

	template<typename T, uint32_t size>
	struct ChunkedArrayChunk{
		ChunkedArrayChunk* next = nullptr;
		T data[size];

		T& get(uint32_t id){
			return data[id];
		}
	};

	template<typename T, template<typename, bool> typename Allocator = allocators::Allocator, bool threadSafe = true, uint32_t chunkSize = 150>
	class ChunkedArray{
		public:
			using Chunk = ChunkedArrayChunk<T, chunkSize>;
			using FullAllocator = Allocator<Chunk, threadSafe>;

			ChunkedArray() = default;
			~ChunkedArray() = default;

			template<typename ...Args>
			void init(uint32_t size = 0, Args... args){
				PROFILE_FUNCTION();

				allocator = (FullAllocator*)malloc(sizeof(FullAllocator));
				allocator->init(args...);
				externAllocator = false;

				if constexpr(threadSafe) pthread_mutex_init(&lock, nullptr);

				reserve(size);
			}

			void init(uint32_t size, FullAllocator* allocator){
				PROFILE_FUNCTION();
				externAllocator = true;
				this->allocator = allocator;
				reserve(size);
				if constexpr(threadSafe) pthread_mutex_init(&lock, nullptr);
			}

			void shutdown(){
				PROFILE_FUNCTION();

				if (externAllocator){
					allocator->shutdown();
					free(allocator);
				}
				allocator = nullptr;
				_front = nullptr;
				_back = nullptr;
				_size = 0;
				chunkCount = 0;
			}

			void resize(uint32_t newSize){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);

				_size = newSize;

				if (newSize > _size){
					uint32_t addCount = newSize - _size;
					reserve(addCount);
				} else {
					popCount(_size - newSize);
				}

				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

			void reserve(uint32_t count){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				_size += count;
				int chunkCount = std::ceil((float)count / (float)chunkSize);

				for (int i=0; i<chunkCount; i++){
					createChunk();
				}
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

			void push(const T &t){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				uint32_t chunkId, localId;
				getChunkAndPos(_size, chunkId, localId);

				if (chunkId >= chunkCount){
					createChunk();
				}

				_back->get(localId) = t;
				_size++;
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

			uint32_t size() const{
				PROFILE_FUNCTION();
				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				uint32_t _size = this->_size;
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
				return _size;
			}

			bool empty() const{
				PROFILE_FUNCTION();
				return size() == 0;
			}

			void clear(){
				PROFILE_FUNCTION();
				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				while (chunkCount > 0){
					pop();
				}
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}
		
		private:
			FullAllocator* allocator = nullptr;
			bool externAllocator = false;

			uint32_t _size = 0;
			uint32_t chunkCount = 0;

			Chunk* _back = nullptr;
			Chunk* _front = nullptr;

			pthread_mutex_t lock;

			Chunk& allocateChunk(){
				PROFILE_FUNCTION();
				Chunk& chunk = allocator->allocate();
				chunk.next = nullptr;
				return chunk;
			}

			Chunk& createChunk(){
				PROFILE_FUNCTION();
				Chunk& chunk = allocateChunk();

				if (_back){
					_back->next = &chunk;
				} else {
					_front = &chunk;
				}

				_back = &chunk;
				chunkCount++;
				return chunk;
			}

			void pop(){
				PROFILE_FUNCTION();

				if (_front){
					Chunk* chunk = _front;
					_front = _front->next;
					allocator->deallocate(*chunk);
					chunkCount--;
				}
			}

			void popCount(uint32_t size){
				PROFILE_FUNCTION();

				int chunkCount = ((float)size / (float)chunkSize);
				for (int i=0; i<chunkCount; i++){
					pop();
				}
			}

			void getChunkAndPos(uint32_t id, uint32_t &chunkID, uint32_t &localID){
				PROFILE_FUNCTION();
				
				chunkID = ((float)id / (float)chunkSize);
				localID = id - (chunkID * chunkSize);

			}
	};
}

#endif