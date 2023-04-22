#include "Core/Debug/logger.hpp"
#include "Core/Debug/debug.hpp"

namespace Raindrop::Core::Debug::Log{
	class __Logger{
		public:
			__Logger(){
				_begin = nullptr;
				_size = 0;
			}

			__Logger(const __Logger& other){
				_begin = other._begin;
				_size = other._size;
			}

			~__Logger(){
				_begin = nullptr;
				_size = 0;
			}

			void pushModule(LogModule* m){
				RAINDROP_assert(!isPushed(m) && "cannot push the same module mutiple times");

				m->next = _begin;
				_begin = m;
				_size++;
			}

			void popModule(LogModule* m){
				RAINDROP_assert(isPushed(m) && "cannot pop a non existant module");

				LogModule* node = _begin;
				LogModule* before = nullptr;

				while (node){
					if (node == m){
						if (before){
							before->next = node->next;
						} else {
							_begin = node->next;
						}
						_size--;
						break;
					}

					before = node;
					node = node->next;
				}

				m->next = nullptr;
			}

			// check if a module is already pushed
			bool isPushed(LogModule* m){
				LogModule* node = _begin;

				while (node){
					if (node == m) return true;
					node = node->next;
				}
				return false;
			}

			void log(const Log &msg){
				LogModule* node = _begin;

				while (node){
					node->callback(msg);
					node = node->next;
				}
			}
		
		private:
			LogModule* _begin;
			uint32 _size;
	};

	static __Logger* __logger;

	RAINDROP_API void initialize(){
		__logger = new __Logger();
	}

	RAINDROP_API void shutdownLog(){
		delete __logger;
		__logger = nullptr;
	}

	RAINDROP_API void LogModule::push(){
		if (!__logger) return;
		__logger->pushModule(this);
	}

	RAINDROP_API void LogModule::pop(){
		if (!__logger) return;
		__logger->popModule(this);
	}
	
	RAINDROP_API void log(const Log &msg){
		if (!__logger) return;
		__logger->log(msg);
	}
	
	RAINDROP_API const char* severityToStr(Severity severity){
		switch (severity){
			case Severity::CRITICAL: return "CRITICAL";
			case Severity::DEBUG: return "DEBUG";
			case Severity::ERROR: return "ERROR";
			case Severity::INFO: return "INFO";
			case Severity::TRACE: return "TRACE";
			case Severity::WARNING: return "WARNING";
		}
		RAINDROP_log(WARNING, OTHER, "cannot convert the given severity value into a string, unknown severity (id : %d)", (int)severity);
		return "";
	}

	RAINDROP_API const char* causeToStr(Cause cause){
		switch (cause){
			case Cause::API: return "API";
			case Cause::ARITMETIC: return "ARITHEMETIC";
			case Cause::ASSERT: return "ASSERT";
			case Cause::IO: return "IO";
			case Cause::MEMORY: return "MEMORY";
			case Cause::OTHER: return "OTHER";
			case Cause::SYSTEM: return "SYSTEM";
			case Cause::THREAD: return "THREAD";
			case Cause::AI: return "AI";
			case Cause::AUDIO: return "AUDIO";
			case Cause::GRAPHICS: return "GRAPHICS";
			case Cause::NETWORKING: return "NETWORKING";
			case Cause::PHYSICS: return "PHYSICS";
			case Cause::TOOLS: return "TOOLS";
			case Cause::UI: return "UI";
			case Cause::SCENE: return "SCENE";
		}
		RAINDROP_log(WARNING, OTHER, "cannot convert the given cause value into a string, unknown cause (id : %d)", (int)cause);
		return "";
	}
}

RAINDROP_MODULE void  __RAINDROP_log_setContext(Raindrop::Core::Debug::Log::__Logger* logger){
	Raindrop::Core::Debug::Log::__logger = logger;
}

RAINDROP_MODULE Raindrop::Core::Debug::Log::__Logger* __RAINDROP_log_getContext(){
	return Raindrop::Core::Debug::Log::__logger;
}