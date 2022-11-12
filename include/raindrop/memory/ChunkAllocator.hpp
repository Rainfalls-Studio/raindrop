
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
 * Portions of this software are licensed under the LGPL license OpenAL Soft Copyright (C) 1991 Free Software Foundation, Inc. 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 */

#ifndef __RAINDROP_MEMORY_CHUNK_ALLOCATOR_HPP__
#define __RAINDROP_MEMORY_CHUNK_ALLOCATOR_HPP__

#include "core.hpp"
#include "Allocator.hpp"

namespace rnd::memory{
	class ChunkAllocator : public Allocator{
		public:
			ChunkAllocator() : Allocator() {};
			~ChunkAllocator() = default;

			void init(uint32_t dataSize, uint32_t size, uint32_t instancePerChunk = 50);

			template<typename T>
			void init(uint32_t size, uint32_t instancePerChunk = 50){
				init(sizeof(T), size, instancePerChunk);
			}

			virtual void shutdown() override;
			virtual void* allocate() override;
			virtual void deallocate(void* ptr) override;

			void clear();

		private:
			struct Buffer{
				Buffer* next = nullptr;
			};

			struct Chunk{
				Chunk* next = nullptr;
				Buffer* get();
			};

			Chunk* begin = nullptr;
			Buffer* buffers = nullptr;

			uint32_t dataSize = 0;
			uint32_t instancePerChunk = 0;

			Chunk* allocateChunk();
			void pushChunk();
			void pushChunks(uint32_t count);
			void pushBuffer(Buffer* buffer);

			void freeChunks();
	};
}

#endif