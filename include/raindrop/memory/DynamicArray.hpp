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
#include "Allocator.hpp"

namespace rnd::memory{
	class DynamicArray{
		public:
			DynamicArray() = default;
			~DynamicArray() = default;

			void init(Allocator* allocator, uint32_t elementSize, uint32_t size, uint32_t elementPerChunk = 50);
			void init(uint32_t elementSize, uint32_t size, uint32_t elementPerChunk = 50);

			void shutdown();

			uint32_t size();
			bool empty();

			void* get(uint32_t index);

		private:
			struct Chunk{
				Chunk(DynamicArray* owner);

				Chunk* next = nullptr;
				DynamicArray* owner = nullptr;

				void* get(uint32_t index);
			};

			uint32_t elementSize = 0;
			uint32_t elementPerChunk = 0;
			Allocator* allocator = nullptr;
			bool customAllocator = false;
			uint32_t chunkCount = 0;

			Chunk* begin = nullptr;
			Chunk* end = nullptr;

			void createAllocator(uint32_t size);

			void pushBack(Chunk* chunk);
			void pushChunks(uint32_t count);
			Chunk* allocateChunk();
			Chunk* getChunk(uint32_t id);
	};
}

#endif