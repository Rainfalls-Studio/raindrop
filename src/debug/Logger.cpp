#include "debug/Logger.hpp"
#include <stdlib.h>

namespace rnd::debug{
	Logger Logger::instance;

	inline const char* info(){
		return "INFO ::";
	}

	inline const char* warn(){
		return "WARNING ::";
	}

	inline const char* error(){
		return "ERROR ::";
	}

	inline const char* critical(){
		return "CRITICAL ERROR ::";
	}

	const char* levelToStr(LogLevel level){
		switch (level){
			case Info: return info();
			case Warn: return warn();
			case Error: return error();
			case Critical: return critical();
		}
		return "UNKNOWN LOG LEVEL :: ";
	}

	void Logger::allow(LogLevel level, bool allowed){
		allowedLogs ^= (-allowed ^ allowedLogs) & (1UL << level);
	}

	bool Logger::isAllowed(LogLevel level){
		return (allowedLogs >> level) & 1U;
	}

	void Logger::log(LogLevel level, const char* msg, const char* reason, const char* file, const char* fnc, int line){
		if (!isAllowed(level)) return;

		fprintf(out, "%s %s\n", levelToStr(level), msg);
		fprintf(out, "reason : %s\n", reason);
		fprintf(out, "from :: file : \"%s\" | function : \"%s\" | line %d\n", file, fnc, line);
		fprintf(out, "\n\n");
		fflush(out);
	}

	void Logger::init(const char* filepath){
		fileOwner = true;

		out = fopen(filepath, "w");
		if (out == nullptr){
			fprintf(stderr, "failed to open / create \"%s\" file", filepath);
			exit(EXIT_FAILURE);
		}
	}

	void Logger::init(FILE* out){
		fileOwner = false;
		this->out = out;
	}

	void Logger::shutdown(){
		if (fileOwner){
			fclose(out);
		}
		out = nullptr;
	}

	Logger& Logger::getInstance(){
		return instance;
	}
}