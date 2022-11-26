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
			~Window() = default;

			void init(Extent extent, const char* title, uint32_t flags);
			void shutdown();

			void fullscreen(bool state);
			void resizable(bool state);
			void minimize();
			void maximize();
			void focus();
			void show();
			void hide();

			glm::i32vec2 getPosition() const;
			const char* getTitle() const;
			Extent getExtent() const;
			Extent getMaxExtent() const;
			Extent getMinExtent() const;
			float getOpacity() const;
			bool isFullscreen() const;
			bool isBorderless() const;
			bool isHidden() const;
			bool isResizable() const;
			bool isHighDPIAllowed() const;
			bool isAlwaysOnTop() const;
			bool isTaskBarHidden() const;
			bool isTooltip() const;
			bool isPopup() const;
			bool isUtility() const;

			static void event(events::Mediator& mediator);

			static const char* getAPIName();
			static Version getAPIVersion();
			void* getWindowHandle() const;
			
		private:
			void* handle;
	};
}

#endif