#pragma once

#include <vulkan/vulkan.h>
#include "../Instance.hpp"
#include "../enum.hpp"

#include <bitset>
#include <vector>

namespace rnd::render::vulkan{

	class PhysicalDeviceBuidler{
		friend class PhysicalDevice;
		public:
			void requireExtension(const char* ext);
			void requireFamily(QueueFamily family);
			void requireFeature(PhysicalDeviceFeature feature);
			void setInstance(Instance* instance);
			
		private:
			VkPhysicalDeviceFeatures getFeatures();

			std::vector<std::string> requiredExtensions{};
			std::bitset<static_cast<size_t>(PhysicalDeviceFeature::FEATURES_COUNT)> requiredFeatures{};
			std::bitset<FAMILY_COUNT> requiredFamilies{};
			Instance* instance = nullptr;

	};
}