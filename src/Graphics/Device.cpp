#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/Window.hpp>

namespace Raindrop::Graphics{
	Device::Device(GraphicsContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Device");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		findPhysicalDevice();
		build();
	}

	Device::~Device(){
		vkDestroyDevice(_device, _context.allocationCallbacks);
	}

	void Device::findPhysicalDevice(){
		uint32_t count = 0;
		vkEnumeratePhysicalDevices(_context.instance.get(), &count, nullptr);

		if (count == 0){
			CLOG(ERROR, "Engine.Graphics.Device") << "Cannot find any physical device supporting vulkan";
			throw std::runtime_error("Failed to find physical device with Vulkan support");
		}

		std::vector<VkPhysicalDevice> physicalDevices(count);
		vkEnumeratePhysicalDevices(_context.instance.get(), &count, physicalDevices.data());

		for (auto &device : physicalDevices){
			if (isPhysicalDeviceSuitable(device)){
				_physicalDevice = device;
				return;
			}
		}

		CLOG(ERROR, "Engine.Graphics.Device") << "Cannot find a suitable physical device";
		throw std::runtime_error("Failed to find a suitable physical device");
	}

	bool Device::isPhysicalDeviceSuitable(VkPhysicalDevice device){
		uint32_t surfaceFormatCount = 0;
		uint32_t presentModeCount = 0;

		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _context.window.surface(), &surfaceFormatCount, nullptr);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _context.window.surface(), &presentModeCount, nullptr);

		return surfaceFormatCount && presentModeCount;
	}
	
	VkDevice Device::get() const{
		return _device;
	}

	VkPhysicalDevice Device::getPhysicalDevice() const{
		return _physicalDevice;
	}

	void Device::build(){
		if (!isExtensionsSupported()){
			throw std::runtime_error("unsupported extensions");
		}

		if (!isLayersSupported()){
			throw std::runtime_error("unsupported layers");
		}

		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(_physicalDevice, &features);
		info.pEnabledFeatures = &features;

		info.ppEnabledExtensionNames = REQUIRED_EXTENSIONS.data();
		info.ppEnabledLayerNames = REQUIRED_LAYERS.data();

		info.enabledExtensionCount = static_cast<uint32_t>(REQUIRED_EXTENSIONS.size());
		info.enabledLayerCount = static_cast<uint32_t>(REQUIRED_LAYERS.size());

		std::vector<VkQueueFamilyProperties> familyProperties = getQueueFamilyProperties();
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

		if (vkCreateDevice(_physicalDevice, &info, _context.allocationCallbacks, &_device) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Device") << "Failed to create vulkan device";
			throw std::runtime_error("failed to create vulkan device");
		}

		_context.graphicsFamily = getGraphicsFamily();
		_context.presentFamily = getPresentFamily();
		_context.transfertFamily = getTransfertFamily();

		vkGetDeviceQueue(_device, _context.graphicsFamily, 0, &_context.graphicsQueue);
		vkGetDeviceQueue(_device, _context.presentFamily, 0, &_context.presentQueue);

		if (_context.graphicsFamily == _context.transfertFamily || _context.presentFamily == _context.transfertFamily){
			if (familyProperties[_context.transfertFamily].queueCount > 1){
				vkGetDeviceQueue(_device, _context.transfertFamily, 1, &_context.transfertQueue);
			} else {
				vkGetDeviceQueue(_device, _context.transfertFamily, 0, &_context.transfertQueue);
			}
		} else {
			vkGetDeviceQueue(_device, _context.transfertFamily, 0, &_context.transfertQueue);
		}
	}

	bool Device::isExtensionsSupported(){
		for (const auto &extension : REQUIRED_EXTENSIONS){
			if (!isExtensionSupported(extension)) return false;
		}
		return true;
	}

	bool Device::isLayersSupported(){
		for (const auto &layer : REQUIRED_LAYERS){
			if (!isLayerSupported(layer)) return false;
		}
		return true;
	}

	bool Device::isExtensionSupported(const char* extensionName){
		uint32_t count=0;
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, nullptr);

		if (count == 0) return false;
		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, extensions.data());
		
		for (auto &e : extensions){
			if (strcmp(e.extensionName, extensionName) == 0) return true;
		}
		return false;
	}

	bool Device::isLayerSupported(const char* layerName){
		uint32_t count=0;
		vkEnumerateDeviceLayerProperties(_physicalDevice, &count, nullptr);

		if (count == 0) return false;
		std::vector<VkLayerProperties> layers(count);
		vkEnumerateDeviceLayerProperties(_physicalDevice, &count, layers.data());
		
		for (auto &l : layers){
			if (strcmp(l.layerName, layerName) == 0) return true;
		}
		return false;
	}

	std::vector<VkQueueFamilyProperties> Device::getQueueFamilyProperties(){
		uint32_t queueCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueCount, nullptr);
		std::vector<VkQueueFamilyProperties> properties(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueCount, properties.data());
		return properties;
	}

	uint32_t Device::getGraphicsFamily(){
		auto properties = getQueueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
				return i;
			}
		}
		throw std::runtime_error("failed to find a graphics family");
	}

	uint32_t Device::getTransfertFamily(){
		auto properties = getQueueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
				return i;
			}
		}
		throw std::runtime_error("failed to find a graphics family");
	}

	uint32_t Device::getPresentFamily(){
		auto properties = getQueueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			VkBool32 supported;
			vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, i, _context.window.surface(), &supported);
			return i;
		}
		throw std::runtime_error("failed to find a present family");
	}

	uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw "failed to find suitable memory type";
	}

	SwapchainSupport Device::getSwapchainSupport(VkSurfaceKHR surface) const{
		SwapchainSupport support;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, surface, &support.capabilities);

		uint32_t count;

		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &count, nullptr);
		if (count){
			support.formats.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &count, support.formats.data());
		}

		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &count, nullptr);
		if (count){
			support.presentModes.resize(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &count, support.presentModes.data());
		}

		support.supported = !support.formats.empty() && !support.presentModes.empty();
		return support;
	}

	void Device::waitIdle(){
		vkDeviceWaitIdle(_device);
	}
}