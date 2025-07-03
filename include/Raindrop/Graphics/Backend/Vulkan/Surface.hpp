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
			const VkSurfaceCapabilitiesKHR& getCapabilities() const noexcept;
			const std::vector<VkSurfaceFormatKHR>& getFormats() const noexcept;
			const std::vector<VkPresentModeKHR>& getCresentModes() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;

		private:
			Context& _context;
			VkSurfaceKHR _surface;

			VkSurfaceCapabilitiesKHR _capabilities;
			std::vector<VkSurfaceFormatKHR> _formats;
			std::vector<VkPresentModeKHR> _presentModes;
	};
}