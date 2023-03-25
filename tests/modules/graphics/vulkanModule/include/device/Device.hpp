#ifndef __VULKANMODULE_DEVICE_HPP__
#define __VULKANMODULE_DEVICE_HPP__

#include "core.hpp"
#include "device/Instance.hpp"
#include "device/PhysicalDevice.hpp"
#include "Allocator.hpp"

class Device{
	public:
		Device(GfxAllocator& allocator);
		~Device();

		void init(PhysicalDevice& physicalDevice);
		bool loaded() const;

		void requiredExtensions(const char* extension);
		void requireLayer(const char* layer);

		PhysicalDevice& physicalDevice();
		Instance& instance();
		VkDevice get() const;

		const Array<VkQueue>& queues() const;
		VkQueue getQueue(uint32 index) const;

		const Array<const char*> requiredExtensions() const;
		const Array<const char*> requiredLayers() const;

		bool createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image,VkDeviceMemory &imageMemory);

		VkCommandPool getCommandPool(VkCommandPoolCreateFlags flags, uint32 familyIndex);

	private:
		GfxAllocator& _allocator;
		PhysicalDevice* _physicalDevice;
		VkDevice _device;
		bool _loaded;

		Array<const char*> _requiredExtensions;
		Array<const char*> _requiredLayers;
		Array<VkQueue> _queues;

		struct CommandPool{
			VkCommandPool _pool;
			VkCommandPoolCreateFlags _flags;
			uint32 _familyIndex;
		};

		List<CommandPool> _commandPools;

		void pouplateQueueCreateInfo(Array<VkDeviceQueueCreateInfo>& queueCreateInfo);
};

#endif