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
 * Portions of this software are licensed under the LGPL license OpenAL Soft Copyright (C) 1991 Free Software Foundation, Inc. 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 */

#ifndef __RAINDROP_MEMORY_FORWARD_LINKED_LIST_HPP__
#define __RAINDROP_MEMORY_FORWARD_LINKED_LIST_HPP__

#include "core.hpp"
#include "Allocator.hpp"

namespace rnd::memory{
	class ForwardLinkedList{
		public:
			ForwardLinkedList() = default;
			~ForwardLinkedList() = default;

			void init(Allocator* allocator, uint32_t elementSize);
			void init(uint32_t elementSize);

			void shutdown();

			void push(void* ptr);
			void pop();

			void* front();
			void* back();

			void clear();

			uint32_t size();
			bool empty();

		private:
			Allocator* allocator = nullptr;
			bool customAllocator = false;

			struct Node{
				Node* next = nullptr;
				// data stored here

				void* get();
			};

			Node* begin = nullptr;
			Node* end = nullptr;

			uint32_t elementSize = 0;
			uint32_t nodeCount = 0;

			Allocator* createAllocator();
	};
}

#endif