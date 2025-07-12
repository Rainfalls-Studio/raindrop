#include "Raindrop/Window/WindowSystem.hpp"
#include "Raindrop/Window/WindowEvents.hpp"
#include "Raindrop/Event/EventSystem.hpp"
#include "Raindrop/Engine.hpp"
#include <stdexcept>
#include <SDL3/SDL.h>

namespace Raindrop::Window{

    void WindowSystem::initialize(Engine& engine){
        _engine = &engine;

        if (SDL_Init(SDL_INIT_VIDEO) == false){
            throw std::runtime_error("Failed to initialize SDL");
        }

        _window = std::make_unique<Window>();
    }

    void WindowSystem::shutdown(){
        SDL_Quit();
    }

    void terminatingEvent(Event::Manager&, SDL_Event&){}
    void lowMemoryEvent(Event::Manager&, SDL_Event&){}
    void willEnterBackgroundEvent(Event::Manager&, SDL_Event&){}
    void didEnterBackgroundEvent(Event::Manager&, SDL_Event&){}
    void willEnterForgroundEvent(Event::Manager&, SDL_Event&){}
    void didEnterForgourndEvent(Event::Manager&, SDL_Event&){}
    void localeChangedEvent(Event::Manager&, SDL_Event&){}
    void systemThemChangedEvent(Event::Manager&, SDL_Event&){}
    void displayOrientationEvent(Event::Manager&, SDL_Event&){}
    void displayAddedEvent(Event::Manager&, SDL_Event&){}
    void displayRemovedEvent(Event::Manager&, SDL_Event&){}
    void displayMovedEvent(Event::Manager&, SDL_Event&){}
    void displayContentScaleChangedEvent(Event::Manager&, SDL_Event&){}
    void displayHDRStateChnagedEvent(Event::Manager&, SDL_Event&){}

    void windowShownEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowShown());
    }

    void windowHiddenEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowHidden());
    }

    void windowExposedEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowExposed());

    }

    void windowMovedEvent(Event::Manager& manager, SDL_Event& event){
        glm::ivec2 pos{
            static_cast<int>(event.window.data1),
            static_cast<int>(event.window.data2)
        };

        manager.trigger(WindowMoved(pos));
    }

    void windowResizedEvent(Event::Manager& manager, SDL_Event& event){
        glm::uvec2 size{
            static_cast<unsigned int>(event.window.data1),
            static_cast<unsigned int>(event.window.data2)
        };

        manager.trigger(WindowResized(size));
    }

    void windowPixelSizeChangedEvent(Event::Manager&, SDL_Event&){}

    void windowMinimizedEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowMinimized());
    }

    void windowMaximizedEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowMaximized());
    }

    void windowRestoredEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowRestored());
    }

    void windowMouseEnterEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowMouseEntered());
    }

    void windowMouseLeaveEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowMouseLeaved());
    }

    void windowFocusGainedEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowFocusGained());
    }

    void windowFocusLostEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowFocusLost());
    }

    void windowCloseRequestedEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowCloseRequest());
    }

    void windowTakeFocusEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowFocusTake());
    }

    void windowHitTestEvent(Event::Manager&, SDL_Event&){}
    void windowICCPROFChangedEvent(Event::Manager&, SDL_Event&){}
    void windowDisplayChangedEvent(Event::Manager&, SDL_Event&){}
    void windowDisplayScaleChangedEvent(Event::Manager&, SDL_Event&){}

    void windowOccludedEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowOccluded());
    }

    void windowEnterFullsreenEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowFullscreenEnter());
    }

    void windowLeaveFullscreenEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowFullscreenLeave());
    }

    void windowDestroyedEvent(Event::Manager& manager, SDL_Event&){
        manager.trigger(WindowDestroyed());
    }

    void windowPenEnterEvent(Event::Manager&, SDL_Event&){}
    void windowPenLeaveEvent(Event::Manager&, SDL_Event&){}
    void keyDownEvent(Event::Manager&, SDL_Event&){}
    void keyUpEvent(Event::Manager&, SDL_Event&){}
    void textEditingEvent(Event::Manager&, SDL_Event&){}
    void textInputEvent(Event::Manager&, SDL_Event&){}
    void keymapChangedEvent(Event::Manager&, SDL_Event&){}
    void mouseMotionEvent(Event::Manager&, SDL_Event&){}
    void mouseButtonDownEvent(Event::Manager&, SDL_Event&){}
    void mouseButtonUpEvent(Event::Manager&, SDL_Event&){}
    void mouseWheelEvent(Event::Manager&, SDL_Event&){}
    void quitEvent(Event::Manager&, SDL_Event&){}
    void joystickAxisMotionEvent(Event::Manager&, SDL_Event&){}
    void joystickHatMotionEvent(Event::Manager&, SDL_Event&){}
    void joystickButtonDownEvent(Event::Manager&, SDL_Event&){}
    void joystickButtonUpEvent(Event::Manager&, SDL_Event&){}
    void joystickAddedEvent(Event::Manager&, SDL_Event&){}
    void joystickRemovedEvent(Event::Manager&, SDL_Event&){}
    void joystickBatteryUpdatedEvent(Event::Manager&, SDL_Event&){}
    void joystickUpdateCompletedEvent(Event::Manager&, SDL_Event&){}
    void gamepadAxisMotionEvent(Event::Manager&, SDL_Event&){}
    void gamepadButtonDownEvent(Event::Manager&, SDL_Event&){}
    void gamepadButtonUpEvent(Event::Manager&, SDL_Event&){}
    void gamepadAddedEvent(Event::Manager&, SDL_Event&){}
    void gamepadRemovedEvent(Event::Manager&, SDL_Event&){}
    void gamepadRemappedEvent(Event::Manager&, SDL_Event&){}
    void gamepadTouchpadDownEvent(Event::Manager&, SDL_Event&){}
    void gamepadTouchpadMotionEvent(Event::Manager&, SDL_Event&){}
    void gamepadTouchpadUpEvent(Event::Manager&, SDL_Event&){}
    void gamepadSensorUpdateEvent(Event::Manager&, SDL_Event&){}
    void gamepadUpdateCompleteEvent(Event::Manager&, SDL_Event&){}
    void gamepadSteamHandleUpdatedEvent(Event::Manager&, SDL_Event&){}
    void fingerDownEvent(Event::Manager&, SDL_Event&){}
    void fingerUpEvent(Event::Manager&, SDL_Event&){}
    void fingerMotionEvent(Event::Manager&, SDL_Event&){}
    void clipboardUpdateEvent(Event::Manager&, SDL_Event&){}
    void dropFileEvent(Event::Manager&, SDL_Event&){}
    void dropTextEvent(Event::Manager&, SDL_Event&){}
    void dropBeginEvent(Event::Manager&, SDL_Event&){}
    void dropCompleteEvent(Event::Manager&, SDL_Event&){}
    void dropPositionEvent(Event::Manager&, SDL_Event&){}
    void audioDeviceAddedEvent(Event::Manager&, SDL_Event&){}
    void audioDeviceRemovedEvent(Event::Manager&, SDL_Event&){}
    void audioDeviceFormatChangedEvent(Event::Manager&, SDL_Event&){}
    void sensorUpdateEvent(Event::Manager&, SDL_Event&){}
    void penDownEvent(Event::Manager&, SDL_Event&){}
    void penUpEvent(Event::Manager&, SDL_Event&){}
    void penMotionEvent(Event::Manager&, SDL_Event&){}
    void penButtonDownEvent(Event::Manager&, SDL_Event&){}
    void penButtonUpEvent(Event::Manager&, SDL_Event&){}
    void cameraDeviceAddedEvent(Event::Manager&, SDL_Event&){}
    void cameraDeviceRemovedEvent(Event::Manager&, SDL_Event&){}
    void cameraDeviceApprovedEvent(Event::Manager&, SDL_Event&){}
    void cameraDeviceDeniedEvent(Event::Manager&, SDL_Event&){}
    void renderTargetsResetEvent(Event::Manager&, SDL_Event&){}
    void renderDeviceResetEvent(Event::Manager&, SDL_Event&){}

    void WindowSystem::event(){
        std::shared_ptr<Event::EventSystem> events;

        try{
            events = _engine->getSystemManager().getSystem<Event::EventSystem>();
        } catch (std::out_of_range&){ // If the system doesn't exists
            return;
        } catch (...){
            throw;
        }

        SDL_Event e;
        auto& manager = events->getManager();

        while (SDL_PollEvent(&e)){
			switch (e.type){

			case SDL_EVENT_TERMINATING: terminatingEvent(manager, e); break;
			case SDL_EVENT_LOW_MEMORY: lowMemoryEvent(manager, e); break;

			case SDL_EVENT_WILL_ENTER_BACKGROUND: willEnterBackgroundEvent(manager, e); break;
			case SDL_EVENT_DID_ENTER_BACKGROUND: didEnterBackgroundEvent(manager, e); break;
			case SDL_EVENT_WILL_ENTER_FOREGROUND: willEnterForgroundEvent(manager, e); break;
			case SDL_EVENT_DID_ENTER_FOREGROUND: didEnterForgourndEvent(manager, e); break;

			case SDL_EVENT_LOCALE_CHANGED: localeChangedEvent(manager, e); break;
			case SDL_EVENT_SYSTEM_THEME_CHANGED: systemThemChangedEvent(manager, e); break;
			case SDL_EVENT_DISPLAY_ORIENTATION: displayOrientationEvent(manager, e); break;
			case SDL_EVENT_DISPLAY_ADDED: displayAddedEvent(manager, e); break;
			case SDL_EVENT_DISPLAY_REMOVED: displayRemovedEvent(manager, e); break;
			case SDL_EVENT_DISPLAY_MOVED: displayMovedEvent(manager, e); break;
			case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: displayContentScaleChangedEvent(manager, e); break;

			case SDL_EVENT_WINDOW_SHOWN: windowShownEvent(manager, e); break;
			case SDL_EVENT_WINDOW_HIDDEN: windowHiddenEvent(manager, e); break;
			case SDL_EVENT_WINDOW_EXPOSED: windowExposedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_MOVED: windowMovedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_RESIZED: windowResizedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: windowPixelSizeChangedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_MINIMIZED: windowMinimizedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_MAXIMIZED: windowMaximizedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_RESTORED: windowRestoredEvent(manager, e); break;
			case SDL_EVENT_WINDOW_MOUSE_ENTER: windowMouseEnterEvent(manager, e); break;
			case SDL_EVENT_WINDOW_MOUSE_LEAVE: windowMouseLeaveEvent(manager, e); break;
			case SDL_EVENT_WINDOW_FOCUS_GAINED: windowFocusGainedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_FOCUS_LOST: windowFocusLostEvent(manager, e); break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED: windowCloseRequestedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_HIT_TEST: windowHitTestEvent(manager, e); break;
			case SDL_EVENT_WINDOW_ICCPROF_CHANGED: windowICCPROFChangedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_DISPLAY_CHANGED: windowDisplayChangedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: windowDisplayScaleChangedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_OCCLUDED: windowOccludedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: windowEnterFullsreenEvent(manager, e); break;
			case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: windowLeaveFullscreenEvent(manager, e); break;
			case SDL_EVENT_WINDOW_DESTROYED: windowDestroyedEvent(manager, e); break;
			case SDL_EVENT_WINDOW_HDR_STATE_CHANGED: displayHDRStateChnagedEvent(manager, e); break;

			case SDL_EVENT_KEY_DOWN: keyDownEvent(manager, e); break;
			case SDL_EVENT_KEY_UP: keyUpEvent(manager, e); break;
			case SDL_EVENT_TEXT_EDITING: textEditingEvent(manager, e); break;
			case SDL_EVENT_TEXT_INPUT: textInputEvent(manager, e); break;
			case SDL_EVENT_KEYMAP_CHANGED: keymapChangedEvent(manager, e); break;
		
			case SDL_EVENT_MOUSE_MOTION: mouseMotionEvent(manager, e); break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN: mouseButtonDownEvent(manager, e); break;
			case SDL_EVENT_MOUSE_BUTTON_UP: mouseButtonUpEvent(manager, e); break;
			case SDL_EVENT_MOUSE_WHEEL: mouseWheelEvent(manager, e); break;

			#ifdef EVENT_JOYSTICK
				case SDL_EVENT_JOYSTICK_AXIS_MOTION: joystickAxisMotionEvent(manager, e); break;
				case SDL_EVENT_JOYSTICK_HAT_MOTION: joystickHatMotionEvent(manager, e); break;
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN: joystickButtonDownEvent(manager, e); break;
				case SDL_EVENT_JOYSTICK_BUTTON_UP: joystickButtonUpEvent(manager, e); break;
				case SDL_EVENT_JOYSTICK_ADDED: joystickAddedEvent(manager, e); break;
				case SDL_EVENT_JOYSTICK_REMOVED: joystickRemovedEvent(manager, e); break;
				case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: joystickBatteryUpdatedEvent(manager, e); break;
				case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: joystickUpdateCompletedEvent(manager, e); break;
			#endif

			#ifdef EVENT_GAMEPAD
				case SDL_EVENT_GAMEPAD_AXIS_MOTION: gamepadAxisMotionEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_BUTTON_DOWN: gamepadButtonDownEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_BUTTON_UP: gamepadButtonUpEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_ADDED: gamepadAddedEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_REMOVED: gamepadRemovedEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_REMAPPED: gamepadRemappedEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: gamepadTouchpadDownEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: gamepadTouchpadMotionEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: gamepadTouchpadUpEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: gamepadSensorUpdateEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: gamepadUpdateCompleteEvent(manager, e); break;
				case SDL_EVENT_GAMEPAD_STEAM_WINDOW_UPDATED: gamepadSteamHandleUpdatedEvent(manager, e); break;
			#endif
			
			#ifdef EVENT_FINGER
				case SDL_EVENT_FINGER_DOWN: fingerDownEvent(manager, e); break;
				case SDL_EVENT_FINGER_UP: fingerUpEvent(manager, e); break;
				case SDL_EVENT_FINGER_MOTION: fingerMotionEvent(manager, e); break;
			#endif
		
			#ifdef EVENT_CLIPBOARD
				case SDL_EVENT_CLIPBOARD_UPDATE: clipboardUpdateEvent(manager, e); break;
			#endif

			#ifdef EVENT_TEXT
				case SDL_EVENT_DROP_FILE: dropFileEvent(manager, e); break;
				case SDL_EVENT_DROP_TEXT: dropTextEvent(manager, e); break;
				case SDL_EVENT_DROP_BEGIN: dropBeginEvent(manager, e); break;
				case SDL_EVENT_DROP_COMPLETE: dropCompleteEvent(manager, e); break;
				case SDL_EVENT_DROP_POSITION: dropPositionEvent(manager, e); break;
			#endif

			#ifdef EVENT_AUDIO
				case SDL_EVENT_AUDIO_DEVICE_ADDED: audioDeviceAddedEvent(manager, e); break;
				case SDL_EVENT_AUDIO_DEVICE_REMOVED: audioDeviceRemovedEvent(manager, e); break;
				case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: audioDeviceFormatChangedEvent(manager, e); break;
			#endif

			#ifdef EVENR_SENSOR
				case SDL_EVENT_SENSOR_UPDATE: sensorUpdateEvent(manager, e); break;
			#endif

			#ifdef EVENT_PEN
				case SDL_EVENT_PEN_DOWN: penDownEvent(manager, e); break;
				case SDL_EVENT_PEN_UP: penUpEvent(manager, e); break;
				case SDL_EVENT_PEN_MOTION: penMotionEvent(manager, e); break;
				case SDL_EVENT_PEN_BUTTON_DOWN: penButtonDownEvent(manager, e); break;
				case SDL_EVENT_PEN_BUTTON_UP: penButtonUpEvent(manager, e); break;
			#endif

			#ifdef EVENT_CAMERA
				case SDL_EVENT_CAMERA_DEVICE_ADDED: cameraDeviceAddedEvent(manager, e); break;
				case SDL_EVENT_CAMERA_DEVICE_REMOVED: cameraDeviceRemovedEvent(manager, e); break;
				case SDL_EVENT_CAMERA_DEVICE_APPROVED: cameraDeviceApprovedEvent(manager, e); break;
				case SDL_EVENT_CAMERA_DEVICE_DENIED: cameraDeviceDeniedEvent(manager, e); break;
			#endif
			
			#ifdef EVENT_RENDER
				case SDL_EVENT_RENDER_TARGETS_RESET: renderTargetsResetEvent(manager, e); break;
				case SDL_EVENT_RENDER_DEVICE_RESET: renderDeviceResetEvent(manager, e); break;
			#endif
			}
		}
    }

    const char* WindowSystem::name() const{
        return "SDL3 windowing system";
    }

    Window& WindowSystem::getWindow(){
        assert(_window && "The system must be initialized");
        return *_window;
    }

}