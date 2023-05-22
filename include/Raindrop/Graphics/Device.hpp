#ifndef __RAINDROP_GRAPHICS_DEVICE_HPP__
#define __RAINDROP_GRAPHICS_DEVICE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Device{
		public:
			struct QueueFamily{
				std::vector<VkQueue> queues;
			};


			Device(const VkDeviceCreateInfo& info, std::shared_ptr<PhysicalDevice> physicalDevice, VkAllocationCallbacks* callbacks = nullptr);
			~Device();

			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

			VkDevice get() const;
			std::shared_ptr<PhysicalDevice> physicalDevice() const;
			const std::vector<QueueFamily>& families() const;
			void waitIdle() const;

			uint32_t graphicsFamily();
			uint32_t presentFamily(VkSurfaceKHR surface);

		private:
			std::shared_ptr<PhysicalDevice> _physicalDevice;
			VkAllocationCallbacks* _callbacks;
			VkDevice _device;

			std::vector<QueueFamily> _families;
	};
}

#endif