#pragma once

#include <vulkan/vulkan.h>
#include "../LogicalDevice.hpp"

#include <vector>

namespace rnd::render::vulkan{
	class DescriptorSetBuilder{
		friend class DescriptorSet;
		public:
			struct Descriptor{
				friend class DescriptorSet;

				uint32_t binding = 0;
				VkDescriptorType type = VK_DESCRIPTOR_TYPE_MAX_ENUM;
				VkPipelineStageFlags stage;
				std::vector<VkDescriptorImageInfo> imageInfos{};
				uint32_t imageCount = 0;
				uint32_t bufferSize = 0;

				private:
					VkDescriptorBufferInfo bufferInfo;
			};

			void setDescriptors(const std::vector<Descriptor> &descriptors);
			void setDescriptorSetCount(uint32_t count);
			void setLogicalDevice(LogicalDevice* device);

		private:
			LogicalDevice* device = nullptr;
			std::vector<Descriptor> descriptors;
			uint32_t descriptorSetCount = 1;
	};
}