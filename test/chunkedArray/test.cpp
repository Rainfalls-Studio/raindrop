#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::ChunkedArray<int> arr;
	arr.init();
	RND_LOG("initialization", "success");

	arr.reserve(5000);
	RND_LOG("reserve", "success");

	for (int i=0; i<500; i++){
		arr.push(i);
	}
	RND_LOG("push", "sucess");

	arr.clear();
	RND_LOG("clear", "success");

	arr.shutdown();
	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}