/**
 * @brief this file contain the vulkan physical device
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @authors @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

/**
 * MIT License
 * Copyright (c) 2022 Rainfalls studio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * 
 */

#ifndef __RAINDROP_RENDER_VULKAN_PHYSICAL_DEVICE_HPP__
#define __RAINDROP_RENDER_VULKAN_PHYSICAL_DEVICE_HPP__

#include "builders/PhysicalDeviceBuilder.hpp"
#include "Instance.hpp"
#include <vulkan/vulkan.h>

#include <array>

namespace rnd::render::vulkan{
	class PhysicalDevice{
		public:
			struct SwapChainSupport{
				VkSurfaceCapabilitiesKHR capabilities{};
				std::vector<VkSurfaceFormatKHR> formats{};
				std::vector<VkPresentModeKHR> presentModes{};
			};

			struct FamilyDetails{
				uint32_t family = 0;
				QueueFamily type = QueueFamily::FAMILY_NONE; // used as a non intialized value
				uint32_t queueCount = 0;
			};

			PhysicalDevice() = default;
			PhysicalDevice(PhysicalDeviceBuidler &builder){
				initialize(builder);
			}
			~PhysicalDevice();

			PhysicalDevice(const PhysicalDevice &) = delete;
			PhysicalDevice& operator=(const PhysicalDevice &) = delete;

			void initialize(PhysicalDeviceBuidler &builder);

			VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

			FamilyDetails getFamily(QueueFamily family) const;
			uint32_t getPresentFamily();

			std::vector<FamilyDetails> getFamilies() const {return families;}
			const std::bitset<FAMILY_COUNT>& getEnabledFamilies();
			VkPhysicalDeviceProperties& getProperties(); 
			VkPhysicalDeviceFeatures getEnabledFeatures();
			VkPhysicalDevice getDevice();
			SwapChainSupport getSwapChainSupport();
			Instance* getInstance();			

		private:
			void checkDeviceExtensionSupport(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			bool isSuitableDevice(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			std::array<uint32_t, FAMILY_COUNT> getFamilies(VkPhysicalDevice physicalDevice, PhysicalDeviceBuidler &builder);
			bool checkDeviceExtensions(VkPhysicalDevice device, PhysicalDeviceBuidler &builder);
			SwapChainSupport getSwapChainSupport(VkPhysicalDevice device);
			bool checkFeatures(VkPhysicalDeviceFeatures features, PhysicalDeviceBuidler &builder);

			std::vector<FamilyDetails> families;
			std::bitset<FAMILY_COUNT> enabledFamiles;
			uint32_t presentFamily = -1;

			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			VkPhysicalDeviceProperties properties{};
			VkPhysicalDeviceFeatures features{};
			SwapChainSupport swapChainSupport{};
			Instance* instance = nullptr;
	};
}

#endif