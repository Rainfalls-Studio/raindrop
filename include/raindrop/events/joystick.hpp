/**
 * @brief this file contain the joystick enums
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

#ifndef __RAINDROP_EVENTS_JOYSTICKS_HPP__
#define __RAINDROP_EVENTS_JOYSTICKS_HPP__

namespace rnd::events{
	enum class Joystick{
		JOYSTICK_TYPE_GAMECONTROLLER,
		JOYSTICK_TYPE_WHEEL,
		JOYSTICK_TYPE_ARCADE_STICK,
		JOYSTICK_TYPE_FLIGHT_STICK,
		JOYSTICK_TYPE_DANCE_PAD,
		JOYSTICK_TYPE_GUITAR,
		JOYSTICK_TYPE_DRUM_KIT,
		JOYSTICK_TYPE_ARCADE_PAD,
		JOYSTICK_TYPE_THROTTLE
	};

	enum class HatPosition{
		Center = 0,
		North = 1 << 0,
		East = 1 << 1,
		South = 1 << 2,
		West = 1 << 3,
		SouthEast = South | East,
		SouthWest = South | West,
		NorthEast = North | East,
		NorthWest = North | West,
	};
}

#endif