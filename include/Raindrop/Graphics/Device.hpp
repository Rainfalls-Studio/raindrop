#ifndef __RAINDROP_GRAPHICS_DEVICE_HPP__
#define __RAINDROP_GRAPHICS_DEVICE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Device{
		public:
			static constexpr std::array<const char*, 1> REQUIRED_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
			static constexpr std::array<const char*, 0> REQUIRED_LAYERS = {};

			Device(GraphicsContext& context);
			~Device();

			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

			VkDevice get() const;
			VkPhysicalDevice getPhysicalDevice() const;

			std::vector<VkQueueFamilyProperties> getQueueFamilyProperties();
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

			SwapchainSupport getSwapchainSupport(VkSurfaceKHR surface) const;

			void waitIdle();

		private:
			GraphicsContext& _context;

			VkPhysicalDevice _physicalDevice;
			VkDevice _device;

			VkQueue _graphicsQueue = VK_NULL_HANDLE;
			VkQueue _presentQueue = VK_NULL_HANDLE;
			VkQueue _transfertQueue = VK_NULL_HANDLE;

			void findPhysicalDevice();
			bool isPhysicalDeviceSuitable(VkPhysicalDevice device);
			void build();

			bool isExtensionsSupported();
			bool isLayersSupported();

			bool isExtensionSupported(const char* extensionName);
			bool isLayerSupported(const char* layerName);

			uint32_t getGraphicsFamily();
			uint32_t getTransfertFamily();
			uint32_t getPresentFamily();

	};
}

#endif