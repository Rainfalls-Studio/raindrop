/**
 * @brief this file contain debug logging classes. The logger is a singleton
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

#ifndef __RAINDROP_DEBUG_LOGGER_HPP__
#define __RAINDROP_DEBUG_LOGGER_HPP__

#include "core.hpp"
#include <stdio.h>
#include <mutex>

namespace rnd::debug{
	enum LogLevel{
		Info = 0,
		Warn = 1,
		Warning = Warn,
		Error = 2,
		Critical = 3,
	};

	class Logger{
		public:
			Logger() = default;
			~Logger() = default;

			void init(const char* filepath);
			void init(FILE* out);

			void shutdown();

			void allow(LogLevel level, bool allowed);
			bool isAllowed(LogLevel level);

			void log(LogLevel level, const char* msg, const char* reason = "none", const char* file = "unknown", const char* fnc = "unknown", int line = -1);

			static Logger& getInstance();
		
		private:
			FILE* out = nullptr;
			bool fileOwner = false;
			uint8_t allowedLogs = UINT8_MAX;
			std::mutex lock;

			static Logger instance;
	};
}

#if defined(RND_LOGGER) && RND_LOGGER == true
	#define RND_LOG_INIT(arg) ::rnd::debug::Logger::getInstance().init(arg)
	#define RND_LOG_SHUTDOWN() ::rnd::debug::Logger::getInstance().shutdown()
	#define RND_LOG_ALLOW(level, allowed) ::rnd::debug::Logger::getInstance().allow(level, allowed)
	#define RND_LOG_IS_ALLOWED(level, allowed) ::rnd::debug::Logger::getInstance().isAllowed(level)
	#define RND_LOG_LOG(level, msg, reason) ::rnd::debug::Logger::getInstance().log(level, msg, reason, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#else
	#define RND_LOG_INIT(arg)
	#define RND_LOG_SHUTDOWN() 
	#define RND_LOG_ALLOW(level, allowed)
	#define RND_LOG_IS_ALLOWED(level, allowed)
	#define RND_LOG_LOG(level, msg, reason)
#endif

#endif