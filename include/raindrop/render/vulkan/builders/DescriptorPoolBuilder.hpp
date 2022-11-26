#pragma once

#include <vulkan/vulkan.h>
#include "../LogicalDevice.hpp"

#include <vector>

namespace rnd::render::vulkan{
	class DescriptorPoolBuilder{
		friend class DescriptorPool;
		public:
			void setMaxSets(uint32_t count);
			void setPoolFlags(VkDescriptorPoolCreateFlags flags);
			void addPoolSize(VkDescriptorType type, uint32_t count);
			void setDevice(LogicalDevice* device);

		private:
			LogicalDevice* device = nullptr;
			std::vector<VkDescriptorPoolSize> poolSizes = {};
			uint32_t maxSets = 1;
			VkDescriptorPoolCreateFlags poolFlags = 0;
			
	};
}