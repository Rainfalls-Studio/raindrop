#include "render/context/opengl/Opengl_Context.hpp"
#include "window/SDL2_Window.hpp"

#include <glad/glad.h>

namespace rnd::render::context::opengl{
	void Opengl_Context::init(window::Window* window){
		PROFILE_FUNCTION();

		switch (window->getAPI()){
			case window::API::SDL2: initWithSDL2(window); return;
			case window::API::GLWF: initWithGLFW(window); return;
		}

		RND_ERR("opengl context initialization", "the given window has an invalid API");
		throw std::runtime_error("opengl context initialization : the given window has an invalid API");
	}
	
	void Opengl_Context::initWithSDL2(window::Window* w){
		PROFILE_FUNCTION();
		window::SDL2_Window* window = (window::SDL2_Window*)w;

		context = SDL_GL_CreateContext(window->getWindow());
		int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

		if (status == 0){
			RND_ERR("opengl SDL2 Context creation", "failed to create a context");
			throw std::domain_error("opengl SDL2 Context creation : failed to create a context");
		}

		RND_LOG("opengl info : Vendor : ", glGetString(GL_VENDOR), " | renderer : ", glGetString(GL_RENDERER), " | version : ", glGetString(GL_VERSION));
	}

	void Opengl_Context::initWithGLFW(window::Window* w){
		PROFILE_FUNCTION();
		RND_ERR("opengl Context creation", "GLFW is not available for now opengl context");
	}
}