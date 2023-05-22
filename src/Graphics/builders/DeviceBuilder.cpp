#include <Raindrop/Graphics/builders/DeviceBuilder.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>

namespace Raindrop::Graphics::Builders{
	DeviceBuilder::DeviceBuilder(){

	}

	DeviceBuilder::~DeviceBuilder(){
		VkDeviceCreateInfo info;
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	}

	DeviceBuilder& DeviceBuilder::setPhysicalDevice(std::shared_ptr<PhysicalDevice> physicalDevice){
		_physicalDevice = physicalDevice;
		return *this;
	}
	
	DeviceBuilder& DeviceBuilder::requireExtension(const char* extension){
		_requiredExtensions.push_back(extension);
		return *this;
	}

	DeviceBuilder& DeviceBuilder::requireLayer(const char* layer){
		_requiredLayers.push_back(layer);
		return *this;
	}

	std::shared_ptr<Device> DeviceBuilder::build(VkAllocationCallbacks* callbacks){
		if (!isExtensionsSupported()){
			throw std::runtime_error("unsupported extensions");
		}

		if (!isLayersSupported()){
			throw std::runtime_error("unsupported layers");
		}

		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		
		VkPhysicalDeviceFeatures features = _physicalDevice->features();
		info.pEnabledFeatures = &features;

		info.ppEnabledExtensionNames = _requiredExtensions.data();
		info.ppEnabledLayerNames = _requiredLayers.data();

		info.enabledExtensionCount = static_cast<uint32_t>(_requiredExtensions.size());
		info.enabledLayerCount = static_cast<uint32_t>(_requiredLayers.size());

		const auto& familyProperties = _physicalDevice->queueFamilyProperties();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(familyProperties.size());
		std::vector<std::vector<float>> priorities(familyProperties.size());

		for (uint32_t i=0; i<static_cast<uint32_t>(queueCreateInfos.size()); i++){
			auto& info = queueCreateInfos[i];
			auto& priority = priorities[i];
			const auto& properties = familyProperties[i];

			priority.resize(properties.queueCount);
			std::fill(priority.begin(), priority.end(), 0.f);

			info = {};
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.queueFamilyIndex = i;
			info.queueCount = properties.queueCount;
			info.pQueuePriorities = priority.data();
		}

		info.pQueueCreateInfos = queueCreateInfos.data();
		info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

		return std::make_shared<Device>(info, _physicalDevice, callbacks);
	}
	
	bool DeviceBuilder::isExtensionsSupported() const{
		for (const auto &e : _requiredExtensions){
			if (!_physicalDevice->supportExtension(e)) return false;
		}
		return true;
	}

	bool DeviceBuilder::isLayersSupported() const{
		for (const auto &l : _requiredLayers){
			if (!_physicalDevice->supportLayer(l)) return false;
		}
		return true;
	}
}