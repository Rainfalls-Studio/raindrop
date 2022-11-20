/**
 * @brief this file contain the default event types's name and data
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @attention IF YOU DO ANY CHANGE IN THIS FILE, MAKE SURE THAT THE WHOLE PROJECT IS RECOMPILED
 * @authors @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

/**
 * MIT License
 * Copyright (c) 2022 Rainfalls studio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 

 * 
 */

#ifndef __RAINDROP_EVENTS_DEFAULT_EVENTS_HPP__
#define __RAINDROP_EVENTS_DEFAULT_EVENTS_HPP__

#include "core.hpp"
#include "EventMediator.hpp"
#include "window/Window.hpp"
#include "mouseButtons.hpp"
#include "joystick.hpp"
#include "controller.hpp"

namespace rnd::events{
	enum class DisplayOrentation{
		LANDSCAPE,
		LANDSCAPE_FLIPPED,
		PORTRAIT,
		PORTRAIT_FLIPPED,
	};

	namespace defaults{
		enum Event{
			QUIT,
			APP_TERMINATING,
			APP_LOW_MEMORY,
			APP_WILL_ENTER_BACKGROUND,
			APP_DID_ENTER_BACKGROUND,
			APP_WILL_ENTER_FORGROUND,
			APP_DID_ENTER_FORGROUND,
			LOCAL_CHANGED,
			DISPLAY_ORIENTATION,
			DISPLAY_CONNECTED,
			DISPLAY_DISCONNECTED,
			WINDOW_SHOWN,
			WINDOW_HIDDEN,
			WINDOW_EXPOSED,
			WINDOW_RESIZED,
			WINDOW_SIZE_CHANGED,
			WINDOW_MINIMIZED,
			WINDOW_MAXIMIZED,
			WINDOW_RESTORED,
			WINDOW_ENTER,
			WINDOW_LEAVE,
			WINDOW_FOCUS_GAINED,
			WINDOW_FOCUS_LOST,
			WINDOW_CLOSE,
			WINDOW_TAKE_FOCUS,
			WINDOW_DISPLAY_CHANGED,
			KEY_DOWN,
			KEY_UP,
			TEXT_EDITING,
			TEXT_INPUT,
			KEY_MAP_CHANGED,
			MOUSE_MOTION,
			MOUSE_BUTTON_DOWN,
			MOUSE_BUTTON_UP,
			MOUSE_WHEEL,
			JOYSTICK_AXIS_MOTION,
			JOYSTICK_BALL_MOTION,
			JOYSTICK_HAT_MOTION,
			JOYSTICK_BUTTON_DOWN,
			JOYSTICK_BUTTON_UP,
			JOYSTICK_DEVICE_ADDED,
			JOYSTICK_DEVICE_REMOVED,
			CONTROLLER_AXIS_MOTION,
			CONTROLLER_BUTTON_DOWN,
			CONTROLLER_BUTTON_UP,
			CONTROLLER_DEVICE_ADDED,
			CONTROLLER_DEVICE_REMOVED,
			CONTROLLER_DEVICE_REMAPED,
			CONTROLLER_TOUCHPAD_DOWN,
			CONTROLLER_TOUCHPAD_UP,
			CONTROLLER_TOUCHPAD_MOTION,
			CONTROLLER_SENSOR_UPDATE,
			FINGER_DOWN,
			FINGER_UP,
			FINGER_MOTION,
			DOLLAR_GERSTURE,
			DOLLAR_RECORD,
			MULTI_GESTURE,
			CLIPBOARD_UPDATE,
			DROP_FILE,
			DROP_TEXT,
			DROP_BEGIN,
			DROP_COMPLETE,
			AUDIO_DEVICE_ADDED,
			AUDIO_DEVICE_REMOVED,
			SENSOR_UPDATE,

			COUNT,
		};

		struct DisplayConnectedEventData{
			uint32_t displayID;
		};
		using DisplayDisconnectedEventData = DisplayConnectedEventData;

		struct DisplayOrentationEventData{
			uint32_t displayID;
			DisplayOrentation orientation;
		};

		struct WindowShownEventData{
			const window::Window* window;
		};
		using WindowHiddenEventData = WindowShownEventData;
		using WindowExposedEventData = WindowShownEventData;
		using WindowMinimizedEventData = WindowShownEventData;
		using WindowMaximizedEventData = WindowShownEventData;
		using WindowRestoredEventData = WindowShownEventData;
		using WindowEnterEventData = WindowShownEventData;
		using WindowLeaveEventData = WindowShownEventData;
		using WindowFocusGainedEventData = WindowShownEventData;
		using WindowFocusLostEventData = WindowShownEventData;
		using WindowCloseEventData = WindowShownEventData;
		using WindowTakeFocusEventData = WindowShownEventData;

		struct WindowResizedEventData{
			const window::Window* window;
			Extent extent;
		};
		using WindowSizeChangedEventData = WindowResizedEventData;

		struct WindowDisplayChangedEventData{
			window::Window& window;
			uint32_t displayID;
		};

		struct KeyDownEventData{
			const window::Window* window;
			bool repeate;
			keys::vKey virtualKey;
		};

		struct KeyUpEventData{
			const window::Window* window;
			keys::vKey virtualKey;
		};

		struct TextEditingEventData{
			const window::Window* window;
			char text[32];
			int start;
			uint32_t lenght;
		};

		struct TextInputEventData{
			const window::Window* window;
			char text[32];
		};

		struct MouseMotionEventData{
			const window::Window* window;
			glm::ivec2 pos;
			glm::vec2 rel;
		};

		struct MouseButtonDownEventData{
			const window::Window* window;
			MouseButton button;
			uint8_t clicks;
			glm::ivec2 pos;
		};

		struct MouseButtonUpEventData{
			const window::Window* window;
			MouseButton button;
			glm::ivec2 pos;
		};

		struct MouseWheelEventData{
			const window::Window* window;
			glm::vec2 scrool;
		};

		struct JoystickAxisMotionEventData{
			// const events::Joystick* joystick; // TODO: RAI-53
			uint8_t axis;
			float range; // [-1;1]
		};

		struct JoystickBallMotionEventData{
			// const events::Joystick* joystick; // TODO: RAI-53
			uint8_t ball;
			glm::vec2 motion;
		};

		struct JoystickHatMotionEventData{
			// const events::Joystick* joystick; // TODO: RAI-53
			uint8_t hat;
			HatPosition position;
		};

		struct JoystickButtonDownEventData{
			// const events::Joystick* joystick; // TODO: RAI-53
			uint8_t button;
		};

		struct JoystickButtonUpEventData{
			// const events::Joystick* joystick; // TODO: RAI-53
			uint8_t button;
		};

		struct JoystickDeviceAddedEventData{
			// const events::Joystick* joystick; // TODO: RAI-53
		};

		struct JoystickDeviceRemovedEventData{
			// const events::Joystick* joystick; // TODO: RAI-53
		};

		struct ControllerAxisMotionEventData{
			// const events::Controller* controller; // TODO: RAI-54
			ControllerAxis axis;
			float range; // [-1;1]
		};

		struct ControllerButtonDownEventData{
			// const events::Controller* controller; // TODO: RAI-54
			ControllerButton button;
		};

		struct ControllerButtonUpEventData{
			// const events::Controller* controller; // TODO: RAI-54
			ControllerButton button;
		};

		struct ControllerDeviceAddedEventData{
			// const events::Controller* controller; // TODO: RAI-54
		};

		struct ControllerDeviceRemovedEventData{
			// const events::Controller* controller; // TODO: RAI-54
		};

		struct ControllerDeviceRemappedEventData{
			// const events::Controller* controller; // TODO: RAI-54
		};

		struct ControllerTouchpadDownEventData{
			// const events::Controller* controller; // TODO: RAI-54
			int index;
			int finger;
			glm::vec2 pos; // [-1;1]
			float pressure;
		};

		struct ControllerTouchpadUpEventData{
			// const events::Controller* controller; // TODO: RAI-54
			int index;
			int finger;
			glm::vec2 pos; // [-1;1]
		};

		struct ControllerTouchpadMotionEventData{
			// const events::Controller* controller; // TODO: RAI-54
			int index;
			int finger;
			glm::vec2 pos; // [-1;1]
			float pressure;
		};

		struct ControllerSensorUpdateEventData{
			// const events::Controller* controller; // TODO: RAI-54
			SensorType sensor;
			glm::vec3 values;
		};

		struct FingerDownEventData{
			const window::Window* window;
			// const event::Finger* finger; // TODO: RAI-55
			glm::i32vec2 pos;
			float pressure;
		};

		struct FingerUpEventData{
			const window::Window* window;
			// const event::Finger* finger; // TODO: RAI-55
		};

		struct FingerMotionEventData{
			const window::Window* window;
			// const event::Finger* finger; // TODO: RAI-55
			glm::i32vec2 pos;
			float pressure;
		};

		struct MultiGestureEventData{
			// TODO
		};

		struct DollarGestureEvent{
			// TODO
		};

		struct ClipboardEventData{
			// TODO: RAI-61
		};

		struct DropFileEventData{
			const window::Window* window;
			char* file;
		};

		struct DropTextEventData{
			const window::Window* window;
			char* text;
		};

		struct DropBeginEventData{
			const window::Window* window;
		};

		struct DropCompleteEventData{
			const window::Window* window;
		};

		struct AudioDeviceAddedEventData{
			// const audio::Device* device; // TODO: RAI-59
		};

		struct AudioDeviceRemovedEventData{
			// const audio::Device* device; // TODO: RAI-59
		};

		struct SensorUpdateEventData{
			// const events::Sensor* sensor; // TODO: RAI-60
			float data[6];
		};

		void init(Mediator &mediator);

		EventID getID(Event event);
		const char* getName(Event event);
	}
}

#endif