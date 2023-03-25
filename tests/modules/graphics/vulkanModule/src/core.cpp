#include "core.hpp"

static SDL_Window* initWindow = nullptr;

extern "C"{
	bool RAINDROP_MODULE initialize(){
		RAINDROP_profile_function();
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			RAINDROP_log(ERROR, GRAPHICS, "failed to initialize SDL2 : %s\n", SDL_GetError());
			return false;
		}

		initWindow = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN | SDL_WINDOW_SKIP_TASKBAR);
		if (!initWindow){
			RAINDROP_log(ERROR, GRAPHICS, "failed to create SDL2 window : %s", SDL_GetError());
			return false;
		}
		return true;
	}

	void RAINDROP_MODULE shutdown(){
		RAINDROP_profile_function();
		if (SDL_WasInit(SDL_INIT_VIDEO)){
			if (initWindow) SDL_DestroyWindow(initWindow);
			SDL_Quit();
		}
	}
}

SDL_Window* getInitWindow(){
	RAINDROP_profile_function();
	return initWindow;
}
