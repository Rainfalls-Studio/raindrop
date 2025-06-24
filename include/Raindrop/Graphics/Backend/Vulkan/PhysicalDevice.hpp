#pragma once

#include <VkBootstrap.h>

namespace Raindrop::Graphics{
	class Context;

	namespace Backend::Vulkan{
		class PhysicalDevice{
			public:
				PhysicalDevice(Context& context, vkb::PhysicalDevice device) noexcept;
				~PhysicalDevice();

				PhysicalDevice(const PhysicalDevice&) = delete;
				PhysicalDevice& operator=(const PhysicalDevice&) = delete;
				
				vkb::PhysicalDevice* operator->() noexcept;
				vkb::PhysicalDevice& operator*() noexcept;

				const VkPhysicalDevice& get() const noexcept;
				const vkb::PhysicalDevice& getVkb() const noexcept;

				std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

			private:
				Context* _context;
				vkb::PhysicalDevice _device;
		};
	}
}