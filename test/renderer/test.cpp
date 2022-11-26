#include "raindrop.hpp"

bool onQuit(void* data, void* ptr){
	bool* launched = (bool*)ptr;
	*launched = false;
}

int main(int argc, char** argv){

	RND_LOG_INIT("out.log");
	PROFILE_BEGIN_SESSION("test", "profile.json");
	PROFILE_RECORD();

	rnd::events::Mediator eventMediator;
	eventMediator.init();
	RND_LOG("initialize", "success");

	rnd::events::defaults::init(eventMediator);
	RND_LOG("default event initialize", "success");

	rnd::window::Window window;
	window.init({1080, 720}, "title", rnd::window::Resizable);
	RND_LOG("window initialization", "success");

	rnd::render::Renderer renderer;
	renderer.init(window);
	RND_LOG("renderer initilization", "success");

	bool launched = true;
	eventMediator.subscribe(rnd::events::defaults::getID(rnd::events::defaults::QUIT), &onQuit, &launched);

	while (launched){
		rnd::window::Window::event(eventMediator);

		eventMediator.update();
		eventMediator.nextFrame();

		renderer.render();
		// _sleep(16);
	}
	RND_LOG("window events", "success");

	window.shutdown();
	RND_LOG("window shutdown", "success");

	eventMediator.shutdown();
	RND_LOG("shutdown", "success");

	PROFILE_END_SESSION();
	RND_LOG_SHUTDOWN();

	return EXIT_SUCCESS;
}