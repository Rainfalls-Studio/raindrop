/**
 * @brief this file contain main preprocessor computation of raindrop, this is a critic file of raindrop
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @attention IF YOU DO ANY CHANGE IN THIS FILE, MAKE SURE THAT THE WHOLE PROJECT IS RECOMPILED
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

#ifndef __RAINDROP_CORE_HPP__
#define __RAINDROP_CORE_HPP__

#include <stdint.h>

#define __FUNC__ __func__

// debug
#define PROFILE true
#define LOGGER true
#define MEMORY_CHECK true

#define LOG_LEVEL true
#define WARN_LEVEL true
#define ERR_LEVEL true
#define CRITIC_LEVEL true

#define RND_ASSERTS true

#include "memory/memory.hpp"
#include "debug/Logger.hpp"

// macros

#if defined(LOG_LEVEL) && LOG_LEVEL == true
	#define LOG(...) LOG_LOG(rnd::debug::Info, __VA_ARGS__)
#else
	#define LOG(...)
#endif

#if defined(WARN_LEVEL) && WARN_LEVEL == true
	#define WARN(...) LOG_LOG(rnd::debug::Warn, __VA_ARGS__)
#else
	#define WARN(...)
#endif

#if defined(ERR_LEVEL) && ERR_LEVEL == true
	#define ERR(...) LOG_LOG(rnd::debug::Error, __VA_ARGS__)
#else
	#define ERR(...)
#endif

#if defined(CRITIC_LEVEL) && CRITIC_LEVEL == true
	#define CRITIC(...) LOG_LOG(rnd::debug::Critical, __VA_ARGS__)
#else
	#define CRITIC(...)
#endif

#if defined(RND_ASSERTS) && RND_ASSERTS == true
	#define RND_ASSERT(x, msg) {if(!(x)) {ERR("Assertion Failed: ", msg);__debugbreak();}}
#else
	#define RND_ASSERT(x, msg)
#endif

#define rnd_max(a, b) a > b ? a : b
#define rnd_min(a, b) a < b ? a : b

#endif