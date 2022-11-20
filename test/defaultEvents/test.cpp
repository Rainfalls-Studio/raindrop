#include "raindrop.hpp"

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::events::Mediator eventMediator;
	eventMediator.init();
	RND_LOG("initialize", "success");

	rnd::events::defaults::init(eventMediator);
	RND_LOG("default event initialize", "success");

	eventMediator.shutdown();
	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}