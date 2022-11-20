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


	void onQuitEvent(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("Quit");
	}

	void onAppTermining(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("App Termining");
	}

	void onAppLowMemory(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("App Low Memory");
	}

	void onAppWillEnterBackground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("App Will Enter Background");
	}

	void onAppDidEnterBackground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("App Did Enter Background");
	}

	void onAppWillEnterForground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("App Will Enter Forground");	
	}

	void onAppDidEnterForground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("App Did Enter Forground");
	}

	void onLocalChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger("Local Changed");
	}

	void onDisplayOrientation(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onDisplayConnected(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();

	}

	void onDisplayDisconnected(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();

	}

	void onDisplayEvent(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		switch (event.display.event){
			case SDL_DISPLAYEVENT_ORIENTATION: onDisplayOrientation(event, mediator); break;
			case SDL_DISPLAYEVENT_CONNECTED: onDisplayConnected(event, mediator); break;
			case SDL_DISPLAYEVENT_DISCONNECTED: onDisplayDisconnected(event, mediator); break;
		}
	}

	void onWindowShown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowHidden(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowExposed(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowResized(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowSizeChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowMinimized(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowMaximized(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowRestored(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowEnter(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowLeave(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowFocusGained(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowFocusLost(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowClose(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowTakeFocus(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowHitTest(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowDisplayChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
	}

	void onWindowEvent(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();

		switch (event.window.event){
			case SDL_WINDOWEVENT_SHOWN: onWindowShown(event, mediator); break;
			case SDL_WINDOWEVENT_HIDDEN: onWindowHidden(event, mediator); break;
			case SDL_WINDOWEVENT_EXPOSED: onWindowExposed(event, mediator); break;
			case SDL_WINDOWEVENT_RESIZED: onWindowResized(event, mediator); break;
			case SDL_WINDOWEVENT_SIZE_CHANGED: onWindowSizeChanged(event, mediator); break;
			case SDL_WINDOWEVENT_MINIMIZED: onWindowMinimized(event, mediator); break;
			case SDL_WINDOWEVENT_MAXIMIZED: onWindowMaximized(event, mediator); break;
			case SDL_WINDOWEVENT_RESTORED: onWindowRestored(event, mediator); break;
			case SDL_WINDOWEVENT_ENTER: onWindowEnter(event, mediator); break;
			case SDL_WINDOWEVENT_LEAVE: onWindowLeave(event, mediator); break;
			case SDL_WINDOWEVENT_FOCUS_GAINED: onWindowFocusGained(event, mediator); break;
			case SDL_WINDOWEVENT_FOCUS_LOST: onWindowFocusLost(event, mediator); break;
			case SDL_WINDOWEVENT_CLOSE: onWindowClose(event, mediator); break;
			case SDL_WINDOWEVENT_TAKE_FOCUS: onWindowTakeFocus(event, mediator); break;
			case SDL_WINDOWEVENT_HIT_TEST: onWindowHitTest(event, mediator); break;
			case SDL_WINDOWEVENT_DISPLAY_CHANGED: onWindowDisplayChanged(event, mediator); break;
		}
	}

	void onKeyDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onKeyUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onTextEditing(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onTextInput(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onKeyMapChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onMouseMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onMouseButtonDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onMouseButtonUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onMouseWheel(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onJoystickAxisMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onJoystickBallMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onJoystickHatMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onJoystickButtonDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onJoystickButtonUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onJoystickDeviceAdded(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onJoystickDeviceRemoved(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerAxisMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerButtonDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerButtonUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerDeviceAdded(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerDeviceRemoved(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerDeviceRemaped(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerTouchpadDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerTouchpadUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerTouchpadMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onControllerSensorUpdate(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onFingerDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onFingerUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onFingerMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onDollarGersture(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onDollarRecord(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onMultiGesture(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onClipboardUpdate(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onDropFile(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onDropText(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onDropBegin(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onDropComplete(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onAudioDeviceAdded(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onAudioDeviceRemoved(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void onSensorUpdate(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		
	}

	void SDL2_Window::event(events::Mediator& mediator){
		PROFILE_FUNCTION();

		SDL_Event event;
		while (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT: onQuitEvent(event, mediator); break;
				case SDL_APP_TERMINATING: onAppTermining(event, mediator); break;
				case SDL_APP_LOWMEMORY: onAppLowMemory(event, mediator); break;
				case SDL_APP_WILLENTERBACKGROUND: onAppWillEnterBackground(event, mediator); break;
				case SDL_APP_DIDENTERBACKGROUND: onAppDidEnterBackground(event, mediator); break;
				case SDL_APP_WILLENTERFOREGROUND: onAppWillEnterForground(event, mediator); break;
				case SDL_APP_DIDENTERFOREGROUND: onAppDidEnterForground(event, mediator); break;
				case SDL_LOCALECHANGED: onLocalChanged(event, mediator); break;
				case SDL_DISPLAYEVENT: onDisplayEvent(event, mediator); break;
				case SDL_WINDOWEVENT: onWindowEvent(event, mediator); break;
				case SDL_KEYDOWN: onKeyDown(event, mediator); break;
				case SDL_KEYUP: onKeyUp(event, mediator); break;
				case SDL_TEXTEDITING: onTextEditing(event, mediator); break;
				case SDL_TEXTINPUT: onTextInput(event, mediator); break;
				case SDL_KEYMAPCHANGED: onKeyMapChanged(event, mediator); break;
				case SDL_MOUSEMOTION: onMouseMotion(event, mediator); break;
				case SDL_MOUSEBUTTONDOWN: onMouseButtonDown(event, mediator); break;
				case SDL_MOUSEBUTTONUP: onMouseButtonUp(event, mediator); break;
				case SDL_MOUSEWHEEL: onMouseWheel(event, mediator); break;
				case SDL_JOYAXISMOTION: onJoystickAxisMotion(event, mediator); break;
				case SDL_JOYBALLMOTION: onJoystickBallMotion(event, mediator); break;
				case SDL_JOYHATMOTION: onJoystickHatMotion(event, mediator); break;
				case SDL_JOYBUTTONDOWN: onJoystickButtonDown(event, mediator); break;
				case SDL_JOYBUTTONUP: onJoystickButtonUp(event, mediator); break;
				case SDL_JOYDEVICEADDED: onJoystickDeviceAdded(event, mediator); break;
				case SDL_JOYDEVICEREMOVED: onJoystickDeviceRemoved(event, mediator); break;
				case SDL_CONTROLLERAXISMOTION: onControllerAxisMotion(event, mediator); break;
				case SDL_CONTROLLERBUTTONDOWN: onControllerButtonDown(event, mediator); break;
				case SDL_CONTROLLERBUTTONUP: onControllerButtonUp(event, mediator); break;
				case SDL_CONTROLLERDEVICEADDED: onControllerDeviceAdded(event, mediator); break;
				case SDL_CONTROLLERDEVICEREMOVED: onControllerDeviceRemoved(event, mediator); break;
				case SDL_CONTROLLERDEVICEREMAPPED: onControllerDeviceRemaped(event, mediator); break;
				case SDL_CONTROLLERTOUCHPADDOWN: onControllerTouchpadDown(event, mediator); break;
				case SDL_CONTROLLERTOUCHPADMOTION: onControllerTouchpadMotion(event, mediator); break;
				case SDL_CONTROLLERTOUCHPADUP: onControllerTouchpadUp(event, mediator); break;
				case SDL_CONTROLLERSENSORUPDATE: onControllerSensorUpdate(event, mediator); break;
				case SDL_FINGERDOWN: onFingerDown(event, mediator); break;
				case SDL_FINGERUP: onFingerUp(event, mediator); break;
				case SDL_FINGERMOTION: onFingerMotion(event, mediator); break;
				case SDL_DOLLARGESTURE: onDollarGersture(event, mediator); break;
				case SDL_DOLLARRECORD: onDollarRecord(event, mediator); break;
				case SDL_MULTIGESTURE: onMultiGesture(event, mediator); break;
				case SDL_CLIPBOARDUPDATE: onClipboardUpdate(event, mediator); break;
				case SDL_DROPFILE: onDropFile(event, mediator); break;
				case SDL_DROPTEXT: onDropText(event, mediator); break;
				case SDL_DROPBEGIN: onDropBegin(event, mediator); break;
				case SDL_DROPCOMPLETE: onDropComplete(event, mediator); break;
				case SDL_AUDIODEVICEADDED: onAudioDeviceAdded(event, mediator); break;
				case SDL_AUDIODEVICEREMOVED: onAudioDeviceRemoved(event, mediator); break;
				case SDL_SENSORUPDATE: onSensorUpdate(event, mediator); break;
			}
		}
	}

}