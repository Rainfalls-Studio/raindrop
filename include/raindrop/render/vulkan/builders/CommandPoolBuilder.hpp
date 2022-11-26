#pragma once

#include "PhysicalDeviceBuilder.hpp"
#include <vulkan/vulkan.h>
#include "../enum.hpp"
#include "../LogicalDevice.hpp"

namespace rnd::render::vulkan{
	class CommandPoolBuilder{
		friend class CommandPool;
		public:
			void setFamily(QueueFamily family);
			void setFlag(VkCommandPoolCreateFlags flags);
			void setLogicalDevice(LogicalDevice* logicalDevice);

		private:
			QueueFamily family = QueueFamily::FAMILY_NONE; // used as undefined
			LogicalDevice* device = nullptr;
			VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	};
}