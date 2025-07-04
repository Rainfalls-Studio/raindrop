#pragma once

#include "../Surface.hpp"
#include "Context.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	struct Context;
	class Surface : public Backend::Surface{
		public:
			Surface(Context& context);
			virtual ~Surface() override;

			VkSurfaceKHR get() const noexcept;
			VkSurfaceCapabilitiesKHR getCapabilities() const noexcept;
			std::vector<VkSurfaceFormatKHR> getFormats() const noexcept;
			std::vector<VkPresentModeKHR> getPresentModes() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;

		private:
			Context& _context;
			VkSurfaceKHR _surface;
	};
}