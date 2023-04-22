#ifndef __RAINDROP_CORE_DEBUG_LOGGER_HPP__
#define __RAINDROP_CORE_DEBUG_LOGGER_HPP__

#include <common.hpp>
#include <cstdio>

namespace Raindrop::Core::Debug::Log{
	enum class Severity : uint8{
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL,

		ERR = ERROR,
	};

	enum class Cause : uint8{
		SYSTEM,
		MEMORY,
		GRAPHICS,
		NETWORKING,
		AI,
		AUDIO,
		PHYSICS,
		TOOLS,
		UI,
		API,
		ARITMETIC,
		ASSERT,
		THREAD,
		IO,
		OTHER,
		SCENE,
	};

	RAINDROP_API const char* severityToStr(Severity severity);
	RAINDROP_API const char* causeToStr(Cause cause);

	struct Log{
		Log(){}

		Severity severity;
		Cause cause;

		char message[1048];
		const char* file;
		const char* function;
		const char* condition;
	};

	class RAINDROP_API LogModule{
		friend class __Logger;
		public:
			LogModule() = default;
			virtual ~LogModule() = default;

			virtual void callback(const Log &msg) = 0;
		
		protected:

			/**
			 * @brief push the module into the logger, cannot be pushed mutiple times
			 * 
			 */
			void push();

			/**
			 * @brief pop the mofule from the logger, cannot be poped if the module is not pushed
			 * 
			 */
			void pop();

		private:
			LogModule* next;
	};

	// output message
	RAINDROP_API void initialize();
	RAINDROP_API void shutdownLog();
	RAINDROP_API void log(const Log &msg);
	
	class __Logger;
}

// internal
RAINDROP_MODULE void __RAINDROP_log_setContext(Raindrop::Core::Debug::Log::__Logger* logger);
RAINDROP_MODULE Raindrop::Core::Debug::Log::__Logger* __RAINDROP_log_getContext();

namespace Raindrop::Core::Debug{
	void inline log(const Log::Log &msg){
		Log::log(msg);	
	}
}

#define RAINDROP_log(_severity, _cause, ...) {								\
	::Raindrop::Core::Debug::Log::Log __log__;								\
	__log__.severity = Raindrop::Core::Debug::Log::Severity::_severity;		\
	__log__.cause = Raindrop::Core::Debug::Log::Cause::_cause;				\
	__log__.function = __PRETTY_FUNCTION__;									\
	__log__.file = __FILE__;												\
	__log__.condition = "none";												\
	sprintf(__log__.message, __VA_ARGS__);									\
	Raindrop::Core::Debug::log(__log__);									\
}

#define RAINDROP_cond_log(_cond, _severity, _cause, ...) {					\
	if (_cond){																\
		::Raindrop::Core::Debug::Log::Log __log__;							\
		__log__.severity = Raindrop::Core::Debug::Log::Severity::_severity;	\
		__log__.cause = Raindrop::Core::Debug::Log::Cause::_cause;			\
		__log__.function = __PRETTY_FUNCTION__;								\
		__log__.file = __FILE__;											\
		__log__.condition = #_cond;											\
		sprintf(__log__.message, __VA_ARGS__);								\
		Raindrop::Core::Debug::log(__log__);								\
	}																		\
}

#endif