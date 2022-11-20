#include "events/defaultEvents.hpp"

namespace rnd::events::defaults{
	
	static EventID IDs[COUNT];

	void init(Mediator &mediator){
		PROFILE_FUNCTION();
		IDs[QUIT] = mediator.registerEvent(getName(QUIT));
		IDs[APP_TERMINATING] = mediator.registerEvent(getName(APP_TERMINATING));
		IDs[APP_LOW_MEMORY] = mediator.registerEvent(getName(APP_LOW_MEMORY));
		IDs[APP_WILL_ENTER_BACKGROUND] = mediator.registerEvent(getName(APP_WILL_ENTER_BACKGROUND));
		IDs[APP_DID_ENTER_BACKGROUND] = mediator.registerEvent(getName(APP_DID_ENTER_BACKGROUND));
		IDs[APP_WILL_ENTER_FORGROUND] = mediator.registerEvent(getName(APP_WILL_ENTER_FORGROUND));
		IDs[APP_DID_ENTER_FORGROUND] = mediator.registerEvent(getName(APP_DID_ENTER_FORGROUND));
		IDs[LOCAL_CHANGED] = mediator.registerEvent(getName(LOCAL_CHANGED));
		IDs[DISPLAY_ORIENTATION] = mediator.registerEvent<DisplayOrentationEventData>(getName(DISPLAY_ORIENTATION));
		IDs[DISPLAY_CONNECTED] = mediator.registerEvent<DisplayConnectedEventData>(getName(DISPLAY_CONNECTED));
		IDs[DISPLAY_DISCONNECTED] = mediator.registerEvent<DisplayDisconnectedEventData>(getName(DISPLAY_DISCONNECTED));
		IDs[WINDOW_SHOWN] = mediator.registerEvent<WindowShownEventData>(getName(WINDOW_SHOWN));
		IDs[WINDOW_HIDDEN] = mediator.registerEvent<WindowHiddenEventData>(getName(WINDOW_HIDDEN));
		IDs[WINDOW_EXPOSED] = mediator.registerEvent<WindowExposedEventData>(getName(WINDOW_EXPOSED));
		IDs[WINDOW_RESIZED] = mediator.registerEvent<WindowResizedEventData>(getName(WINDOW_RESIZED));
		IDs[WINDOW_SIZE_CHANGED] = mediator.registerEvent<WindowSizeChangedEventData>(getName(WINDOW_SIZE_CHANGED));
		IDs[WINDOW_MINIMIZED] = mediator.registerEvent<WindowMinimizedEventData>(getName(WINDOW_MINIMIZED));
		IDs[WINDOW_MAXIMIZED] = mediator.registerEvent<WindowMaximizedEventData>(getName(WINDOW_MAXIMIZED));
		IDs[WINDOW_RESTORED] = mediator.registerEvent<WindowRestoredEventData>(getName(WINDOW_RESTORED));
		IDs[WINDOW_ENTER] = mediator.registerEvent<WindowEnterEventData>(getName(WINDOW_ENTER));
		IDs[WINDOW_LEAVE] = mediator.registerEvent<WindowLeaveEventData>(getName(WINDOW_LEAVE));
		IDs[WINDOW_FOCUS_GAINED] = mediator.registerEvent<WindowFocusGainedEventData>(getName(WINDOW_FOCUS_GAINED));
		IDs[WINDOW_FOCUS_LOST] = mediator.registerEvent<WindowFocusLostEventData>(getName(WINDOW_FOCUS_LOST));
		IDs[WINDOW_CLOSE] = mediator.registerEvent<WindowCloseEventData>(getName(WINDOW_CLOSE));
		IDs[WINDOW_TAKE_FOCUS] = mediator.registerEvent<WindowTakeFocusEventData>(getName(WINDOW_TAKE_FOCUS));
		IDs[WINDOW_DISPLAY_CHANGED] = mediator.registerEvent<WindowDisplayChangedEventData>(getName(WINDOW_DISPLAY_CHANGED));
		IDs[KEY_DOWN] = mediator.registerEvent<KeyDownEventData>(getName(KEY_DOWN));
		IDs[KEY_UP] = mediator.registerEvent<KeyUpEventData>(getName(KEY_UP));
		IDs[TEXT_EDITING] = mediator.registerEvent<TextEditingEventData>(getName(TEXT_EDITING));
		IDs[TEXT_INPUT] = mediator.registerEvent<TextInputEventData>(getName(TEXT_INPUT));
		IDs[KEY_MAP_CHANGED] = mediator.registerEvent(getName(KEY_MAP_CHANGED));
		IDs[MOUSE_MOTION] = mediator.registerEvent<MouseMotionEventData>(getName(MOUSE_MOTION));
		IDs[MOUSE_BUTTON_DOWN] = mediator.registerEvent<MouseButtonDownEventData>(getName(MOUSE_BUTTON_DOWN));
		IDs[MOUSE_BUTTON_UP] = mediator.registerEvent<MouseButtonUpEventData>(getName(MOUSE_BUTTON_UP));
		IDs[MOUSE_WHEEL] = mediator.registerEvent<MouseWheelEventData>(getName(MOUSE_WHEEL));
		IDs[JOYSTICK_AXIS_MOTION] = mediator.registerEvent<JoystickAxisMotionEventData>(getName(JOYSTICK_AXIS_MOTION));
		IDs[JOYSTICK_BALL_MOTION] = mediator.registerEvent<JoystickBallMotionEventData>(getName(JOYSTICK_BALL_MOTION));
		IDs[JOYSTICK_HAT_MOTION] = mediator.registerEvent<JoystickHatMotionEventData>(getName(JOYSTICK_HAT_MOTION));
		IDs[JOYSTICK_BUTTON_DOWN] = mediator.registerEvent<JoystickButtonDownEventData>(getName(JOYSTICK_BUTTON_DOWN));
		IDs[JOYSTICK_BUTTON_UP] = mediator.registerEvent<JoystickButtonUpEventData>(getName(JOYSTICK_BUTTON_UP));
		IDs[JOYSTICK_DEVICE_ADDED] = mediator.registerEvent<JoystickDeviceAddedEventData>(getName(JOYSTICK_DEVICE_ADDED));
		IDs[JOYSTICK_DEVICE_REMOVED] = mediator.registerEvent<JoystickDeviceRemovedEventData>(getName(JOYSTICK_DEVICE_REMOVED));
		IDs[CONTROLLER_AXIS_MOTION] = mediator.registerEvent<ControllerAxisMotionEventData>(getName(CONTROLLER_AXIS_MOTION));
		IDs[CONTROLLER_BUTTON_DOWN] = mediator.registerEvent<ControllerButtonDownEventData>(getName(CONTROLLER_BUTTON_DOWN));
		IDs[CONTROLLER_BUTTON_UP] = mediator.registerEvent<ControllerButtonUpEventData>(getName(CONTROLLER_BUTTON_UP));
		IDs[CONTROLLER_DEVICE_ADDED] = mediator.registerEvent<ControllerDeviceAddedEventData>(getName(CONTROLLER_DEVICE_ADDED));
		IDs[CONTROLLER_DEVICE_REMOVED] = mediator.registerEvent<ControllerDeviceRemovedEventData>(getName(CONTROLLER_DEVICE_REMOVED));
		IDs[CONTROLLER_DEVICE_REMAPED] = mediator.registerEvent<ControllerDeviceRemappedEventData>(getName(CONTROLLER_DEVICE_REMAPED));
		IDs[CONTROLLER_TOUCHPAD_DOWN] = mediator.registerEvent<ControllerTouchpadDownEventData>(getName(CONTROLLER_TOUCHPAD_DOWN));
		IDs[CONTROLLER_TOUCHPAD_UP] = mediator.registerEvent<ControllerTouchpadUpEventData>(getName(CONTROLLER_TOUCHPAD_UP));
		IDs[CONTROLLER_TOUCHPAD_MOTION] = mediator.registerEvent<ControllerTouchpadMotionEventData>(getName(CONTROLLER_TOUCHPAD_MOTION));
		IDs[FINGER_DOWN] = mediator.registerEvent<FingerDownEventData>(getName(FINGER_DOWN));
		IDs[FINGER_UP] = mediator.registerEvent<FingerUpEventData>(getName(FINGER_UP));
		IDs[FINGER_MOTION] = mediator.registerEvent<FingerMotionEventData>(getName(FINGER_MOTION));
		IDs[DOLLAR_GERSTURE] = mediator.registerEvent<DollarGestureEvent>(getName(DOLLAR_GERSTURE));
		IDs[MULTI_GESTURE] = mediator.registerEvent<MultiGestureEventData>(getName(MULTI_GESTURE));
		IDs[CLIPBOARD_UPDATE] = mediator.registerEvent(getName(CLIPBOARD_UPDATE)); // TODO: RAI-61
		IDs[DROP_FILE] = mediator.registerEvent<DropFileEventData>(getName(DROP_FILE));
		IDs[DROP_TEXT] = mediator.registerEvent<DropTextEventData>(getName(DROP_TEXT));
		IDs[DROP_BEGIN] = mediator.registerEvent<DropBeginEventData>(getName(DROP_BEGIN));
		IDs[DROP_COMPLETE] = mediator.registerEvent<DropCompleteEventData>(getName(DROP_COMPLETE));
		IDs[AUDIO_DEVICE_ADDED] = mediator.registerEvent<AudioDeviceAddedEventData>(getName(AUDIO_DEVICE_ADDED));
		IDs[AUDIO_DEVICE_REMOVED] = mediator.registerEvent<AudioDeviceRemovedEventData>(getName(AUDIO_DEVICE_REMOVED));
		IDs[SENSOR_UPDATE] = mediator.registerEvent<SensorUpdateEventData>(getName(SENSOR_UPDATE));
	}

	EventID getID(Event event){
		PROFILE_FUNCTION();
		return IDs[event];
	}

	const char* getName(Event event){
		PROFILE_FUNCTION();
		switch (event){
			case QUIT: return "Quit";
			case APP_TERMINATING: return "App Terminating";
			case APP_LOW_MEMORY: return "App Low Memory";
			case APP_WILL_ENTER_BACKGROUND: return "App Will Enter Background";
			case APP_DID_ENTER_BACKGROUND: return "App Did Enter Background";
			case APP_WILL_ENTER_FORGROUND: return "App Will Enter Forground";
			case APP_DID_ENTER_FORGROUND: return "App Did Enter Forground";
			case LOCAL_CHANGED: return "Local Changed";
			case DISPLAY_ORIENTATION: return "Display Orientation";
			case DISPLAY_CONNECTED: return "Display Connected";
			case DISPLAY_DISCONNECTED: return "Display Disconnected";
			case WINDOW_SHOWN: return "Window Shown";
			case WINDOW_HIDDEN: return "Window Hidden";
			case WINDOW_EXPOSED: return "Window Exposed";
			case WINDOW_RESIZED: return "Window Resized";
			case WINDOW_SIZE_CHANGED: return "Window Size Changed";
			case WINDOW_MINIMIZED: return "Window Minimized";
			case WINDOW_MAXIMIZED: return "Window Maximized";
			case WINDOW_RESTORED: return "Window Restored";
			case WINDOW_ENTER: return "Window Enter";
			case WINDOW_LEAVE: return "Window Leave";
			case WINDOW_FOCUS_GAINED: return "Window Focus Gained";
			case WINDOW_FOCUS_LOST: return "Window Focus Lost";
			case WINDOW_CLOSE: return "Window Close";
			case WINDOW_TAKE_FOCUS: return "Window Take Focus";
			case WINDOW_DISPLAY_CHANGED: return "Window Display Changed";
			case KEY_DOWN: return "Key Down";
			case KEY_UP: return "Key Up";
			case TEXT_EDITING: return "Text Editing";
			case TEXT_INPUT: return "Text Input";
			case KEY_MAP_CHANGED: return "Key Map Changed";
			case MOUSE_MOTION: return "Mouse Motion";
			case MOUSE_BUTTON_DOWN: return "Mouse Button Down";
			case MOUSE_BUTTON_UP: return "Mouse Button Up";
			case MOUSE_WHEEL: return "Mouse Wheel";
			case JOYSTICK_AXIS_MOTION: return "Joystick Axis Motion";
			case JOYSTICK_BALL_MOTION: return "Joystick Ball Motion";
			case JOYSTICK_HAT_MOTION: return "Joystick Hat Motion";
			case JOYSTICK_BUTTON_DOWN: return "Joystick Button Down";
			case JOYSTICK_BUTTON_UP: return "Joystick Button Up";
			case JOYSTICK_DEVICE_ADDED: return "Joystick Device Added";
			case JOYSTICK_DEVICE_REMOVED: return "Joystick Device Removed";
			case CONTROLLER_AXIS_MOTION: return "Controller Axis Motion";
			case CONTROLLER_BUTTON_DOWN: return "Controller Button Down";
			case CONTROLLER_BUTTON_UP: return "Controller Button Up";
			case CONTROLLER_DEVICE_ADDED: return "Controller Device Added";
			case CONTROLLER_DEVICE_REMOVED: return "Controller Device Removed";
			case CONTROLLER_DEVICE_REMAPED: return "Controller Device Remaped";
			case CONTROLLER_TOUCHPAD_DOWN: return "Controller Touchpad Down";
			case CONTROLLER_TOUCHPAD_UP: return "Controller Touchpad Up";
			case CONTROLLER_TOUCHPAD_MOTION: return "Controller Touchpad Motion";
			case CONTROLLER_SENSOR_UPDATE: return "Controller Sensor Update";
			case FINGER_DOWN: return "Finger Down";
			case FINGER_UP: return "Finger Up";
			case FINGER_MOTION: return "Finger Motion";
			case DOLLAR_GERSTURE: return "Dollar Gesture";
			case DOLLAR_RECORD: return "Dollar Record";
			case MULTI_GESTURE: return "Multi Gesture";
			case CLIPBOARD_UPDATE: return "Clipboard Update";
			case DROP_FILE: return "Drop File";
			case DROP_TEXT: return "Drop Text";
			case DROP_BEGIN: return "Drop Begin";
			case DROP_COMPLETE: return "Drop Complete";
			case AUDIO_DEVICE_ADDED: return "Audio Device Added";
			case AUDIO_DEVICE_REMOVED: return "Audio Device Removed";
			case SENSOR_UPDATE: return "Sensor Update";
		}
		return "none";
	}
}