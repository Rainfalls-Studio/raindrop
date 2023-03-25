#ifndef __VULKANMODULE_PHYSICAL_DEVICE_HPP__
#define __VULKANMODULE_PHYSICAL_DEVICE_HPP__

#include "core.hpp"
#include "Instance.hpp"
#include "Allocator.hpp"

class PhysicalDevice{
	public:
		PhysicalDevice(GfxAllocator& allocator, Instance& instance, VkPhysicalDevice physicalDevice);
		~PhysicalDevice();

		bool loaded() const;

		bool isFamilySuported(VkQueueFlagBits family);
		bool isExtensionSupported(const char* extension, uint32 minimumVersion = -1);
		bool isFeaturesSupported(VkPhysicalDeviceFeatures features);
		const VkQueueFamilyProperties& getFamilyProperties(uint32 familyIndex) const;
		const Array<VkQueueFamilyProperties>& getFamiliesProperties() const;
		uint32 findFamily(VkQueueFlagBits families, bool presentSupported = false, VkSurfaceKHR surface = VK_NULL_HANDLE);

		const VkPhysicalDeviceProperties& getProperties() const;
		const VkPhysicalDeviceFeatures& getFeatures() const;
		const VkPhysicalDeviceMemoryProperties& getMemoryProperties() const;

		Array<VkPresentModeKHR> getSurfacePresentModes(Allocator& allocator, VkSurfaceKHR surface) const;
		Array<VkSurfaceFormatKHR> getSurfaceFormat(Allocator& allocator, VkSurfaceKHR surface) const;
		VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkSurfaceKHR surface) const;
		bool isPresentSupported(uint32 queueIndex, VkSurfaceKHR surface) const;

		bool isFormatSupported(VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat getFirstSupportedFormat(Array<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		VkPhysicalDevice get() const;
		Instance& instance();

		static void getPhysicalDevices(Array<PhysicalDevice*>& devices, GfxAllocator& allocator, Instance& instance);

	private:
		bool _loaded;
		Instance& _instance;
		GfxAllocator& _allocator;
		VkPhysicalDevice _physicalDevice;
		VkPhysicalDeviceProperties _properties;
		VkPhysicalDeviceFeatures _features;
		VkPhysicalDeviceMemoryProperties _memoryProperties;
		Array<VkExtensionProperties> _extensions;
		Array<VkQueueFamilyProperties> _familiesProperties;

		bool queryProperties();
		bool queryFeatures();
		bool queryExtensions();
		bool queryFamilies();
};

#endif