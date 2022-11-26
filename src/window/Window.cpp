#include "window/Window.hpp"
#include "events/defaultEvents.hpp"

#include <SDL2/SDL.h>

namespace dfl = rnd::events::defaults;


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
			window = SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_VULKAN);
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

	void Window::init(Extent extent, const char* title, uint32_t flags){
		PROFILE_FUNCTION();
		handle = newWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, extent.x, extent.y, RndFlagsToSDLFlags(flags));
		SDL_SetWindowData((SDL_Window*)handle, "owner", this);

	}

	void Window::shutdown(){
		PROFILE_FUNCTION();
		destroyWindow((SDL_Window*)handle);
		handle = nullptr;
	}

	void Window::fullscreen(bool state){
		PROFILE_FUNCTION();
		SDL_SetWindowFullscreen((SDL_Window*)handle, state ? SDL_WINDOW_FULLSCREEN : 0);
	}

	void Window::resizable(bool state){
		PROFILE_FUNCTION();
		SDL_SetWindowResizable((SDL_Window*)handle, (SDL_bool)state);
	}

	void Window::minimize(){
		PROFILE_FUNCTION();
		SDL_MinimizeWindow((SDL_Window*)handle);
	}

	void Window::maximize(){
		PROFILE_FUNCTION();
		SDL_MaximizeWindow((SDL_Window*)handle);
	}

	void Window::focus(){
		PROFILE_FUNCTION();
		SDL_SetWindowInputFocus((SDL_Window*)handle);
	}

	void Window::show(){
		PROFILE_FUNCTION();
		SDL_ShowWindow((SDL_Window*)handle);
	}

	void Window::hide(){
		PROFILE_FUNCTION();
		SDL_HideWindow((SDL_Window*)handle);
	}

	glm::i32vec2 Window::getPosition() const{
		PROFILE_FUNCTION();
		int x, y;
		SDL_GetWindowPosition((SDL_Window*)handle, &x, &y);
		return glm::i32vec2(x, y);
	}

	const char* Window::getTitle() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowTitle((SDL_Window*)handle);
	}

	Extent Window::getExtent() const{
		PROFILE_FUNCTION();
		int w, h;
		SDL_GetWindowSize((SDL_Window*)handle, &w, &h);
		return Extent(w, h);
	}
	
	Extent Window::getMaxExtent() const{
		PROFILE_FUNCTION();
		int w, h;
		SDL_GetWindowMaximumSize((SDL_Window*)handle, &w, &h);
		return Extent(w, h);
	}

	Extent Window::getMinExtent() const{
		PROFILE_FUNCTION();
		int w, h;
		SDL_GetWindowMinimumSize((SDL_Window*)handle, &w, &h);
		return Extent(w, h);
	}

	float Window::getOpacity() const{
		PROFILE_FUNCTION();
		float opacity = 0.f;
		SDL_GetWindowOpacity((SDL_Window*)handle, &opacity);
		return opacity;
	}

	bool Window::isFullscreen() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_FULLSCREEN;
	}

	bool Window::isBorderless() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_BORDERLESS;
	}

	bool Window::isHidden() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_HIDDEN;
	}

	bool Window::isResizable() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_RESIZABLE;
	}

	bool Window::isHighDPIAllowed() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_ALLOW_HIGHDPI;
	}

	bool Window::isAlwaysOnTop() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_ALWAYS_ON_TOP;
	}

	bool Window::isTaskBarHidden() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_SKIP_TASKBAR;
	}

	bool Window::isTooltip() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_TOOLTIP;
	}

	bool Window::isPopup() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_POPUP_MENU;
	}

	bool Window::isUtility() const{
		PROFILE_FUNCTION();
		return SDL_GetWindowFlags((SDL_Window*)handle) & SDL_WINDOW_UTILITY;
	}
	
	const char* Window::getAPIName(){
		PROFILE_FUNCTION();
		return "Simple Direct Media Layer 2 (SDL2)";
	}

	Version Window::getAPIVersion(){
		PROFILE_FUNCTION();
		Version version;
		version.major = SDL_MAJOR_VERSION;
		version.minor = SDL_MINOR_VERSION;
		version.patch = SDL_PATCHLEVEL;
		return version;
	}

	void* Window::getWindowHandle() const{
		PROFILE_FUNCTION();
		return handle;
	}


	void onQuitEvent(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::QUIT));
	}

	void onAppTermining(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::APP_TERMINATING));
	}

	void onAppLowMemory(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::APP_LOW_MEMORY));
	}

	void onAppWillEnterBackground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::APP_WILL_ENTER_BACKGROUND));
	}

	void onAppDidEnterBackground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::APP_DID_ENTER_BACKGROUND));
	}

	void onAppWillEnterForground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::APP_WILL_ENTER_FORGROUND));	
	}

	void onAppDidEnterForground(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::APP_DID_ENTER_FORGROUND));
	}

	void onLocalChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::LOCAL_CHANGED));
	}

	void onDisplayOrientation(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DisplayOrentationEventData data;
		data.displayID = event.display.display;
		data.orientation = (events::DisplayOrentation)event.display.data1;

		mediator.trigger(dfl::getID(dfl::DISPLAY_ORIENTATION), data);
	}

	void onDisplayConnected(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DisplayConnectedEventData data;
		data.displayID = event.display.display;
		
		mediator.trigger(dfl::getID(dfl::DISPLAY_CONNECTED), data);
	}

	void onDisplayDisconnected(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DisplayDisconnectedEventData data;
		data.displayID = event.display.display;
		
		mediator.trigger(dfl::getID(dfl::DISPLAY_DISCONNECTED), data);
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
		dfl::WindowShownEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_SHOWN), data);
	}

	void onWindowHidden(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowHiddenEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_HIDDEN), data);
	}

	void onWindowExposed(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowExposedEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_EXPOSED), data);
	}

	void onWindowResized(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowResizedEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.extent = Extent(event.window.data1, event.window.data2);
		
		mediator.trigger(dfl::getID(dfl::WINDOW_RESIZED), data);
	}

	void onWindowSizeChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowSizeChangedEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.extent = Extent(event.window.data1, event.window.data2);
		
		mediator.trigger(dfl::getID(dfl::WINDOW_SIZE_CHANGED), data);
	}

	void onWindowMinimized(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowMinimizedEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_MINIMIZED), data);
	}

	void onWindowMaximized(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowMaximizedEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_MAXIMIZED), data);
	}

	void onWindowRestored(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowRestoredEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_RESTORED), data);
	}

	void onWindowEnter(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowEnterEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_ENTER), data);
	}

	void onWindowLeave(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowLeaveEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_LEAVE), data);
	}

	void onWindowFocusGained(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowFocusGainedEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_FOCUS_GAINED), data);
	}

	void onWindowFocusLost(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowFocusLostEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_FOCUS_LOST), data);
	}

	void onWindowClose(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowCloseEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_CLOSE), data);
	}

	void onWindowTakeFocus(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowTakeFocusEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_TAKE_FOCUS), data);
	}

	void onWindowHitTest(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowHitTestEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_HIT_TEST), data);
	}

	void onWindowDisplayChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::WindowDisplayChangedEventData data;
		SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::WINDOW_DISPLAY_CHANGED), data);
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
		dfl::KeyDownEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.key.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.repeate = (bool)event.key.repeat;
		data.virtualKey = (events::keys::vKey)event.key.keysym.sym;
		
		mediator.trigger(dfl::getID(dfl::KEY_DOWN), data);
	}

	void onKeyUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::KeyUpEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.key.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.virtualKey = (events::keys::vKey)event.key.keysym.sym;
		
		mediator.trigger(dfl::getID(dfl::KEY_UP), data);
	}

	void onTextEditing(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::TextEditingEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.edit.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.start = event.edit.start;
		data.lenght = event.edit.length;
		memory::memcpy(data.text, event.edit.text, event.edit.length);
		
		mediator.trigger(dfl::getID(dfl::WINDOW_HIT_TEST), data);
	}

	void onTextInput(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::TextInputEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.text.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		memory::memcpy(data.text, event.text.text, 32);
		
		mediator.trigger(dfl::getID(dfl::WINDOW_HIT_TEST), data);
	}

	void onKeyMapChanged(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		mediator.trigger(dfl::getID(dfl::KEY_MAP_CHANGED));
	}

	void onMouseMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::MouseMotionEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.motion.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.pos = glm::ivec2(event.motion.x, event.motion.y);
		data.rel = glm::vec2(event.motion.xrel, event.motion.yrel);
		
		mediator.trigger(dfl::getID(dfl::MOUSE_MOTION), data);
	}

	void onMouseButtonDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::MouseButtonDownEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.button.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.clicks = event.button.clicks;
		data.pos = glm::ivec2(event.button.x, event.button.y);
		data.button = (events::MouseButton)event.button.button;
		
		mediator.trigger(dfl::getID(dfl::MOUSE_BUTTON_DOWN), data);
	}

	void onMouseButtonUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::MouseButtonUpEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.button.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.pos = glm::ivec2(event.button.x, event.button.y);
		data.button = (events::MouseButton)event.button.button;
		
		mediator.trigger(dfl::getID(dfl::MOUSE_BUTTON_UP), data);
	}

	void onMouseWheel(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::MouseWheelEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.wheel.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.scrool = glm::vec2(event.wheel.preciseX, event.wheel.preciseY);
		
		mediator.trigger(dfl::getID(dfl::MOUSE_WHEEL), data);
	}

	void onJoystickAxisMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::JoystickAxisMotionEventData data;

		data.axis = event.jaxis.axis;
		data.range = (float)event.jaxis.value / (float)SDL_JOYSTICK_AXIS_MAX;
		
		mediator.trigger(dfl::getID(dfl::JOYSTICK_AXIS_MOTION), data);
	}

	void onJoystickBallMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::JoystickBallMotionEventData data;

		data.ball = event.jball.ball;
		data.motion = glm::vec2(event.jball.xrel, event.jball.yrel);
		
		mediator.trigger(dfl::getID(dfl::JOYSTICK_BALL_MOTION), data);
	}

	void onJoystickHatMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::JoystickHatMotionEventData data;

		data.hat = event.jhat.hat;
		data.position = (events::HatPosition)event.jhat.value;
		
		mediator.trigger(dfl::getID(dfl::JOYSTICK_HAT_MOTION), data);
	}

	void onJoystickButtonDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::JoystickButtonDownEventData data;

		data.button = event.jbutton.button;
		
		mediator.trigger(dfl::getID(dfl::JOYSTICK_BUTTON_DOWN), data);
	}

	void onJoystickButtonUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::JoystickButtonUpEventData data;

		data.button = event.jbutton.button;
		
		mediator.trigger(dfl::getID(dfl::JOYSTICK_BUTTON_UP), data);
	}

	void onJoystickDeviceAdded(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::JoystickDeviceAddedEventData data;
		
		mediator.trigger(dfl::getID(dfl::JOYSTICK_DEVICE_ADDED), data);
	}

	void onJoystickDeviceRemoved(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::JoystickDeviceRemovedEventData data;
		
		mediator.trigger(dfl::getID(dfl::JOYSTICK_DEVICE_REMOVED), data);
	}

	void onControllerAxisMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerAxisMotionEventData data;
		data.axis = (events::ControllerAxis)event.caxis.axis;
		data.range = (float)event.caxis.value / (float)SDL_JOYSTICK_AXIS_MAX;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_AXIS_MOTION), data);
	}

	void onControllerButtonDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerButtonDownEventData data;
		data.button = (events::ControllerButton)event.cbutton.button;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_BUTTON_DOWN), data);
	}

	void onControllerButtonUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerButtonUpEventData data;
		data.button = (events::ControllerButton)event.cbutton.button;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_BUTTON_UP), data);
	}

	void onControllerDeviceAdded(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerDeviceAddedEventData data;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_DEVICE_ADDED), data);
	}

	void onControllerDeviceRemoved(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerDeviceRemovedEventData data;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_DEVICE_REMOVED), data);
	}

	void onControllerDeviceRemapped(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerDeviceRemappedEventData data;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_DEVICE_REMAPPED), data);
	}

	void onControllerTouchpadDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerTouchpadDownEventData data;

		data.finger = event.ctouchpad.finger;
		data.index = event.ctouchpad.touchpad;
		data.pos = glm::vec2(event.ctouchpad.x, event.ctouchpad.y);
		data.pressure = event.ctouchpad.pressure;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_TOUCHPAD_DOWN), data);
	}

	void onControllerTouchpadUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerTouchpadUpEventData data;

		data.finger = event.ctouchpad.finger;
		data.index = event.ctouchpad.touchpad;
		data.pos = glm::vec2(event.ctouchpad.x, event.ctouchpad.y);
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_TOUCHPAD_UP), data);
	}

	void onControllerTouchpadMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ControllerTouchpadMotionEventData data;

		data.finger = event.ctouchpad.finger;
		data.index = event.ctouchpad.touchpad;
		data.pos = glm::vec2(event.ctouchpad.x, event.ctouchpad.y);
		data.pressure = event.ctouchpad.pressure;
		
		mediator.trigger(dfl::getID(dfl::CONTROLLER_TOUCHPAD_MOTION), data);
	}

	void onControllerSensorUpdate(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::SensorUpdateEventData data;

		memcpy(data.data, event.sensor.data, sizeof(float) * 6);
		
		mediator.trigger(dfl::getID(dfl::SENSOR_UPDATE), data);
	}

	void onFingerDown(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::FingerDownEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.tfinger.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.pressure = event.tfinger.pressure;
		data.pos = glm::vec2(event.tfinger.x, event.tfinger.y);
		
		mediator.trigger(dfl::getID(dfl::FINGER_DOWN), data);
	}

	void onFingerUp(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::FingerUpEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.tfinger.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.pos = glm::vec2(event.tfinger.x, event.tfinger.y);
		
		mediator.trigger(dfl::getID(dfl::FINGER_UP), data);
	}

	void onFingerMotion(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::FingerMotionEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.tfinger.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.pressure = event.tfinger.pressure;
		data.pos = glm::vec2(event.tfinger.x, event.tfinger.y);
		
		mediator.trigger(dfl::getID(dfl::FINGER_MOTION), data);
	}

	void onDollarGersture(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DollarGestureEventData data;
		mediator.trigger(dfl::getID(dfl::DOLLAR_GERSTURE), data);
	}

	void onDollarRecord(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DollarRecordEventData data;
		mediator.trigger(dfl::getID(dfl::DOLLAR_RECORD), data);
	}

	void onMultiGesture(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::MultiGestureEventData data;
		mediator.trigger(dfl::getID(dfl::MULTI_GESTURE), data);
	}

	void onClipboardUpdate(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::ClipboardEventData data;
		mediator.trigger(dfl::getID(dfl::CLIPBOARD_UPDATE), data);
	}

	void onDropFile(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DropFileEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.drop.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.file = event.drop.file;
		
		mediator.trigger(dfl::getID(dfl::DROP_FILE), data);
	}

	void onDropText(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DropTextEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.drop.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		data.text = event.drop.file;
		
		mediator.trigger(dfl::getID(dfl::DROP_TEXT), data);
	}

	void onDropBegin(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DropBeginEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.drop.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::DROP_BEGIN), data);
	}

	void onDropComplete(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::DropCompleteEventData data;

		SDL_Window* window = SDL_GetWindowFromID(event.drop.windowID);
		data.window = (Window*)SDL_GetWindowData(window, "owner");
		
		mediator.trigger(dfl::getID(dfl::DROP_COMPLETE), data);
	}

	void onAudioDeviceAdded(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::AudioDeviceAddedEventData data;

		mediator.trigger(dfl::getID(dfl::AUDIO_DEVICE_ADDED), data);
	}

	void onAudioDeviceRemoved(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::AudioDeviceRemovedEventData data;

		mediator.trigger(dfl::getID(dfl::AUDIO_DEVICE_REMOVED), data);
	}

	void onSensorUpdate(SDL_Event &event, events::Mediator& mediator){
		PROFILE_FUNCTION();
		dfl::SensorUpdateEventData data;

		memcpy(data.data, event.sensor.data, sizeof(float) * 6);

		mediator.trigger(dfl::getID(dfl::SENSOR_UPDATE), data);
	}

	void Window::event(events::Mediator& mediator){
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
				case SDL_CONTROLLERDEVICEREMAPPED: onControllerDeviceRemapped(event, mediator); break;
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