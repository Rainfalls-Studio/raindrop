/**
 * @brief this file contain dynamic array data structure.
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

#ifndef __RAINDROP_MEMORY_DYNAMIC_ARRAY_HPP__
#define __RAINDROP_MEMORY_DYNAMIC_ARRAY_HPP__

#include "core.hpp"

namespace rnd::memory{
	class DynamicArray{
		public:
			DynamicArray() = default;
			~DynamicArray() = default;

			void init(uint32_t instanceSize, uint32_t instanceCount, uint32_t instancePerChunk = 50);

			template<typename T>
			void init(uint32_t instanceCount, uint32_t instancePerChunk = 50){
				init(sizeof(T), instanceCount, instancePerChunk);
			}

			void shutdown();

			template<typename T>
			T* get(uint32_t index){
				return (T*)getFromIndex(index);
			}

			void resize(uint32_t size);

		private:
			struct Chunk{
				Chunk* next = nullptr;

				void* get(uint32_t index, uint32_t instanceSize);

				// data stored here
			};

			Chunk* begin = nullptr;
			
			uint32_t instanceSize = 0;
			uint32_t instancePerChunk = 0;
			uint32_t chunkSize = 0;
			uint32_t chunkCount = 0;

			void pushChunk();
			void pushChunks(uint32_t count);
			Chunk* allocChunk();
			void* getFromIndex(uint32_t index);
			Chunk* getChunkFromIndex(uint32_t index);
			void popFromHere(Chunk* start);

	};
}

#endif