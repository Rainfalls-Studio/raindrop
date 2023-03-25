#include "logger.hpp"
#include "debug.hpp"

namespace Raindrop::Core::Debug::Log{
	class Logger{
		public:
			Logger(){
				_begin = nullptr;
				_size = 0;
			}

			Logger(const Logger& other){
				_begin = other._begin;
				_size = other._size;
			}

			~Logger(){
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

	static Logger logger;

	void LogModule::push(){
		logger.pushModule(this);
	}

	void LogModule::pop(){
		logger.popModule(this);
	}
	
	void log(const Log &msg){
		logger.log(msg);
	}
	
	const char* severityToStr(Severity severity){
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

	const char* causeToStr(Cause cause){
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
		}
		RAINDROP_log(WARNING, OTHER, "cannot convert the given cause value into a string, unknown cause (id : %d)", (int)cause);
		return "";
	}
}

extern "C"{
	void RAINDROP_MODULE __RAINDROP_log_setContext(const Raindrop::Core::Debug::Log::Logger& __logger){
		Raindrop::Core::Debug::Log::logger = __logger;
	}

	const Raindrop::Core::Debug::Log::Logger& RAINDROP_MODULE __RAINDROP_log_getContext(){
		return Raindrop::Core::Debug::Log::logger;
	}
}