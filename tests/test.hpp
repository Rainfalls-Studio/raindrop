#ifndef __TEST_HPP__
#define __TEST_HPP__

#include <Raindrop.hpp>
#include "gtest/gtest.h"

static constexpr Raindrop::usize MEMORY_O = 1;
static constexpr Raindrop::usize MEMORY_KO = MEMORY_O * 1024;
static constexpr Raindrop::usize MEMORY_MO = MEMORY_KO * 1024;
static constexpr Raindrop::usize MEMORY_GO = MEMORY_MO * 1024;

#define STRUCT(x) struct struct ## x{char a[x];}; static_assert(sizeof(struct ## x) == x)

STRUCT(4);
STRUCT(8);
STRUCT(16);
STRUCT(32);
STRUCT(64);
STRUCT(128);
STRUCT(256);
STRUCT(512);
STRUCT(1024);

#ifndef NLOG
	class LogModule : public ::Raindrop::Core::Debug::Log::LogModule{
		public:
			using Log = ::Raindrop::Core::Debug::Log::Log;
			virtual void callback(const Log &msg) override{
				printf("%s :: %s : %s\n", Raindrop::Core::Debug::Log::severityToStr(msg.severity), Raindrop::Core::Debug::Log::causeToStr(msg.cause), msg.message);
			}

			LogModule(){
				push();
			}

			~LogModule(){
				pop();
			}
	};

	LogModule __log__;
#endif

#endif