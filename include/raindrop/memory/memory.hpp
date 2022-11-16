
/**
 * @brief thes file contain the fondamental memory functions.
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

#ifndef __RAINDROP_MEMORY_MEMORY_HPP__
#define __RAINDROP_MEMORY_MEMORY_HPP__

#include "core.hpp"

namespace rnd::memory{

	// allocation
	void* malloc(uint32_t size);
	void* calloc(uint32_t count, uint32_t size);
	void* realloc(void* memory, uint32_t newSize);
	void free(void* memory);

	// memory modifier
	void* memcpy(void* dst, const void* src, uint32_t n);
	errno_t memcpy_s(void* dst, uint32_t dstS, const void* src, uint32_t size);
	void* mempcpy(void* dst, const char* src, uint32_t n);
	void* memset(void* dst, int32_t val, size_t size);
	int memcmp(const void* buf1, const void* buf2, uint32_t size);
	
	uint32_t getMemStat(const char* fnc);
	uint32_t getLeakedMemoryCount();
	bool isMemoryLeaked();
}

#endif