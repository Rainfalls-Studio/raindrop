#pragma once

#include <vector>
#include <array>
#include <bitset>

#include "PhysicalDeviceBuilder.hpp"
#include "../PhysicalDevice.hpp"

namespace rnd::render::vulkan{
	class LogicalDeviceBuilder{
		friend class LogicalDevice;
		public:
			LogicalDeviceBuilder();

			void requireExtension(const char *extension);
			void requireQueue(QueueFamily family, uint32_t count);
			void setQueuePriority(QueueFamily family, uint32_t index, float priority);
			void setPhysicalDevice(PhysicalDevice *physicalDevice);
			void setCommandPoolFlags(VkCommandPoolCreateFlags flags, QueueFamily family);

		private:
			uint32_t requiredQueuesCount[FAMILY_COUNT];
			std::vector<float> queuePriorities[FAMILY_COUNT];
			VkCommandPoolCreateFlags commandPoolFlags[FAMILY_COUNT];

			std::vector<const char*> requiredExtensions;
			PhysicalDevice* physicalDevice = nullptr;
	};
}