#pragma once

#include <vulkan/vulkan.h>
#include "../LogicalDevice.hpp"

#include <unordered_map>

namespace rnd::render::vulkan{
	class DescriptorSetLayoutBuilder{
		friend class DescriptorSetLayout;
		public:
			void addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags shaderStages, uint32_t count = 1);
			void setDevice(LogicalDevice* device);

		private:
			LogicalDevice* device = nullptr;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};

	};
}