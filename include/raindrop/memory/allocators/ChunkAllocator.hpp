
/**
 * @brief thes file contain the data structure allocator, a critical part of the engine.
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
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

#ifndef __RAINDROP_MEMORY_ALLOCATORS_CHUNK_ALLOCATOR_HPP__
#define __RAINDROP_MEMORY_ALLOCATORS_CHUNK_ALLOCATOR_HPP__

#include "core.hpp"
#include <pthread.h>
#include <math.h>
#include "debug/Instrumentor.hpp"

namespace rnd::memory::allocators{

	template<typename T, bool threadSafe = true, uint32_t instancePerChunk = 50>
	class ChunkAllocator{
		public:
			ChunkAllocator() = default;
			~ChunkAllocator() = default;

			ChunkAllocator(ChunkAllocator &) = delete;
			ChunkAllocator& operator=(ChunkAllocator &) = delete;

			void init(uint32_t size){
				PROFILE_FUNCTION();

				begin = nullptr;
				buffers = nullptr;

				if constexpr(threadSafe) pthread_mutex_init(&lock, nullptr);
				
				resize(size);
			}

			void shutdown(){
				PROFILE_FUNCTION();

				freeChunks();
				if constexpr(threadSafe) pthread_mutex_destroy(&lock);
			}

			T& allocate(){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				Buffer* buffer = popBuffer();
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
				return buffer->data;
			}

			void deallocate(T& t){
				PROFILE_FUNCTION();
				Buffer* buffer = (Buffer*)&t;

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				pushBuffer(buffer);
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

			void clear(){
				PROFILE_FUNCTION();
				
				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				freeChunks();
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

			void resize(uint32_t size){
				PROFILE_FUNCTION();

				if constexpr(threadSafe) pthread_mutex_lock(&lock);
				int chunkCount = std::ceil((float)size / (float)instancePerChunk);
				freeChunks();
				createChunks(chunkCount);
				if constexpr(threadSafe) pthread_mutex_unlock(&lock);
			}

		private:
			struct Buffer{
				union{
					Buffer* next = nullptr;
					T data;
				};
			};

			struct Chunk{
				Chunk* next = nullptr;
				Buffer buffers[instancePerChunk];
			};

			Chunk* begin = nullptr;
			Buffer* buffers = nullptr;

			pthread_mutex_t lock;

			Chunk* allocateChunk(){
				PROFILE_FUNCTION();
				Chunk* chunk = (Chunk*)malloc(sizeof(Chunk));
				chunk->next = nullptr;
				return chunk;
			}

			void pushChunk(Chunk* chunk){
				PROFILE_FUNCTION();

				chunk->next = begin;
				begin = chunk;

				Buffer* buffer = chunk->buffers;
				for (int i=0; i<instancePerChunk; i++){
					pushBuffer(buffer);
					buffer++;
				}
			}

			void createChunk(){
				PROFILE_FUNCTION();
				pushChunk(allocateChunk());
			}

			void createChunks(uint32_t count){
				PROFILE_FUNCTION();
				for (int i=0; i<count; i++){
					createChunk();
				}
			}

			void pushBuffer(Buffer* buffer){
				PROFILE_FUNCTION();
				buffer->next = buffers;
				buffers = buffer;
			}

			Buffer* popBuffer(){
				PROFILE_FUNCTION();
				if (!buffers) createChunk();
				Buffer* buffer = buffers;
				buffers = buffers->next;
				return buffer;
			}

			void freeChunks(){
				PROFILE_FUNCTION();

				Chunk* chunk = begin;

				while (chunk){
					Chunk* current = chunk;
					chunk = chunk->next;
					free(current);
				}

				begin = nullptr;
				buffers = nullptr;
			}
	};
}

#endif