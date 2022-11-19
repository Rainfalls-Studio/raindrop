/**
 * @brief this file contain the window class. critic part of rendering.
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

#ifndef __RAINDROP_WINDOW_WINDOW_HPP__
#define __RAINDROP_WINDOW_WINDOW_HPP__

#include "core.hpp"
#include "events/EventMediator.hpp"

namespace rnd::window{

	enum class API{
		SDL2 = 1 << 0,
		GLWF = 1 << 1,

		Default = SDL2,
	};

	enum Flag{
		None = 0,
		Fullscreen = 1 << 0,
		Borderless = 1 << 1,
		Hidden = 1 << 2,
		Resizable = 1 << 3,
		AllowHighDPI = 1 << 4,
		AlwawsOnTop = 1 << 5,
		HideTaskbar = 1 << 6,
		Tooltip = 1 << 7,
		Popup = 1 << 8,
		Utility = 1 << 9,
	};
	
	class Window{
		public:
			virtual ~Window() = default;

			static Window* create(API api);

			virtual void init(Extent extent, const char* title, uint32_t flags) = 0;
			virtual void shutdown() = 0;

			virtual void fullscreen(bool state) = 0;
			virtual void resizable(bool state) = 0;
			virtual void minimize() = 0;
			virtual void maximize() = 0;
			virtual void focus() = 0;
			virtual void show() = 0;
			virtual void hide() = 0;

			virtual glm::i32vec2 getPosition() const = 0;
			virtual const char* getTitle() const = 0;
			virtual Extent getExtent() const = 0;
			virtual Extent getMaxExtent() const = 0;
			virtual Extent getMinExtent() const = 0;
			virtual float getOpacity() const = 0;
			virtual bool isFullscreen() const = 0;
			virtual bool isBorderless() const = 0;
			virtual bool isHidden() const = 0;
			virtual bool isResizable() const = 0;
			virtual bool isHighDPIAllowed() const = 0;
			virtual bool isAlwaysOnTop() const = 0;
			virtual bool isTaskBarHidden() const = 0;
			virtual bool isTooltip() const = 0;
			virtual bool isPopup() const = 0;
			virtual bool isUtility() const = 0;

			static void event(events::Mediator& mediator);

		private:

	};
}

#endif