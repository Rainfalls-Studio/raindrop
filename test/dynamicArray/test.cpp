#include "raindrop.hpp"

int main(int argc, char** argv){

	LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::DynamicArray array;
	array.init(sizeof(int), 500);

	for (int i=0; i<500; i++){
		void* ptr = array.get(i);
	}

	array.shutdown();
	LOG("end", "end successfully");

	PROFILE_END_SESSION();
	LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}