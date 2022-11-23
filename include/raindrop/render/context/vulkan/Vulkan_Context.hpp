/**
 * @brief this file contain the vulkan Context class, derived of the Context class
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

#ifndef __RAINDROP_RENDER_CONTEXT_VULKAN_VULKAN_CONTEXT_HPP__
#define __RAINDROP_RENDER_CONTEXT_VULKAN_VULKAN_CONTEXT_HPP__

#include "../../Context.hpp"
#include "Instance.hpp"
#include "AllocationCallback.hpp"

namespace rnd::render::context::vulkan{
	class Vulkan_Context : public Context{
		public:
			virtual void init(window::Window* window) override;

			// instance dependence
			DynamicArray<const char*> getWindowExtensions();
			void createSurface(VkInstance& instance, VkSurfaceKHR& surface);
		
			// classes
			AllocationCallback allocCallback;
			Instance instance;

			bool validationlayers = true;
			
		private:
			DynamicArray<const char*> getSDL2WindowExtensions();
			DynamicArray<const char*> getGLFWWindowExtensions();

			void createSDL2Surface(VkInstance& instance, VkSurfaceKHR& surface);
			void createGLFWSurface(VkInstance& instance, VkSurfaceKHR& surface);

			window::Window* window;
	};
}

#endif