#ifndef __RAINDROP_GRAPHICS_PHYSICAL_DEVICE_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_PHYSICAL_DEVICE_MANAGER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class PhysicalDeviceManager{
		public:
			PhysicalDeviceManager(std::shared_ptr<Instance> instance, VkAllocationCallbacks* callbacks = nullptr);
			~PhysicalDeviceManager();

			std::shared_ptr<Instance> instance() const;
			const std::list<std::shared_ptr<PhysicalDevice>>& devices() const;

		private:
			VkAllocationCallbacks* callbacks;
			std::shared_ptr<Instance> _instance;
			std::list<std::shared_ptr<PhysicalDevice>> _devices;

			void createPhysicalDevices();
	};
}

#endif