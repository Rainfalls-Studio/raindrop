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

void threadFnc(rnd::events::Mediator* mediator, rnd::Barrier* barrier){

	for (int i=0; i<150; i++){
		mediator->trigger(i);

		barrier->wait(); // frame update
		barrier->wait(); // loop restart
	}
}

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::events::Mediator eventMediator;
	eventMediator.init(2, 2);
	RND_LOG("initialize", "success");

	#if true // registring test
		for (int i=0; i<250; i++){
			const char name[] = {(char)i, '\0'};
			eventMediator.registerEvent(name);
		}
		RND_LOG("register", "success");
	#endif
	
	#if true // subscribtion test
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
	#endif

	#if true // trigger and update
		for (int i=0; i<50; i++){

			eventMediator.trigger(i);
		}
		RND_LOG("trigger", "success");

		eventMediator.update();
		RND_LOG("update", "success");
	#endif

	#if true // multithreading test

		rnd::Barrier barrier(2);
		rnd::Thread thread(&threadFnc, &eventMediator, &barrier);

		for (int i=0; i<150; i++){
			eventMediator.update(0);
			eventMediator.update(1);

			barrier.wait();
			eventMediator.nextFrame();
			barrier.wait();
		}
		
		thread.join();

		RND_LOG("multithreading", "success");
	#endif

	#if true // unsubscribing
		for (int i=0; i<250; i+=10){
			eventMediator.unsubscribe(i, fnc1);
		}
		RND_LOG("unsubscribe", "success");
	#endif

	eventMediator.shutdown();
	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}