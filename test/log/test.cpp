#include "debug/Logger.hpp"

int main(int argc, char** argv){
	LOG_INIT("out.log");

	LOG_ALLOW(rnd::debug::Info, false);

	LOG(rnd::debug::Info, "test", "");
	LOG(rnd::debug::Warn, "test", "");
	LOG(rnd::debug::Error, "test", "");
	LOG(rnd::debug::Critical, "test", "");

	LOG_SHUTDOWN();
	return 0;
}
