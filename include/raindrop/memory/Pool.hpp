/**
 * @brief this file contain memory pool dara structure.
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
 * Portions of this software are licensed under the LGPL license OpenAL Soft Copyright (C) 1991 Free Software Foundation, Inc. 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 */

#ifndef __RAINDROP_MEMORY_POOL_HPP__
#define __RAINDROP_MEMORY_POOL_HPP__

#include "core.hpp"

namespace rnd::memory{
	class Pool{
		public:
			Pool() = default;
			~Pool() = default;

			void init(uint32_t instanceSize, uint32_t instanceCount);

			template<typename T>
			void init(uint32_t instanceCount){
				init(sizeof(T), instanceCount);
			}

			void shutdown();

			template<typename T>
			T* pop(){
				Chunk* chunk = popOneChunk();
				if (!chunk) return nullptr;
				return chunk->getData<T>();
			}

			template<typename T>
			void push(T* t){
				RND_ASSERT(sizeof(T) == instanceSize, "invalid type pushed into the pool");
				Chunk* chunk = (Chunk*)(((char*)t)-1);
			
				Chunk* next = begin;
				begin = chunk;
				chunk->next = begin;
				instanceCount++;
			}

			void reserve(uint32_t count);
			
			uint32_t getSize();
			bool empty();

		private:
			struct Chunk{
				Chunk* next = nullptr;

				template<typename T>
				T* getData(){
					return (T*)((char*)this + sizeof(Chunk*));
				}
			};

			Chunk* begin = nullptr;

			uint32_t instanceSize = 0;
			uint32_t instanceCount = 0;

			void destroy();
			void pushChunks(uint32_t count);
			void pushOneChunk();
			Chunk* allocOneChunk();

			Chunk* popOneChunk();

			bool destroyOneChunk();
			void destroyChunks(uint32_t count);

			uint32_t getChunkSize();
	};
}

#endif