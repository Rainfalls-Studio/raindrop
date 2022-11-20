#include "window/Window.hpp"
#include "window/SDL2_Window.hpp"


namespace rnd::window{
	Window* Window::create(API api){
		switch (api){
			case API::SDL2: return new SDL2_Window();
			// case API::GLWF:
		}
		RND_ASSERT(false, "unknown window API");
		return nullptr;
	}

	void Window::event(events::Mediator& mediator){
		SDL2_Window::event(mediator);
	}

}