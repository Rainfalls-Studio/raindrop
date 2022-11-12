#include "raindrop.hpp"

int main(int argc, char** argv){

	LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::Stack stack;
	stack.init(10000);

	for (int i=0; i<500; i++){
		stack.allocate(rand() % 25);
	}

	stack.reset();

	stack.shutdown();

	LOG("end", "end successfully");

	PROFILE_END_SESSION();
	LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}