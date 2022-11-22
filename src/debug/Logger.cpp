#include "debug/Logger.hpp"
#include <stdlib.h>

rnd::debug::Logger rnd::debug::Logger::instance;

namespace rnd::debug{

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
		return "UNKNOWN RND_LOG LEVEL :: ";
	}

	void Logger::allow(LogLevel level, bool allowed){
		allowedLogs ^= (-allowed ^ allowedLogs) & (1UL << level);
	}

	bool Logger::isAllowed(LogLevel level){
		return (allowedLogs >> level) & 1U;
	}
	
	void Logger::begin(LogLevel level, const char* file, const char* fnc, int line){
		out << levelToStr(level) << " \"" << file << "\"; \"" << fnc << "\"; " << line;
	}

	void Logger::end(){
		out << '\n';
		out.flush();
	}

	void Logger::init(const char* filepath){
		file.open(filepath);

		if (!file.is_open()){
			fprintf(stderr, "LOGGER ERROR : Faild to open : %s\n", filepath);
			throw std::runtime_error("LOGGER ERROR : Faild to open output file");
		}

		out.rdbuf(file.rdbuf());
	}

	void Logger::shutdown(){
		if (file.is_open()){
			file.close();
		}
	}

	void Logger::init(std::ostream out){
		this->out.rdbuf(out.rdbuf());
	}

	Logger& Logger::getInstance(){
		return instance;
	}
}