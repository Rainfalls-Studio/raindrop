#include "render/context/vulkan/Vulkan_Context.hpp"
#include "window/SDL2_Window.hpp"

#include <SDL2/SDL_vulkan.h>

namespace rnd::render::context::vulkan{
	void Vulkan_Context::init(window::Window* window){
		PROFILE_FUNCTION();

		allocCallback.init();
		instance.init(this);

		RND_RUNTIME_ERR("vulkan context initialization, unknown window api");
	}

	DynamicArray<const char*> Vulkan_Context::getWindowExtensions(){
		PROFILE_FUNCTION();

		switch (window->getAPI()){
			case window::API::SDL2: return getSDL2WindowExtensions();
			case window::API::GLWF: return getGLFWWindowExtensions();
		}
		
		RND_RUNTIME_ERR("vulkan context initialization, window API vulkan extention getter, unknown window api");
	}

	DynamicArray<const char*> Vulkan_Context::getSDL2WindowExtensions(){
		PROFILE_FUNCTION();

		SDL_Window* win = ((window::SDL2_Window*)window)->getWindow();

		unsigned int count = 0;
		SDL_Vulkan_GetInstanceExtensions(win, &count, nullptr);

		std::vector<const char*> extensions(count);
		SDL_Vulkan_GetInstanceExtensions(win, &count, extensions.data());

		if (validationlayers){
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	DynamicArray<const char*> Vulkan_Context::getGLFWWindowExtensions(){
		PROFILE_FUNCTION();
		RND_RUNTIME_ERR("GLFW is not available for now with a vulkan context");
	}

	void Vulkan_Context::createSurface(VkInstance& instance, VkSurfaceKHR& surface){
		PROFILE_FUNCTION();

		bool result;
		switch (window->getAPI()){
			case window::API::SDL2: createSDL2Surface(instance, surface); return;
			case window::API::GLWF: createGLFWSurface(instance, surface); return;
		}
		
		RND_RUNTIME_ERR("vulkan context initialization, window surface creation, unknown window api");
	}

	void Vulkan_Context::createSDL2Surface(VkInstance& instance, VkSurfaceKHR& surface){
		PROFILE_FUNCTION();
		if (SDL_Vulkan_CreateSurface(((window::SDL2_Window*)window)->getWindow(), instance, &surface) == SDL_FALSE){
			RND_RUNTIME_ERR("vulkan context initialization, window surface creation, failed to create a surface with SDL2");
		}
	}

	void Vulkan_Context::createGLFWSurface(VkInstance& instance, VkSurfaceKHR& surface){
		PROFILE_FUNCTION();
		RND_RUNTIME_ERR("GLFW is not available for now with a vulkan context");
	}
}