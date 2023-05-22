#include <Raindrop/Graphics/PhysicalDevice.hpp>

namespace Raindrop::Graphics{
	PhysicalDevice::PhysicalDevice(VkPhysicalDevice device) : _device{device}{
		vkGetPhysicalDeviceFeatures(device, &_features);
		vkGetPhysicalDeviceProperties(device, &_properties);
		vkGetPhysicalDeviceMemoryProperties(device, &_memoryProperties);

		getToolProperties();
		getFamilyProperties();
		getExtensionProperties();
		getLayerProperties();
	}

	void PhysicalDevice::getToolProperties(){
		uint32_t count=0;
		vkGetPhysicalDeviceToolProperties(_device, &count, nullptr);
		_toolProperties.resize(count);
		vkGetPhysicalDeviceToolProperties(_device, &count, _toolProperties.data());
	}

	void PhysicalDevice::getFamilyProperties(){
		uint32_t count=0;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, nullptr);
		_queueFamilyProperties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, _queueFamilyProperties.data());
	}

	void PhysicalDevice::getExtensionProperties(){
		uint32_t count=0;
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, nullptr);
		_supportedExtensionProperties.resize(count);
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, _supportedExtensionProperties.data());
	}

	void PhysicalDevice::getLayerProperties(){
		uint32_t count=0;
		vkEnumerateDeviceLayerProperties(_device, &count, nullptr);
		_supportedLayerProperties.resize(count);
		vkEnumerateDeviceLayerProperties(_device, &count, _supportedLayerProperties.data());
	}

	PhysicalDevice::~PhysicalDevice(){}

	VkPhysicalDevice PhysicalDevice::get() const{
		return _device;
	}

	const VkPhysicalDeviceFeatures& PhysicalDevice::features() const{
		return _features;
	}

	const VkPhysicalDeviceProperties& PhysicalDevice::properties() const{
		return _properties;
	}

	const VkPhysicalDeviceMemoryProperties& PhysicalDevice::memoryProperties() const{
		return _memoryProperties;
	}

	const std::vector<VkPhysicalDeviceToolProperties>& PhysicalDevice::toolProperties() const{
		return _toolProperties;
	}

	const std::vector<VkQueueFamilyProperties>& PhysicalDevice::queueFamilyProperties() const{
		return _queueFamilyProperties;
	}

	const std::vector<VkExtensionProperties> PhysicalDevice::getSupportedExtensionProperties() const{
		return _supportedExtensionProperties;
	}

	const std::vector<VkLayerProperties> PhysicalDevice::getSupportedLayerProperties() const{
		return _supportedLayerProperties;
	}

	bool PhysicalDevice::supportFeatures(const VkPhysicalDeviceFeatures& features) const{
		static constexpr uint32_t featuresCount = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);

		const VkBool32* supportedFeatures = reinterpret_cast<const VkBool32*>(&_features);
		const VkBool32* requiredFeatures = reinterpret_cast<const VkBool32*>(&features);

		for (uint32_t i=0; i<featuresCount; i++){
			if (requiredFeatures[i] == VK_TRUE){
				if (supportedFeatures[i] == VK_FALSE)
					return false;
			}
		}

		return true;
	}
	
	bool PhysicalDevice::supportExtension(const char* extension) const{
		for (auto &e : _supportedExtensionProperties){
			if (strcmp(e.extensionName, extension) == 0){
				return true;
			}
		}
		return false;
	}

	bool PhysicalDevice::supportLayer(const char* layer) const{
		for (auto &e : _supportedLayerProperties){
			if (strcmp(e.layerName, layer) == 0){
				return true;
			}
		}
		return false;
	}

	PhysicalDevice::SwapchainSupport PhysicalDevice::getSwapchainSupport(VkSurfaceKHR surface) const{
		SwapchainSupport support;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, surface, &support.capabilities);

		uint32_t count;

		vkGetPhysicalDeviceSurfaceFormatsKHR(_device, surface, &count, nullptr);
		if (count){
			support.formats.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_device, surface, &count, support.formats.data());
		}

		vkGetPhysicalDeviceSurfacePresentModesKHR(_device, surface, &count, nullptr);
		if (count){
			support.presentModes.resize(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_device, surface, &count, support.presentModes.data());
		}

		support.supported = !support.formats.empty() && !support.presentModes.empty();
		return support;
	}
}