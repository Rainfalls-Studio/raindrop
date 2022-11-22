/**
 * @brief this file contain the SDL2 window class.
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

#ifndef __RAINDROP_WINDOW_SDL2_WINDOW_HPP__
#define __RAINDROP_WINDOW_SDL2_WINDOW_HPP__

#include "core.hpp"
#include "Window.hpp"
#include <SDL2/SDL.h>

namespace rnd::window{

	class SDL2_Window : public Window{
		public:
			SDL2_Window() : Window(){}
			~SDL2_Window() = default;

			virtual void init(Extent extent, const char* title, uint32_t flags) override;
			virtual void shutdown() override;

			virtual void fullscreen(bool state) override;
			virtual void resizable(bool state) override;
			virtual void minimize() override;
			virtual void maximize() override;
			virtual void focus() override;
			virtual void show() override;
			virtual void hide() override;

			virtual glm::i32vec2 getPosition() const override;
			virtual const char* getTitle() const override;
			virtual Extent getExtent() const override;
			virtual Extent getMaxExtent() const override;
			virtual Extent getMinExtent() const override;
			virtual float getOpacity() const override;
			virtual bool isFullscreen() const override;
			virtual bool isBorderless() const override;
			virtual bool isHidden() const override;
			virtual bool isResizable() const override;
			virtual bool isHighDPIAllowed() const override;
			virtual bool isAlwaysOnTop() const override;
			virtual bool isTaskBarHidden() const override;
			virtual bool isTooltip() const override;
			virtual bool isPopup() const override;
			virtual bool isUtility() const override;

			virtual API getAPI() const override;
			SDL_Window* getWindow() const;

			static void event(events::Mediator& mediator);

		private:
			SDL_Window* handle = nullptr;
	};
}

#endif