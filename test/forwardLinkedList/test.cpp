#include "raindrop.hpp"

int main(int argc, char** argv){

	LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::ForwardLinkedList list;
	list.init(sizeof(int));
	
	for (int i=0; i<50; i++){
		list.push(&i);
	}

	for (int i=0; i<65; i++){
		list.pop();
	}

	for (int i=0; i<1000; i++){
		int a = rand() % 2;
		if (a == 0){
			list.push(&i);
		} else {
			list.pop();
		}
	}

	list.shutdown();

	LOG("end", "end successfully");

	PROFILE_END_SESSION();
	LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}