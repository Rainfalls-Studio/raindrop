#include "Raindrop/Systems/Window/WindowSystem.hpp"
#include "Raindrop/Systems/Window/WindowEvents.hpp"
#include "Raindrop/Systems/Event/EventSystem.hpp"
#include "Raindrop/Engine.hpp"
#include <stdexcept>
#include <SDL3/SDL.h>

namespace Raindrop::Window{
    std::expected<void, Error> WindowSystem::initialize(Engine& engine){
        _engine = &engine;

        if (SDL_Init(SDL_INIT_VIDEO) == false){
            throw std::runtime_error("Failed to initialize SDL");
        }

        eventSubscription = engine.getScheduler().subscribe([this]{event();}, Scheduler::Priority::EVENT);
        
        return {};
    }

    std::vector<Systems::Dependency> WindowSystem::dependencies() const noexcept{
        return {
            Systems::Dependency("Event")
        };
    }


    std::shared_ptr<Window> WindowSystem::createWindow(const WindowConfig& config){
        std::shared_ptr<Window> win = std::make_shared<Window>(config);
        _windows[SDL_GetWindowID(win->getWindow())] = win;
        return win;
    }

    void WindowSystem::shutdown(){
        SDL_Quit();
    }

    struct EventInfo{
        Event::Manager& manager;
        SDL_Event& e;
        std::unordered_map<SDL_WindowID, std::weak_ptr<Window>>& windows;
    };

    void terminatingEvent(EventInfo&){}
    void lowMemoryEvent(EventInfo&){}
    void willEnterBackgroundEvent(EventInfo&){}
    void didEnterBackgroundEvent(EventInfo&){}
    void willEnterForgroundEvent(EventInfo&){}
    void didEnterForgourndEvent(EventInfo&){}
    void localeChangedEvent(EventInfo&){}
    void systemThemChangedEvent(EventInfo&){}
    void displayOrientationEvent(EventInfo&){}
    void displayAddedEvent(EventInfo&){}
    void displayRemovedEvent(EventInfo&){}
    void displayMovedEvent(EventInfo&){}
    void displayContentScaleChangedEvent(EventInfo&){}
    void displayHDRStateChnagedEvent(EventInfo&){}

    void windowShownEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowShown(window));
    }

    void windowHiddenEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowHidden(window));
    }

    void windowExposedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowExposed(window));

    }

    void windowMovedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);

        glm::ivec2 pos{
            static_cast<int>(info.e.window.data1),
            static_cast<int>(info.e.window.data2)
        };

        info.manager.trigger(Events::WindowMoved(window, pos));
    }

    void windowResizedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);

        glm::uvec2 size{
            static_cast<unsigned int>(info.e.window.data1),
            static_cast<unsigned int>(info.e.window.data2)
        };

        info.manager.trigger(Events::WindowResized(window, size));
    }

    void windowPixelSizeChangedEvent(EventInfo&){}

    void windowMinimizedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowMinimized(window));
    }

    void windowMaximizedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowMaximized(window));
    }

    void windowRestoredEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowRestored(window));
    }

    void windowMouseEnterEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowMouseEntered(window));
    }

    void windowMouseLeaveEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowMouseLeaved(window));
    }

    void windowFocusGainedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowFocusGained(window));
    }

    void windowFocusLostEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowFocusLost(window));
    }

    void windowCloseRequestedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowCloseRequest(window));
    }

    void windowTakeFocusEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowFocusTake(window));
    }

    void windowHitTestEvent(EventInfo&){}
    void windowICCPROFChangedEvent(EventInfo&){}
    void windowDisplayChangedEvent(EventInfo&){}
    void windowDisplayScaleChangedEvent(EventInfo&){}

    void windowOccludedEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowOccluded(window));
    }

    void windowEnterFullsreenEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowFullscreenEnter(window));
    }

    void windowLeaveFullscreenEvent(EventInfo& info){
        std::shared_ptr<Window> window = info.windows[info.e.window.windowID].lock();
        assert(window);
        info.manager.trigger(Events::WindowFullscreenLeave(window));
    }

    void windowDestroyedEvent(EventInfo& info){
        info.manager.trigger(Events::WindowDestroyed());
    }

    void windowPenEnterEvent(EventInfo&){}
    void windowPenLeaveEvent(EventInfo&){}
    void keyDownEvent(EventInfo&){}
    void keyUpEvent(EventInfo&){}
    void textEditingEvent(EventInfo&){}
    void textInputEvent(EventInfo&){}
    void keymapChangedEvent(EventInfo&){}
    void mouseMotionEvent(EventInfo&){}
    void mouseButtonDownEvent(EventInfo&){}
    void mouseButtonUpEvent(EventInfo&){}
    void mouseWheelEvent(EventInfo&){}
    void quitEvent(EventInfo&){}
    void joystickAxisMotionEvent(EventInfo&){}
    void joystickHatMotionEvent(EventInfo&){}
    void joystickButtonDownEvent(EventInfo&){}
    void joystickButtonUpEvent(EventInfo&){}
    void joystickAddedEvent(EventInfo&){}
    void joystickRemovedEvent(EventInfo&){}
    void joystickBatteryUpdatedEvent(EventInfo&){}
    void joystickUpdateCompletedEvent(EventInfo&){}
    void gamepadAxisMotionEvent(EventInfo&){}
    void gamepadButtonDownEvent(EventInfo&){}
    void gamepadButtonUpEvent(EventInfo&){}
    void gamepadAddedEvent(EventInfo&){}
    void gamepadRemovedEvent(EventInfo&){}
    void gamepadRemappedEvent(EventInfo&){}
    void gamepadTouchpadDownEvent(EventInfo&){}
    void gamepadTouchpadMotionEvent(EventInfo&){}
    void gamepadTouchpadUpEvent(EventInfo&){}
    void gamepadSensorUpdateEvent(EventInfo&){}
    void gamepadUpdateCompleteEvent(EventInfo&){}
    void gamepadSteamHandleUpdatedEvent(EventInfo&){}
    void fingerDownEvent(EventInfo&){}
    void fingerUpEvent(EventInfo&){}
    void fingerMotionEvent(EventInfo&){}
    void clipboardUpdateEvent(EventInfo&){}
    void dropFileEvent(EventInfo&){}
    void dropTextEvent(EventInfo&){}
    void dropBeginEvent(EventInfo&){}
    void dropCompleteEvent(EventInfo&){}
    void dropPositionEvent(EventInfo&){}
    void audioDeviceAddedEvent(EventInfo&){}
    void audioDeviceRemovedEvent(EventInfo&){}
    void audioDeviceFormatChangedEvent(EventInfo&){}
    void sensorUpdateEvent(EventInfo&){}
    void penDownEvent(EventInfo&){}
    void penUpEvent(EventInfo&){}
    void penMotionEvent(EventInfo&){}
    void penButtonDownEvent(EventInfo&){}
    void penButtonUpEvent(EventInfo&){}
    void cameraDeviceAddedEvent(EventInfo&){}
    void cameraDeviceRemovedEvent(EventInfo&){}
    void cameraDeviceApprovedEvent(EventInfo&){}
    void cameraDeviceDeniedEvent(EventInfo&){}
    void renderTargetsResetEvent(EventInfo&){}
    void renderDeviceResetEvent(EventInfo&){}

    void WindowSystem::event(){
        std::shared_ptr<Event::EventSystem> events;
        events = _engine->getSystemManager().getSystemAs<Event::EventSystem>("Event");

        SDL_Event e;
        auto& manager = events->getManager();

        EventInfo info{
            .manager = manager,
            .e = e,
            .windows = _windows
        };

        while (SDL_PollEvent(&e)){
			switch (e.type){

			case SDL_EVENT_TERMINATING: terminatingEvent(info); break;
			case SDL_EVENT_LOW_MEMORY: lowMemoryEvent(info); break;

			case SDL_EVENT_WILL_ENTER_BACKGROUND: willEnterBackgroundEvent(info); break;
			case SDL_EVENT_DID_ENTER_BACKGROUND: didEnterBackgroundEvent(info); break;
			case SDL_EVENT_WILL_ENTER_FOREGROUND: willEnterForgroundEvent(info); break;
			case SDL_EVENT_DID_ENTER_FOREGROUND: didEnterForgourndEvent(info); break;

			case SDL_EVENT_LOCALE_CHANGED: localeChangedEvent(info); break;
			case SDL_EVENT_SYSTEM_THEME_CHANGED: systemThemChangedEvent(info); break;
			case SDL_EVENT_DISPLAY_ORIENTATION: displayOrientationEvent(info); break;
			case SDL_EVENT_DISPLAY_ADDED: displayAddedEvent(info); break;
			case SDL_EVENT_DISPLAY_REMOVED: displayRemovedEvent(info); break;
			case SDL_EVENT_DISPLAY_MOVED: displayMovedEvent(info); break;
			case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: displayContentScaleChangedEvent(info); break;

			case SDL_EVENT_WINDOW_SHOWN: windowShownEvent(info); break;
			case SDL_EVENT_WINDOW_HIDDEN: windowHiddenEvent(info); break;
			case SDL_EVENT_WINDOW_EXPOSED: windowExposedEvent(info); break;
			case SDL_EVENT_WINDOW_MOVED: windowMovedEvent(info); break;
			case SDL_EVENT_WINDOW_RESIZED: windowResizedEvent(info); break;
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: windowPixelSizeChangedEvent(info); break;
			case SDL_EVENT_WINDOW_MINIMIZED: windowMinimizedEvent(info); break;
			case SDL_EVENT_WINDOW_MAXIMIZED: windowMaximizedEvent(info); break;
			case SDL_EVENT_WINDOW_RESTORED: windowRestoredEvent(info); break;
			case SDL_EVENT_WINDOW_MOUSE_ENTER: windowMouseEnterEvent(info); break;
			case SDL_EVENT_WINDOW_MOUSE_LEAVE: windowMouseLeaveEvent(info); break;
			case SDL_EVENT_WINDOW_FOCUS_GAINED: windowFocusGainedEvent(info); break;
			case SDL_EVENT_WINDOW_FOCUS_LOST: windowFocusLostEvent(info); break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED: windowCloseRequestedEvent(info); break;
			case SDL_EVENT_WINDOW_HIT_TEST: windowHitTestEvent(info); break;
			case SDL_EVENT_WINDOW_ICCPROF_CHANGED: windowICCPROFChangedEvent(info); break;
			case SDL_EVENT_WINDOW_DISPLAY_CHANGED: windowDisplayChangedEvent(info); break;
			case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: windowDisplayScaleChangedEvent(info); break;
			case SDL_EVENT_WINDOW_OCCLUDED: windowOccludedEvent(info); break;
			case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: windowEnterFullsreenEvent(info); break;
			case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: windowLeaveFullscreenEvent(info); break;
			case SDL_EVENT_WINDOW_DESTROYED: windowDestroyedEvent(info); break;
			case SDL_EVENT_WINDOW_HDR_STATE_CHANGED: displayHDRStateChnagedEvent(info); break;

			case SDL_EVENT_KEY_DOWN: keyDownEvent(info); break;
			case SDL_EVENT_KEY_UP: keyUpEvent(info); break;
			case SDL_EVENT_TEXT_EDITING: textEditingEvent(info); break;
			case SDL_EVENT_TEXT_INPUT: textInputEvent(info); break;
			case SDL_EVENT_KEYMAP_CHANGED: keymapChangedEvent(info); break;
		
			case SDL_EVENT_MOUSE_MOTION: mouseMotionEvent(info); break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN: mouseButtonDownEvent(info); break;
			case SDL_EVENT_MOUSE_BUTTON_UP: mouseButtonUpEvent(info); break;
			case SDL_EVENT_MOUSE_WHEEL: mouseWheelEvent(info); break;

			#ifdef EVENT_JOYSTICK
				case SDL_EVENT_JOYSTICK_AXIS_MOTION: joystickAxisMotionEvent(info); break;
				case SDL_EVENT_JOYSTICK_HAT_MOTION: joystickHatMotionEvent(info); break;
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN: joystickButtonDownEvent(info); break;
				case SDL_EVENT_JOYSTICK_BUTTON_UP: joystickButtonUpEvent(info); break;
				case SDL_EVENT_JOYSTICK_ADDED: joystickAddedEvent(info); break;
				case SDL_EVENT_JOYSTICK_REMOVED: joystickRemovedEvent(info); break;
				case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: joystickBatteryUpdatedEvent(info); break;
				case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: joystickUpdateCompletedEvent(info); break;
			#endif

			#ifdef EVENT_GAMEPAD
				case SDL_EVENT_GAMEPAD_AXIS_MOTION: gamepadAxisMotionEvent(info); break;
				case SDL_EVENT_GAMEPAD_BUTTON_DOWN: gamepadButtonDownEvent(info); break;
				case SDL_EVENT_GAMEPAD_BUTTON_UP: gamepadButtonUpEvent(info); break;
				case SDL_EVENT_GAMEPAD_ADDED: gamepadAddedEvent(info); break;
				case SDL_EVENT_GAMEPAD_REMOVED: gamepadRemovedEvent(info); break;
				case SDL_EVENT_GAMEPAD_REMAPPED: gamepadRemappedEvent(info); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: gamepadTouchpadDownEvent(info); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: gamepadTouchpadMotionEvent(info); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: gamepadTouchpadUpEvent(info); break;
				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: gamepadSensorUpdateEvent(info); break;
				case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: gamepadUpdateCompleteEvent(info); break;
				case SDL_EVENT_GAMEPAD_STEAM_WINDOW_UPDATED: gamepadSteamHandleUpdatedEvent(info); break;
			#endif
			
			#ifdef EVENT_FINGER
				case SDL_EVENT_FINGER_DOWN: fingerDownEvent(info); break;
				case SDL_EVENT_FINGER_UP: fingerUpEvent(info); break;
				case SDL_EVENT_FINGER_MOTION: fingerMotionEvent(info); break;
			#endif
		
			#ifdef EVENT_CLIPBOARD
				case SDL_EVENT_CLIPBOARD_UPDATE: clipboardUpdateEvent(info); break;
			#endif

			#ifdef EVENT_TEXT
				case SDL_EVENT_DROP_FILE: dropFileEvent(info); break;
				case SDL_EVENT_DROP_TEXT: dropTextEvent(info); break;
				case SDL_EVENT_DROP_BEGIN: dropBeginEvent(info); break;
				case SDL_EVENT_DROP_COMPLETE: dropCompleteEvent(info); break;
				case SDL_EVENT_DROP_POSITION: dropPositionEvent(info); break;
			#endif

			#ifdef EVENT_AUDIO
				case SDL_EVENT_AUDIO_DEVICE_ADDED: audioDeviceAddedEvent(info); break;
				case SDL_EVENT_AUDIO_DEVICE_REMOVED: audioDeviceRemovedEvent(info); break;
				case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: audioDeviceFormatChangedEvent(info); break;
			#endif

			#ifdef EVENR_SENSOR
				case SDL_EVENT_SENSOR_UPDATE: sensorUpdateEvent(info); break;
			#endif

			#ifdef EVENT_PEN
				case SDL_EVENT_PEN_DOWN: penDownEvent(info); break;
				case SDL_EVENT_PEN_UP: penUpEvent(info); break;
				case SDL_EVENT_PEN_MOTION: penMotionEvent(info); break;
				case SDL_EVENT_PEN_BUTTON_DOWN: penButtonDownEvent(info); break;
				case SDL_EVENT_PEN_BUTTON_UP: penButtonUpEvent(info); break;
			#endif

			#ifdef EVENT_CAMERA
				case SDL_EVENT_CAMERA_DEVICE_ADDED: cameraDeviceAddedEvent(info); break;
				case SDL_EVENT_CAMERA_DEVICE_REMOVED: cameraDeviceRemovedEvent(info); break;
				case SDL_EVENT_CAMERA_DEVICE_APPROVED: cameraDeviceApprovedEvent(info); break;
				case SDL_EVENT_CAMERA_DEVICE_DENIED: cameraDeviceDeniedEvent(info); break;
			#endif
			
			#ifdef EVENT_RENDER
				case SDL_EVENT_RENDER_TARGETS_RESET: renderTargetsResetEvent(info); break;
				case SDL_EVENT_RENDER_DEVICE_RESET: renderDeviceResetEvent(info); break;
			#endif
			}
		}
    }

    std::string WindowSystem::name() const noexcept{
        return "Window";
    }
}