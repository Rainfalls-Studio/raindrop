#include "raindrop.hpp"

int main(int argc, char** argv){

	LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::DynamicArray array;

	const int size = 500;
	array.init<int>(size, 50);


	for (int i; i<size; i++){
		int* a = array.get<int>(i);
		*a = i;
	}

	
	int* a = array.get<int>(476);
	printf("%d\n", *a);

	for (int i=0; i<100; i++){
		array.push<int>(5);
	}

	LOG("end", "end successfully");

	array.shutdown();

	PROFILE_END_SESSION();
	LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}