#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::Stack stack;
	stack.init(10000);
	RND_LOG("initialization", "success");

	for (int i=0; i<500; i++){
		stack.allocate(rand() % 25);
	}
	RND_LOG("allocation", "success");

	stack.reset();
	RND_LOG("reset", "success");

	stack.shutdown();
	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}