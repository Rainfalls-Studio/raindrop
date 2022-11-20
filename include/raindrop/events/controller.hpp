#ifndef __RAINDROP_EVENTS_CONTROLLER_HPP__
#define __RAINDRIP_EVENTS_CONTROLLER_HPP__

namespace rnd::events{
	enum class ControllerAxis{
		LEFTX = 0,
		LEFTY = 1,
		RIGHTX = 2,
		RIGHTY = 3,
		TRIGGERLEFT = 4,
		TRIGGERRIGHT = 5,
	};

	enum class ControllerButton{
		INVALID = -1,
		A,
		B,
		X,
		Y,
		BACK,
		GUIDE,
		START,
		LEFTSTICK,
		RIGHTSTICK,
		LEFTSHOULDER,
		RIGHTSHOULDER,
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
		MISC1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
		PADDLE1,  /* Xbox Elite paddle P1 */
		PADDLE2,  /* Xbox Elite paddle P3 */
		PADDLE3,  /* Xbox Elite paddle P2 */
		PADDLE4,  /* Xbox Elite paddle P4 */
		TOUCHPAD, /* PS4/PS5 touchpad button */
	};

	enum class SensorType{
		ACCEL,
		GYRO
	};
}

#endif