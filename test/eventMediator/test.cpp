#include "raindrop.hpp"

bool fnc1(void* ptr){
	printf("fnc1\n");
	return false;
}

bool fnc2(void* ptr){
	printf("fnc2\n");
	return false;
}

bool mtd1(void* instance, void* ptr){
	printf("mtd1\n");
	return false;
}

bool mtd2(void* instance, void* ptr){
	printf("mtd2\n");
	return false;
}

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::events::Mediator eventMediator;
	eventMediator.init();
	RND_LOG("initialize", "success");

	for (int i=0; i<250; i++){
		const char name[] = {(char)i, '\0'};
		eventMediator.registerEvent<int, int>(name);
	}
	RND_LOG("register", "success");
	
	for (int i=0; i<500; i++){
		eventMediator.subscribe(i / 2, fnc1);
	}

	for (int i=0; i<250; i+=2){
		eventMediator.subscribe(i, fnc2);
	}
	RND_LOG("function subscription", "success");

	for (int i=0; i<100; i++){
		eventMediator.subscribe(i, mtd1, &i);
	}

	for (int i=0; i<50; i++){
		eventMediator.subscribe(i, mtd2, &i);
	}
	RND_LOG("methode subscription", "success");

	eventMediator.shutdown();
	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}