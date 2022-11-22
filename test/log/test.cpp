#include "debug/Logger.hpp"

int main(int argc, char** argv){
	RND_LOG_INIT("out.log");

	RND_LOG("test");
	RND_WARN("test");
	RND_ERR("test");
	RND_CRITIC("test");

	RND_LOG_SHUTDOWN();
	return 0;
}
