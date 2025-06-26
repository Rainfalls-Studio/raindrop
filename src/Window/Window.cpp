#include "Raindrop/Window/Window.hpp"
#include "Raindrop/Event/Manager.hpp"
#include "Raindrop/Input/Key.hpp"
#include "Raindrop/Input/MouseEvents.hpp"
#include "Raindrop/Input/KeyEvents.hpp"
#include "Raindrop/Window/Config.hpp"
#include "Raindrop/Window/Flags.hpp"
#include "Raindrop/Window/SurfaceProviders/Vulkan.hpp"
#include "Raindrop/Window/SurfaceProviders/Metal.hpp"
#include "Raindrop/Window/WindowEvents.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <SDL3/SDL_metal.h>
#include <SDL3/SDL_vulkan.h>
#include <spdlog/spdlog.h>
#include <SDL3/SDL.h>
#include <stdexcept>
#include <string_view>

namespace Raindrop::Window{

	class SDL2VulkanSurfaceProvider : public SurfaceProviders::Vulkan{
		public:
            SDL2VulkanSurfaceProvider(Window& window) : SurfaceProviders::Vulkan(window){}

            virtual char const* const* getInstanceExtensions(uint32_t* count) override{
				return SDL_Vulkan_GetInstanceExtensions(count);
			}

            virtual bool createSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) override {
				return SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(_window.getNativeHandle()), instance, allocator, surface);
			}

            virtual void destroySurface(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* allocator) override{
				SDL_Vulkan_DestroySurface(instance, surface, allocator);
			}
	};

	class SDL2MetalSurfaceProvider : public SurfaceProviders::Metal{
		public:
            SDL2MetalSurfaceProvider(Window& window) : SurfaceProviders::Metal(window){}

            virtual SDL_MetalView createView() override{
				return SDL_Metal_CreateView(static_cast<SDL_Window*>(_window.getNativeHandle()));
			}

            virtual void destroyView(SDL_MetalView view) override{
				return SDL_Metal_DestroyView(view);
			}

            virtual void* getLayer(SDL_MetalView view) override{
				return SDL_Metal_GetLayer(view);
			}
	};

	SDL_WindowFlags raindropToSDLWindowFlags(Flags flags){
		SDL_WindowFlags out = 0;

		if (flags & Flags::FULLSCREEN) out |= SDL_WINDOW_FULLSCREEN;
		if (flags & Flags::BORDERLESS) out |= SDL_WINDOW_BORDERLESS;
		if (flags & Flags::RESIZABLE) out |= SDL_WINDOW_RESIZABLE;
		if (flags & Flags::MINIMIZED) out |= SDL_WINDOW_MINIMIZED;
		if (flags & Flags::MAXIMIZED) out |= SDL_WINDOW_MAXIMIZED;
		if (flags & Flags::MODAL) out |= SDL_WINDOW_MODAL;
		if (flags & Flags::ALWAYS_ON_TOP) out |= SDL_WINDOW_ALWAYS_ON_TOP;
		if (flags & Flags::UTILITY) out |= SDL_WINDOW_UTILITY;
		if (flags & Flags::TOOLTIP) out |= SDL_WINDOW_TOOLTIP;
		if (flags & Flags::POPUP_MENU) out |= SDL_WINDOW_POPUP_MENU;
		if (flags & Flags::TRANSPARENT) out |= SDL_WINDOW_TRANSPARENT;
		if (flags & Flags::HIDDEN) out |= SDL_WINDOW_HIDDEN;
		if (flags & Flags::NOT_FOCUSABLE) out |= SDL_WINDOW_NOT_FOCUSABLE;
		if (flags & Flags::MOUSE_CAPTURE) out |= SDL_WINDOW_MOUSE_CAPTURE;

		return out;
	}

    Window::Window(const Config& config) :
            _engine{config._engine},
            _event{config._events},
            _handle{nullptr}{
        createLogger();
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0){
            SPDLOG_LOGGER_TRACE(_logger, "Initializing SDL3 ...");

            if (!SDL_Init(SDL_INIT_VIDEO)){
                SPDLOG_LOGGER_ERROR(_logger, "Failed to initialize SDL3 : %s", SDL_GetError());
                throw std::runtime_error("Failed to initialize SDL3");
            }

            SPDLOG_LOGGER_TRACE(_logger, "SDL3 initialized successfully");
        }

        SPDLOG_LOGGER_TRACE(_logger, "Creating SDL3 window ...");

        _handle = SDL_CreateWindow(
            config._title.c_str(),
            config._size.x,
            config._size.y,
            raindropToSDLWindowFlags(config._flags)
        );

        if (!_handle){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to create SDL3 window : %s", SDL_GetError());
            throw std::runtime_error("Failed to create window");
        }

		setPosition(config._position);

        SPDLOG_LOGGER_TRACE(_logger, "SDL3 window created succsessfully");
    }

    void Window::createLogger(){
        _logger = spdlog::get("Raindrop::Window");
        if (!_logger){
            _logger = spdlog::stdout_color_mt("Raindrop::Window");
        }
    }

    Window::~Window(){
        if (_handle){
            SPDLOG_LOGGER_TRACE(_logger, "Destroying SDL3 Window");
            SDL_DestroyWindow(static_cast<SDL_Window*>(_handle));
        }
        
        int count;
        SDL_GetWindows(&count);

        if (count == 0){
            SPDLOG_LOGGER_TRACE(_logger, "Shutting down SDL3 ...");
            SDL_Quit();
        }
    }

    glm::ivec2 Window::getSize() const{
        int w, h;
        SDL_GetWindowSizeInPixels(static_cast<SDL_Window*>(_handle), &w, &h);
        return glm::ivec2(w, h);
    }

    glm::ivec2 Window::getPosition() const{
        int x, y;
        SDL_GetWindowPosition(static_cast<SDL_Window*>(_handle), &x, &y);
        return glm::ivec2(x, y);
    }

    const char* Window::getTitle() const{
        return SDL_GetWindowTitle(static_cast<SDL_Window*>(_handle));
    }

    bool Window::isFullscreen() const{
        return SDL_GetWindowFlags(static_cast<SDL_Window*>(_handle)) & SDL_WINDOW_FULLSCREEN;
    }

    bool Window::isMinimized() const{
        return SDL_GetWindowFlags(static_cast<SDL_Window*>(_handle)) & SDL_WINDOW_MINIMIZED;
    }

    bool Window::isMaximized() const{
        return SDL_GetWindowFlags(static_cast<SDL_Window*>(_handle)) & SDL_WINDOW_MAXIMIZED;
    }

    bool Window::resized() const{
        return false;
    }

    bool Window::moved() const{
        return false;
    }
    
    void Window::setSize(glm::ivec2 size){
        SDL_SetWindowSize(static_cast<SDL_Window*>(_handle), size.x, size.y);
    }

    void Window::setPosition(glm::ivec2 position){
        SDL_SetWindowPosition(static_cast<SDL_Window*>(_handle), position.x, position.y);
    }

    void* Window::getNativeHandle() const{
        return _handle;
    }

	std::unique_ptr<SurfaceProvider> Window::getSurfaceProvider(std::string_view API){
		if (API == "Vulkan"){
			return std::make_unique<SDL2VulkanSurfaceProvider>(*this);
		} else if (API == "Metal"){
			return std::make_unique<SDL2MetalSurfaceProvider>(*this);
		}

		return nullptr;
	}
    
	void Window::events(){
		// resetFlags();
		// _stateManager.reset();
		SDL_Event e;

		while (SDL_PollEvent(&e)){
			switch (e.type){

			case SDL_EVENT_TERMINATING: terminatingEvent(e); break;
			case SDL_EVENT_LOW_MEMORY: lowMemoryEvent(e); break;

			case SDL_EVENT_WILL_ENTER_BACKGROUND: willEnterBackgroundEvent(e); break;
			case SDL_EVENT_DID_ENTER_BACKGROUND: didEnterBackgroundEvent(e); break;
			case SDL_EVENT_WILL_ENTER_FOREGROUND: willEnterForgroundEvent(e); break;
			case SDL_EVENT_DID_ENTER_FOREGROUND: didEnterForgourndEvent(e); break;

			case SDL_EVENT_LOCALE_CHANGED: localeChangedEvent(e); break;
			case SDL_EVENT_SYSTEM_THEME_CHANGED: systemThemChangedEvent(e); break;
			case SDL_EVENT_DISPLAY_ORIENTATION: displayOrientationEvent(e); break;
			case SDL_EVENT_DISPLAY_ADDED: displayAddedEvent(e); break;
			case SDL_EVENT_DISPLAY_REMOVED: displayRemovedEvent(e); break;
			case SDL_EVENT_DISPLAY_MOVED: displayMovedEvent(e); break;
			case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: displayContentScaleChangedEvent(e); break;

			case SDL_EVENT_WINDOW_SHOWN: windowShownEvent(e); break;
			case SDL_EVENT_WINDOW_HIDDEN: windowHiddenEvent(e); break;
			case SDL_EVENT_WINDOW_EXPOSED: windowExposedEvent(e); break;
			case SDL_EVENT_WINDOW_MOVED: windowMovedEvent(e); break;
			case SDL_EVENT_WINDOW_RESIZED: windowResizedEvent(e); break;
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: windowPixelSizeChangedEvent(e); break;
			case SDL_EVENT_WINDOW_MINIMIZED: windowMinimizedEvent(e); break;
			case SDL_EVENT_WINDOW_MAXIMIZED: windowMaximizedEvent(e); break;
			case SDL_EVENT_WINDOW_RESTORED: windowRestoredEvent(e); break;
			case SDL_EVENT_WINDOW_MOUSE_ENTER: windowMouseEnterEvent(e); break;
			case SDL_EVENT_WINDOW_MOUSE_LEAVE: windowMouseLeaveEvent(e); break;
			case SDL_EVENT_WINDOW_FOCUS_GAINED: windowFocusGainedEvent(e); break;
			case SDL_EVENT_WINDOW_FOCUS_LOST: windowFocusLostEvent(e); break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED: windowCloseRequestedEvent(e); break;
			case SDL_EVENT_WINDOW_HIT_TEST: windowHitTestEvent(e); break;
			case SDL_EVENT_WINDOW_ICCPROF_CHANGED: windowICCPROFChangedEvent(e); break;
			case SDL_EVENT_WINDOW_DISPLAY_CHANGED: windowDisplayChangedEvent(e); break;
			case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: windowDisplayScaleChangedEvent(e); break;
			case SDL_EVENT_WINDOW_OCCLUDED: windowOccludedEvent(e); break;
			case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: windowEnterFullsreenEvent(e); break;
			case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: windowLeaveFullscreenEvent(e); break;
			case SDL_EVENT_WINDOW_DESTROYED: windowDestroyedEvent(e); break;
			case SDL_EVENT_WINDOW_HDR_STATE_CHANGED: displayHDRStateChnagedEvent(e); break;

			case SDL_EVENT_KEY_DOWN: keyDownEvent(e); break;
			case SDL_EVENT_KEY_UP: keyUpEvent(e); break;
			case SDL_EVENT_TEXT_EDITING: textEditingEvent(e); break;
			case SDL_EVENT_TEXT_INPUT: textInputEvent(e); break;
			case SDL_EVENT_KEYMAP_CHANGED: keymapChangedEvent(e); break;
		
			case SDL_EVENT_MOUSE_MOTION: mouseMotionEvent(e); break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN: mouseButtonDownEvent(e); break;
			case SDL_EVENT_MOUSE_BUTTON_UP: mouseButtonUpEvent(e); break;
			case SDL_EVENT_MOUSE_WHEEL: mouseWheelEvent(e); break;

			#ifdef EVENT_JOYSTICK
				case SDL_EVENT_JOYSTICK_AXIS_MOTION: joystickAxisMotionEvent(e); break;
				case SDL_EVENT_JOYSTICK_HAT_MOTION: joystickHatMotionEvent(e); break;
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN: joystickButtonDownEvent(e); break;
				case SDL_EVENT_JOYSTICK_BUTTON_UP: joystickButtonUpEvent(e); break;
				case SDL_EVENT_JOYSTICK_ADDED: joystickAddedEvent(e); break;
				case SDL_EVENT_JOYSTICK_REMOVED: joystickRemovedEvent(e); break;
				case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: joystickBatteryUpdatedEvent(e); break;
				case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: joystickUpdateCompletedEvent(e); break;
			#endif

			#ifdef EVENT_GAMEPAD
				case SDL_EVENT_GAMEPAD_AXIS_MOTION: gamepadAxisMotionEvent(e); break;
				case SDL_EVENT_GAMEPAD_BUTTON_DOWN: gamepadButtonDownEvent(e); break;
				case SDL_EVENT_GAMEPAD_BUTTON_UP: gamepadButtonUpEvent(e); break;
				case SDL_EVENT_GAMEPAD_ADDED: gamepadAddedEvent(e); break;
				case SDL_EVENT_GAMEPAD_REMOVED: gamepadRemovedEvent(e); break;
				case SDL_EVENT_GAMEPAD_REMAPPED: gamepadRemappedEvent(e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: gamepadTouchpadDownEvent(e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: gamepadTouchpadMotionEvent(e); break;
				case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: gamepadTouchpadUpEvent(e); break;
				case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: gamepadSensorUpdateEvent(e); break;
				case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: gamepadUpdateCompleteEvent(e); break;
				case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: gamepadSteamHandleUpdatedEvent(e); break;
			#endif
			
			#ifdef EVENT_FINGER
				case SDL_EVENT_FINGER_DOWN: fingerDownEvent(e); break;
				case SDL_EVENT_FINGER_UP: fingerUpEvent(e); break;
				case SDL_EVENT_FINGER_MOTION: fingerMotionEvent(e); break;
			#endif
		
			#ifdef EVENT_CLIPBOARD
				case SDL_EVENT_CLIPBOARD_UPDATE: clipboardUpdateEvent(e); break;
			#endif

			#ifdef EVENT_TEXT
				case SDL_EVENT_DROP_FILE: dropFileEvent(e); break;
				case SDL_EVENT_DROP_TEXT: dropTextEvent(e); break;
				case SDL_EVENT_DROP_BEGIN: dropBeginEvent(e); break;
				case SDL_EVENT_DROP_COMPLETE: dropCompleteEvent(e); break;
				case SDL_EVENT_DROP_POSITION: dropPositionEvent(e); break;
			#endif

			#ifdef EVENT_AUDIO
				case SDL_EVENT_AUDIO_DEVICE_ADDED: audioDeviceAddedEvent(e); break;
				case SDL_EVENT_AUDIO_DEVICE_REMOVED: audioDeviceRemovedEvent(e); break;
				case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED: audioDeviceFormatChangedEvent(e); break;
			#endif

			#ifdef EVENR_SENSOR
				case SDL_EVENT_SENSOR_UPDATE: sensorUpdateEvent(e); break;
			#endif

			#ifdef EVENT_PEN
				case SDL_EVENT_PEN_DOWN: penDownEvent(e); break;
				case SDL_EVENT_PEN_UP: penUpEvent(e); break;
				case SDL_EVENT_PEN_MOTION: penMotionEvent(e); break;
				case SDL_EVENT_PEN_BUTTON_DOWN: penButtonDownEvent(e); break;
				case SDL_EVENT_PEN_BUTTON_UP: penButtonUpEvent(e); break;
			#endif

			#ifdef EVENT_CAMERA
				case SDL_EVENT_CAMERA_DEVICE_ADDED: cameraDeviceAddedEvent(e); break;
				case SDL_EVENT_CAMERA_DEVICE_REMOVED: cameraDeviceRemovedEvent(e); break;
				case SDL_EVENT_CAMERA_DEVICE_APPROVED: cameraDeviceApprovedEvent(e); break;
				case SDL_EVENT_CAMERA_DEVICE_DENIED: cameraDeviceDeniedEvent(e); break;
			#endif
			
			#ifdef EVENT_RENDER
				case SDL_EVENT_RENDER_TARGETS_RESET: renderTargetsResetEvent(e); break;
				case SDL_EVENT_RENDER_DEVICE_RESET: renderDeviceResetEvent(e); break;
			#endif
			}
		}
	}

	void Window::terminatingEvent(SDL_Event&){}
	void Window::lowMemoryEvent(SDL_Event&){}

	void Window::willEnterBackgroundEvent(SDL_Event&){}
	void Window::didEnterBackgroundEvent(SDL_Event&){}
	void Window::willEnterForgroundEvent(SDL_Event&){}
	void Window::didEnterForgourndEvent(SDL_Event&){}

	void Window::localeChangedEvent(SDL_Event&){}
	void Window::systemThemChangedEvent(SDL_Event&){}

	void Window::displayOrientationEvent(SDL_Event&){}
	void Window::displayAddedEvent(SDL_Event&){}
	void Window::displayRemovedEvent(SDL_Event&){}
	void Window::displayMovedEvent(SDL_Event&){}
	void Window::displayContentScaleChangedEvent(SDL_Event&){}
	void Window::displayHDRStateChnagedEvent(SDL_Event&){}

	void Window::windowShownEvent(SDL_Event&){
		_event->trigger(WindowShown(*this));
	}

	void Window::windowHiddenEvent(SDL_Event&){
		_event->trigger(WindowHidden(*this));
	}

	void Window::windowExposedEvent(SDL_Event&){
		// _event->trigger(WindowExposed(*this));
	}

	void Window::windowMovedEvent(SDL_Event& e){
		glm::u32vec2 position{
			static_cast<uint32_t>(e.window.data1),
			static_cast<uint32_t>(e.window.data2)
		};

		// flagMoved();

		_event->trigger(WindowMoved(*this, position));
	}

	void Window::windowResizedEvent(SDL_Event& e){
		glm::u32vec2 size{
			static_cast<uint32_t>(e.window.data1),
			static_cast<uint32_t>(e.window.data2)
		};

		// flagResized();

		_event->trigger(WindowResized(*this, size));
	}

	void Window::windowPixelSizeChangedEvent(SDL_Event&){
		
	}

	void Window::windowMinimizedEvent(SDL_Event&){
		_event->trigger(WindowMinimized(*this));
	}

	void Window::windowMaximizedEvent(SDL_Event&){
		_event->trigger(WindowMaximized(*this));
	}

	void Window::windowRestoredEvent(SDL_Event&){
		_event->trigger(WindowRestored(*this));
	}

	void Window::windowMouseEnterEvent(SDL_Event&){
		_event->trigger(WindowMouseEntered(*this));
	}

	void Window::windowMouseLeaveEvent(SDL_Event&){
		_event->trigger(WindowMouseLeaved(*this));
	}

	void Window::windowFocusGainedEvent(SDL_Event&){
		_event->trigger(WindowFocusGained(*this));
	}

	void Window::windowFocusLostEvent(SDL_Event&){
		_event->trigger(WindowFocusLost(*this));
	}

	void Window::windowCloseRequestedEvent(SDL_Event&){
		_event->trigger(WindowCloseRequest(*this));
	}

	void Window::windowTakeFocusEvent(SDL_Event&){}
	void Window::windowHitTestEvent(SDL_Event&){}
	void Window::windowICCPROFChangedEvent(SDL_Event&){}
	void Window::windowDisplayChangedEvent(SDL_Event&){}
	void Window::windowDisplayScaleChangedEvent(SDL_Event&){}

	void Window::windowOccludedEvent(SDL_Event&){
		_event->trigger(WindowOccluded(*this));
	}

	void Window::windowEnterFullsreenEvent(SDL_Event&){
		_event->trigger(WindowFullscreenEnter(*this));
	}

	void Window::windowLeaveFullscreenEvent(SDL_Event&){
		_event->trigger(WindowFullscreenLeave(*this));
	}

	void Window::windowDestroyedEvent(SDL_Event&){
		_event->trigger(WindowDestroyed());
	}

	void Window::windowPenEnterEvent(SDL_Event&){}
	void Window::windowPenLeaveEvent(SDL_Event&){}

	void Window::keyDownEvent(SDL_Event& e){
		Input::Key key = static_cast<Input::Key>(e.key.scancode);
		uint32_t repeat = e.key.repeat;
		_event->trigger(Input::KeyDown(key, repeat));
	}

	void Window::keyUpEvent(SDL_Event& e){
		Input::Key key = static_cast<Input::Key>(e.key.scancode);
		_event->trigger(Input::KeyUp(key));
	}

	void Window::textEditingEvent(SDL_Event&){}
	void Window::textInputEvent(SDL_Event&){}
	void Window::keymapChangedEvent(SDL_Event&){}

	void Window::mouseMotionEvent(SDL_Event& e){
		glm::u32vec2 position{
			static_cast<std::uint32_t>(e.motion.x),
			static_cast<std::uint32_t>(e.motion.y)
		};

		glm::i32vec2 delta{
			static_cast<std::int32_t>(e.motion.xrel),
			static_cast<std::int32_t>(e.motion.yrel)
		};

		_event->trigger(Input::MouseMovedEvent(position, delta));
	}

	void Window::mouseButtonDownEvent(SDL_Event& e){
		Input::MouseButton button = static_cast<Input::MouseButton>(e.button.button);
		std::uint32_t clicks = e.button.clicks;
		
		glm::u32vec2 position{
			static_cast<std::uint32_t>(e.button.x),
			static_cast<std::uint32_t>(e.button.y)
		};

		_event->trigger(Input::MouseButtonDownEvent(button, clicks, position));
	}

	void Window::mouseButtonUpEvent(SDL_Event& e){
		Input::MouseButton button = static_cast<Input::MouseButton>(e.button.button);
		
		glm::u32vec2 position{
			static_cast<std::uint32_t>(e.button.x),
			static_cast<std::uint32_t>(e.button.y)
		};

		_event->trigger(Input::MouseButtonUpEvent(button, position));
	}

	void Window::mouseWheelEvent(SDL_Event& e){
		_event->trigger(Input::MouseScrollEvent(e.wheel.y));
	}

	// bool Window::wasResized() const noexcept{
	// 	return _flags & static_cast<Flags>(FlagsBits::RESIZED);
	// }

	// bool Window::wasMoved() const noexcept{
	// 	return _flags & static_cast<Flags>(FlagsBits::MOVED);
	// }

	// bool Window::wasExposed() const noexcept{
	// 	return _flags & static_cast<Flags>(FlagsBits::EXPOSED);
	// }

	// bool Window::wasShown() const noexcept{
	// 	return _flags & static_cast<Flags>(FlagsBits::SHOWN);
	// }

	// bool Window::wasHidden() const noexcept{
	// 	return _flags & static_cast<Flags>(FlagsBits::HIDDEN);
	// }

	// void Window::quitEvent(SDL_Event &e){
	// 	_context.getInternalContext().getEventManager().get("quit").trigger();
	// }
	
	// void Window::terminatingEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("terminating").trigger();
	// }

	// void Window::lowMemoryEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("memory.low").trigger();
	// }

	// void Window::willEnterBackgroundEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("will_enter_background").trigger();
	// }

	// void Window::didEnterBackgroundEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("did_enter_background").trigger();
	// }

	// void Window::willEnterForgroundEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("will_enter_forground").trigger();
	// }

	// void Window::didEnterForgourndEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("did_enter_forground").trigger();
	// }

	// void Window::localeChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("locale.changed").trigger();
	// }

	// void Window::systemThemChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("system_theme.changed").trigger();
	// }

	// void Window::displayOrientationEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("display.orientation").trigger(e.display.data1);
	// }

	// void Window::displayAddedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("display.added").trigger();
	// }

	// void Window::displayRemovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("display.removed").trigger();
	// }

	// void Window::displayMovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("display.moved").trigger();
	// }

	// void Window::displayContentScaleChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("display.content_scale.changed").trigger();
	// }

	// void Window::displayHDRStateChnagedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("display.HDR_state.changed").trigger();
	// }

	// void Window::windowShownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.shown").trigger();
	// }

	// void Window::windowHiddenEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.hidden").trigger();
	// }

	// void Window::windowExposedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.exposed").trigger();
	// }

	// void Window::windowMovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.moved").trigger();
	// }

	// void Window::windowResizedEvent(SDL_Event&){
	// 	_resized = true;
	// 	_context.getInternalContext().getEventManager().get("window.resized").trigger(Maths::ivec2(e.window.data1, e.window.data2));
	// }

	// void Window::windowPixelSizeChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.pixel_size.changed").trigger(Maths::ivec2(e.window.data1, e.window.data2));
	// }

	// void Window::windowMinimizedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.minimized").trigger();
	// }

	// void Window::windowMaximizedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.maximized").trigger();
	// }

	// void Window::windowRestoredEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.restored").trigger();
	// }

	// void Window::windowMouseEnterEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.mouse.enter").trigger();
	// }

	// void Window::windowMouseLeaveEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.mouse.leave").trigger();
	// }

	// void Window::windowFocusGainedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.focus.gained").trigger();
	// }

	// void Window::windowFocusLostEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.focus.lost").trigger();
	// }

	// void Window::windowCloseRequestedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.close.requested").trigger();
	// }

	// void Window::windowTakeFocusEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.focus.take").trigger();
	// }

	// void Window::windowHitTestEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.hit_test").trigger();
	// }

	// void Window::windowICCPROFChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.ICCPROF.changed").trigger();
	// }

	// void Window::windowDisplayChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.display.changed").trigger(e.window.data1);
	// }

	// void Window::windowDisplayScaleChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.display.scale.changed").trigger();
	// }

	// void Window::windowOccludedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.occluded").trigger();
	// }

	// void Window::windowEnterFullsreenEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.fullscreen.enter").trigger();
	// }

	// void Window::windowLeaveFullscreenEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.fullscreen.leave").trigger();
	// }

	// void Window::windowDestroyedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.destroyed").trigger();
	// }

	// void Window::windowPenEnterEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.pen.enter").trigger();
	// }

	// void Window::windowPenLeaveEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("window.pen.leave").trigger();
	// }

	// void Window::keyDownEvent(SDL_Event&){
	// 	const auto& keysym = e.key.keysym;
	// 	_context.getInternalContext().getEventManager().get("key.down").trigger(SDL_ScancodeToKey(keysym.scancode), SDL_KeycodeToKey(keysym.sym), keysym.mod);
	// }

	// void Window::keyUpEvent(SDL_Event&){
	// 	const auto& keysym = e.key.keysym;
	// 	_context.getInternalContext().getEventManager().get("key.up").trigger(SDL_ScancodeToKey(keysym.scancode), SDL_KeycodeToKey(keysym.sym), keysym.mod);
	// }

	// void Window::textEditingEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("text.editing").trigger(e.edit.text, e.edit.start, e.edit->length);
	// }

	// void Window::textInputEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("text.input").trigger(e.text.text);
	// }

	// void Window::keymapChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("keymap.changed").trigger();
	// }

	// void Window::mouseMotionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("mouse.motion").trigger(Maths::vec2(e.motion.x, e.motion.y), Maths::vec2(e.motion.xrel, e.motion.yrel));
	// }

	// void Window::mouseButtonDownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("mouse.button.down").trigger(e.button.button, e.button.clicks, Maths::vec2(e.button.x, e.button.y));
	// }

	// void Window::mouseButtonUpEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("mouse.button.up").trigger(e.button.button, Maths::vec2(e.button.x, e.button.y));
	// }

	// void Window::mouseWheelEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("mouse.wheel").trigger(Maths::vec2(e.wheel.x, e.wheel.y), Maths::vec2(e.wheel.mouse_x, e.wheel.mouse_y));
	// }

	// void Window::joystickAxisMotionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.axis.motion").trigger(e.jaxis.axis, e.jaxis.value);
	// }

	// void Window::joystickHatMotionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.hat.motion").trigger(e.jhat.hat, e.jhat.value);
	// }

	// void Window::joystickButtonDownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.button.down").trigger(e.jbutton.button);
	// }

	// void Window::joystickButtonUpEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.button.up").trigger(e.jbutton.button);
	// }

	// void Window::joystickAddedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.added").trigger();
	// }

	// void Window::joystickRemovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.removed").trigger();
	// }

	// void Window::joystickBatteryUpdatedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.battery.updated").trigger(e.jbattery.level);
	// }

	// void Window::joystickUpdateCompletedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("joystick.update.complete").trigger();
	// }

	// void Window::gamepadAxisMotionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.axis.motion").trigger(e.gaxis.axis, e.gaxis.value);
	// }

	// void Window::gamepadButtonDownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.button.down").trigger(e.gbutton.button);
	// }

	// void Window::gamepadButtonUpEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.button.up").trigger(e.gbutton.button);
	// }

	// void Window::gamepadAddedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.added").trigger();
	// }

	// void Window::gamepadRemovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.removed").trigger();
	// }

	// void Window::gamepadRemappedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.remapped").trigger();
	// }

	// void Window::gamepadTouchpadDownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.touchpad.down").trigger(Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	// }
	
	// void Window::gamepadTouchpadMotionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.touchpad.motion").trigger(Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	// }

	// void Window::gamepadTouchpadUpEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.touchpad.up").trigger(Maths::vec2(e.gtouchpad.x, e.gtouchpad.y), e.gtouchpad.pressure);
	// }

	// void Window::gamepadSensorUpdateEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.sensor.update").trigger(e.gsensor.sensor_timestamp, Maths::vec3(e.gsensor.data[0], e.gsensor.data[1], e.gsensor.data[2]));
	// }

	// void Window::gamepadUpdateCompleteEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.update.complete").trigger();
	// }

	// void Window::gamepadSteamHandleUpdatedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("gamepad.steam_handle_updated").trigger();
	// }

	// void Window::fingerDownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("finger.down").trigger(e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y));
	// }

	// void Window::fingerUpEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("finger.up").trigger(e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y));
	// }

	// void Window::fingerMotionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("finger.motion").trigger(e.tfinger.fingerID, Maths::vec2(e.tfinger.x, e.tfinger.y), Maths::vec2(e.tfinger.dx, e.tfinger.dy));
	// }

	// void Window::clipboardUpdateEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("clipboard.update").trigger();
	// }

	// void Window::dropFileEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("drop.file").trigger();
	// }

	// void Window::dropTextEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("drop.text").trigger();
	// }

	// void Window::dropBeginEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("drop.begin").trigger();
	// }

	// void Window::dropCompleteEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("drop.complete").trigger();
	// }

	// void Window::dropPositionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("drop.position").trigger();
	// }

	// void Window::audioDeviceAddedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("audio_device.added").trigger();
	// }

	// void Window::audioDeviceRemovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("audio_device.removed").trigger();
	// }

	// void Window::audioDeviceFormatChangedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("audio_device.format_changed").trigger();
	// }

	// void Window::sensorUpdateEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("sensor.update").trigger();
	// }

	// void Window::penDownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("pen.down").trigger();
	// }

	// void Window::penUpEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("pen.up").trigger();
	// }

	// void Window::penMotionEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("pen.motion").trigger();
	// }

	// void Window::penButtonDownEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("pen.button.down").trigger();
	// }

	// void Window::penButtonUpEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("pen.button.up").trigger();
	// }

	// void Window::cameraDeviceAddedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("camera_device.added").trigger();
	// }

	// void Window::cameraDeviceRemovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("camera_device.removed").trigger();
	// }

	// void Window::cameraDeviceApprovedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("camera_device.approved").trigger();
	// }

	// void Window::cameraDeviceDeniedEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("camera_device.denied").trigger();
	// }

	// void Window::renderTargetsResetEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("render_targets.reset").trigger();
	// }

	// void Window::renderDeviceResetEvent(SDL_Event&){
	// 	_context.getInternalContext().getEventManager().get("render_device.reset").trigger();
	// }
}