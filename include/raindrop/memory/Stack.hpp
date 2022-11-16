/**
 * @brief this file contain stack data structure. can contain all wanted types
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

#ifndef __RAINDROP_MEMORY_STACK_HPP__
#define __RAINDROP_MEMORY_STACK_HPP__

#include "core.hpp"

namespace rnd::memory{
	class Stack{
		public:
			Stack() = default;
			~Stack() = default;

			void init(uint32_t size);
			void shutdown();

			void reset(uint32_t to = 0);

			uint32_t size();

			void* allocate(uint32_t size);

			template<typename T>
			T& allocate(){
				return *(T*)allocate(sizeof(T));
			}
			
		private:
			char* memory = nullptr;
			char* ptr = nullptr;

			uint32_t usedSize = 0;
			uint32_t maxSize = 0;

	};
}

#endif