#include "window/SDL2_Window.hpp"

namespace rnd::window{
	static uint32_t windowCount = 0;

	SDL_Window* newWindow(const char *title, int x, int y, int w, int h, Uint32 flags){
		PROFILE_FUNCTION();

		if (windowCount == 0){
			PROFILE_SCOPE("SDL_Init");
			if (SDL_Init(SDL_INIT_VIDEO) != 0){
				std::string err = SDL_GetError();
				RND_ERR("SDL_Init", err.c_str());
				throw std::domain_error(err);
			}
		}

		SDL_Window* window = nullptr;
		{
			PROFILE_SCOPE("SDL_CreateWindow");
			window = SDL_CreateWindow(title, x, y, w, h, flags);
			if (!window){
				std::string err = SDL_GetError();
				RND_ERR("SDL_CreateWindow", err.c_str());
				throw std::domain_error(err);
			}
		}

		windowCount++;
		return window;
	}

	void destroyWindow(SDL_Window* window){
		PROFILE_FUNCTION();
		RND_ASSERT(window != nullptr, "cannot destroy an invalid window");

		{
			PROFILE_SCOPE("SDL_DestroyWindow");
			SDL_DestroyWindow(window);
			windowCount--;
		}

		if (windowCount == 0){
			PROFILE_SCOPE("SDL_Quit");
			SDL_Quit();
		}
	}

	uint32_t RndFlagsToSDLFlags(uint32_t flags){
		PROFILE_FUNCTION();
		uint32_t out = 0;
		if (flags & Fullscreen) out |= SDL_WINDOW_FULLSCREEN;
		if (flags & Borderless) out |= SDL_WINDOW_BORDERLESS;
		if (flags & Hidden) out |= SDL_WINDOW_HIDDEN;
		if (flags & Resizable) out |= SDL_WINDOW_RESIZABLE;
		if (flags & AllowHighDPI) out |= SDL_WINDOW_ALLOW_HIGHDPI;
		if (flags & HideTaskbar) out |= SDL_WINDOW_SKIP_TASKBAR;
		if (flags & Tooltip) out |= SDL_WINDOW_TOOLTIP;
		if (flags & Popup) out |= SDL_WINDOW_POPUP_MENU;
		if (flags & Utility) out |= SDL_WINDOW_UTILITY;
		return out;
	}

	void SDL2_Window::init(Extent extent, const char* title, uint32_t flags){
		PROFILE_FUNCTION();
		handle = newWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, extent.x, extent.y, RndFlagsToSDLFlags(flags));
		SDL_SetWindowData(handle, "owner", this);

	}

	void SDL2_Window::shutdown(){
		PROFILE_FUNCTION();
		destroyWindow(handle);
		handle = nullptr;
	}

	void SDL2_Window::fullscreen(bool state){
		PROFILE_FUNCTION();
		SDL_SetWindowFullscreen(handle, state ? SDL_WINDOW_FULLSCREEN : 0);
	}

	void SDL2_Window::resizable(bool state){
		PROFILE_FUNCTION();
		SDL_SetWindowResizable(handle, (SDL_bool)state);
	}

	void SDL2_Window::minimize(){
		PROFILE_FUNCTION();
		SDL_MinimizeWindow(handle);
	}

	void SDL2_Window::maximize(){
		PROFILE_FUNCTION();
		SDL_MaximizeWindow(handle);
	}

	void SDL2_Window::focus(){
		PROFILE_FUNCTION();
		SDL_SetWindowInputFocus(handle);
	}

	void SDL2_Window::show(){
		PROFILE_FUNCTION();
		SDL_ShowWindow(handle);
	}

	void SDL2_Window::hide(){
		PROFILE_FUNCTION();
		SDL_HideWindow(handle);
	}

	glm::i32vec2 SDL2_Window::getPosition() const{
		PROFILE_FUNCTION();
		int x, y;
		SDL_GetWindowPosition(handle, &x, &y);
		return glm::i32vec2(x, y);
	}

	const char* SDL2_Window::getTitle() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowTitle(handle);
	}

	Extent SDL2_Window::getExtent() const{
		PROFILE_FUNCTION();
		int w, h;
		SDL_GetWindowSize(handle, &w, &h);
		return Extent(w, h);
	}
	
	Extent SDL2_Window::getMaxExtent() const{
		PROFILE_FUNCTION();
		int w, h;
		SDL_GetWindowMaximumSize(handle, &w, &h);
		return Extent(w, h);
	}

	Extent SDL2_Window::getMinExtent() const{
		PROFILE_FUNCTION();
		int w, h;
		SDL_GetWindowMinimumSize(handle, &w, &h);
		return Extent(w, h);
	}

	float SDL2_Window::getOpacity() const{
		PROFILE_FUNCTION();
		float opacity = 0.f;
		SDL_GetWindowOpacity(handle, &opacity);
		return opacity;
	}

	bool SDL2_Window::isFullscreen() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_FULLSCREEN;
	}

	bool SDL2_Window::isBorderless() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_BORDERLESS;
	}

	bool SDL2_Window::isHidden() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_HIDDEN;
	}

	bool SDL2_Window::isResizable() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_RESIZABLE;
	}

	bool SDL2_Window::isHighDPIAllowed() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_ALLOW_HIGHDPI;
	}

	bool SDL2_Window::isAlwaysOnTop() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_ALWAYS_ON_TOP;
	}

	bool SDL2_Window::isTaskBarHidden() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_SKIP_TASKBAR;
	}

	bool SDL2_Window::isTooltip() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_TOOLTIP;
	}

	bool SDL2_Window::isPopup() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_POPUP_MENU;
	}

	bool SDL2_Window::isUtility() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags(handle) & SDL_WINDOW_UTILITY;
	}

	void onWindowEvent(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();

		switch (event.window.event){
			case SDL_WINDOWEVENT_SHOWN: break;
			case SDL_WINDOWEVENT_HIDDEN: break;
			case SDL_WINDOWEVENT_EXPOSED: break;
			case SDL_WINDOWEVENT_RESIZED: break;
			case SDL_WINDOWEVENT_SIZE_CHANGED: break;
			case SDL_WINDOWEVENT_MINIMIZED: break;
			case SDL_WINDOWEVENT_MAXIMIZED: break;
			case SDL_WINDOWEVENT_RESTORED: break;
			case SDL_WINDOWEVENT_ENTER: break;
			case SDL_WINDOWEVENT_LEAVE: break;
			case SDL_WINDOWEVENT_FOCUS_GAINED: break;
			case SDL_WINDOWEVENT_FOCUS_LOST: break;
			case SDL_WINDOWEVENT_CLOSE: break;
			case SDL_WINDOWEVENT_TAKE_FOCUS: break;
			case SDL_WINDOWEVENT_HIT_TEST: break;
			case SDL_WINDOWEVENT_DISPLAY_CHANGED: break;
		}
	}

	void SDL2_Window::event(events::Mediator& mediator){
		PROFILE_FUNCTION();

		SDL_Event event;
		while (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT: break;
				case SDL_APP_TERMINATING: break;
				case SDL_APP_LOWMEMORY: break;
				case SDL_APP_WILLENTERBACKGROUND: break;
				case SDL_APP_DIDENTERBACKGROUND: break;
				case SDL_APP_WILLENTERFOREGROUND: break;
				case SDL_APP_DIDENTERFOREGROUND: break;
				case SDL_LOCALECHANGED: break;
				case SDL_DISPLAYEVENT: break;
				case SDL_WINDOWEVENT: onWindowEvent(event, mediator); break;
				case SDL_SYSWMEVENT: break;
				case SDL_KEYDOWN: break;
				case SDL_KEYUP: break;
				case SDL_TEXTEDITING: break;
				case SDL_TEXTINPUT: break;
				case SDL_KEYMAPCHANGED: break;
				case SDL_MOUSEMOTION: break;
				case SDL_MOUSEBUTTONDOWN: break;
				case SDL_MOUSEBUTTONUP: break;
				case SDL_MOUSEWHEEL: break;
				case SDL_JOYAXISMOTION: break;
				case SDL_JOYBALLMOTION: break;
				case SDL_JOYHATMOTION: break;
				case SDL_JOYBUTTONDOWN: break;
				case SDL_JOYBUTTONUP: break;
				case SDL_JOYDEVICEADDED: break;
				case SDL_JOYDEVICEREMOVED: break;
				case SDL_CONTROLLERAXISMOTION: break;
				case SDL_CONTROLLERBUTTONDOWN: break;
				case SDL_CONTROLLERBUTTONUP: break;
				case SDL_CONTROLLERDEVICEADDED: break;
				case SDL_CONTROLLERDEVICEREMOVED: break;
				case SDL_CONTROLLERDEVICEREMAPPED: break;
				case SDL_CONTROLLERTOUCHPADDOWN: break;
				case SDL_CONTROLLERTOUCHPADMOTION: break;
				case SDL_CONTROLLERTOUCHPADUP: break;
				case SDL_CONTROLLERSENSORUPDATE: break;
				case SDL_FINGERDOWN: break;
				case SDL_FINGERUP: break;
				case SDL_FINGERMOTION: break;
				case SDL_DOLLARGESTURE: break;
				case SDL_DOLLARRECORD: break;
				case SDL_MULTIGESTURE: break;
				case SDL_CLIPBOARDUPDATE: break;
				case SDL_DROPFILE: break;
				case SDL_DROPTEXT: break;
				case SDL_DROPBEGIN: break;
				case SDL_DROPCOMPLETE: break;
				case SDL_AUDIODEVICEADDED: break;
				case SDL_AUDIODEVICEREMOVED: break;
				case SDL_SENSORUPDATE: break;
			}
		}
	}

}