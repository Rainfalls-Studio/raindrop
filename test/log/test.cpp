#include "debug/Logger.hpp"

int main(int argc, char** argv){
	LOG_INIT("out.log");

	LOG_ALLOW(rnd::debug::Info, false);

	LOG("test", "");
	WARN("test", "");
	ERR("test", "");
	CRITIC("test", "");

	LOG_SHUTDOWN();
	return 0;
}
