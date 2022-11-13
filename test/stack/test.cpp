#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::Stack stack;
	stack.init(10000);

	for (int i=0; i<500; i++){
		stack.allocate(rand() % 25);
	}

	stack.reset();

	stack.shutdown();

	RND_LOG("end", "end successfully");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}