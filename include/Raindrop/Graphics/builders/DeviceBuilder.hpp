#ifndef __RAINDROP_BUILDERS_DEVICE_BUILDER_HPP__
#define __RAINDROP_BUILDERS_DEVICE_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Builders{
	class DeviceBuilder{
		public:
			DeviceBuilder();
			~DeviceBuilder();

			DeviceBuilder& setPhysicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice);
			DeviceBuilder& requireExtension(const char* extension);
			DeviceBuilder& requireLayer(const char* layer);

			std::shared_ptr<Device> build(VkAllocationCallbacks* callbacks = nullptr);

		private:
			std::shared_ptr<PhysicalDevice> _physicalDevice;
			std::vector<const char*> _requiredExtensions;
			std::vector<const char*> _requiredLayers;

			bool isExtensionsSupported() const;
			bool isLayersSupported() const;
	};
}

#endif