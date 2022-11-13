#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::DynamicArray array;
	array.init<int>(500);

	for (int i=0; i<500; i++){
		int a = array.get<int>(i);
	}

	array.reserve(50);

	array.shutdown();
	RND_LOG("end", "end successfully");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}