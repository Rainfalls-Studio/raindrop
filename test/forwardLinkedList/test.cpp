#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::memory::ForwardLinkedList list;
	list.init<int>();
	
	for (int i=0; i<50; i++){
		list.push<int>(i);
	}

	for (int i=0; i<65; i++){
		list.pop();
	}

	for (int i=0; i<100000; i++){
		int a = rand() % 2;
		if (a == 1){
			list.push<int>(i);
		} else {
			list.pop();
		}
	}

	list.shutdown();

	RND_LOG("end", "end successfully");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}