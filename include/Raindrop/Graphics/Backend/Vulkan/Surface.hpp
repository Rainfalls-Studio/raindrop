#pragma once

#include "Raindrop/pch.pch"
#include "Instance.hpp"

namespace Raindrop::Graphics{
	class Context;

	namespace Backend::Vulkan{
		class Surface{
			public:
				Surface(Context& instance, SDL_Window* window) noexcept;
				~Surface();

				const VkSurfaceKHR& get() const noexcept; 

			private:
				Context* _context;
				VkSurfaceKHR _surface;

				void _create(SDL_Window* window);

		};
	}
}