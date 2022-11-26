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

 * 
 */

#ifndef __RAINDROP_CORE_HPP__
#define __RAINDROP_CORE_HPP__

#include "pch.hpp"

#define RND_MAJOR 0
#define RND_MINOR 1
#define RND_PATCH 0

#define RND_DEVELOPMENT_STAGE "pre-alpha"

#define RND__FUNC__ __func__
#define RND__COMPLETE_FUNC__ __PRETTY_FUNCTION__
#define RND__FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// debug
#define RND_PROFILE true
#define RND_LOGGER true
#define RND_MEMORY_CHECK true

#define RND_LOG_LEVEL true
#define RND_WARN_LEVEL true
#define RND_ERR_LEVEL true
#define RND_CRITIC_LEVEL true

#define RND_ASSERTS true

#include "memory/memory.hpp"
#include "memory/Stack.hpp"
#include "events/events.hpp"
#include "events/keys.hpp"
#include "debug/Logger.hpp"
#include "debug/Instrumentor.hpp"
#include "thread/Barrier.hpp"

// macros

#if defined(RND_LOG_LEVEL) && RND_LOG_LEVEL == true
	#define RND_LOG(...) RND_LOG_LOG(rnd::debug::Info, __VA_ARGS__)
#else
	#define RND_LOG(...)
#endif

#if defined(RND_WARN_LEVEL) && RND_WARN_LEVEL == true
	#define RND_WARN(...) RND_LOG_LOG(rnd::debug::Warn, __VA_ARGS__)
#else
	#define RND_WARN(...)
#endif

#if defined(RND_ERR_LEVEL) && RND_ERR_LEVEL == true
	#define RND_ERR(...) RND_LOG_LOG(rnd::debug::Error, __VA_ARGS__)
#else
	#define RND_ERR(...)
#endif

#if defined(RND_CRITIC_LEVEL) && RND_CRITIC_LEVEL == true
	#define RND_CRITIC(...) RND_LOG_LOG(rnd::debug::Critical, __VA_ARGS__)
#else
	#define RND_CRITIC(...)
#endif

#if defined(RND_ASSERTS) && RND_ASSERTS == true
	#define RND_ASSERT(x, msg) {if(!(x)) {RND_ERR("Assertion Failed: ", msg);__debugbreak();}}
#else
	#define RND_ASSERT(x, msg)
#endif

#define rnd_max(a, b) a > b ? a : b
#define rnd_min(a, b) a < b ? a : b

#define RND_RUNTIME_ERR(...) RND_ERR(__VA_ARGS__); throw std::runtime_error(std::string(__VA_ARGS__))

namespace rnd{
	template<typename T, typename Allocator = std::allocator<T>>
	using DynamicArray = std::vector<T>;
	template<typename T, size_t size>
	using Array = std::array<T, size>;
	template<typename T, typename Allocator = std::allocator<T>>
	using List = std::list<T, Allocator>;
	template<typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = std::allocator<std::pair<const Key, T>>>
	using Map = std::map<Key, T, Compare, Allocator>;
	template<typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T>>
	using Set = std::set<T, Compare, Allocator>;
	template<typename T, typename Allocator = std::allocator<T>>
	using Deque = std::deque<T, Allocator>;
	template<typename T, typename Sequence = Deque<T>>
	using Queue = std::queue<T, Sequence>;
	template<size_t growSize = 1024>
	using Stack = memory::Stack;
	template<typename T, typename Container = DynamicArray<T>, typename Compare = std::less<typename Container::value_type>>
	using PriorityQueue = std::priority_queue<T, Container, Compare>;
	template<typename Key, typename T,typename Hash = std::hash<T>, typename Compare = std::less<Key>, typename Allocator = std::allocator<std::pair<const Key, T>>>
	using UnorderedMap = std::unordered_map<Key, T, Hash, Compare, Allocator>;
	template<typename T, typename Hash = std::hash<T>, typename Compare = std::equal_to<T>, typename Allocator = std::allocator<T>>
	using UnorderedSet = std::unordered_set<T, Hash, Compare, Allocator>;
	template<size_t size>
	using Bitset = std::bitset<size>;

	using Thread = std::thread;
	using Mutex = std::mutex;
	using Barrier = thread::Barrier;
	using ConditionVariable = std::condition_variable;

	using Extent = glm::u32vec2;

	struct Version{
		uint32_t major;
		uint32_t minor;
		uint32_t patch;
	};
}

#endif