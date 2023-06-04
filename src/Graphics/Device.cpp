#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>

namespace Raindrop::Graphics{
	Device::Device(const VkDeviceCreateInfo& info, std::shared_ptr<PhysicalDevice> physicalDevice, VkAllocationCallbacks* callbacks) : _physicalDevice{physicalDevice}, _callbacks{callbacks}{

		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.LogicalDevice");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.LogicalDevice") << "Creating a new vulkan logical device...";

		if (vkCreateDevice(physicalDevice->get(), &info, callbacks, &_device) != VK_SUCCESS){
			throw std::runtime_error("Failed to create vulkan logical device");
		}

		_families.resize(info.queueCreateInfoCount);
		for (uint32_t i=0; i<info.queueCreateInfoCount; i++){
			const auto& queueInfo = info.pQueueCreateInfos[i];
			auto& family = _families[i];

			family.queues.resize(queueInfo.queueCount);
			for (uint32_t j=0; j<queueInfo.queueCount; j++){
				auto& queue = family.queues[j];
				vkGetDeviceQueue(_device, i, j, &queue);
			}
		}

		CLOG(INFO, "Engine.Graphics.LogicalDevice") << "Vulkan logical device created with success !";

	}

	Device::~Device(){
		CLOG(INFO, "Engine.Graphics.LogicalDevice") << "Destroying vulkan logical device...";
		if (_device){
			vkDeviceWaitIdle(_device);
			vkDestroyDevice(_device, _callbacks);
		}
		CLOG(INFO, "Engine.Graphics.LogicalDevice") << "Vulkan logical device destroyed with success !";
	}

	VkDevice Device::get() const{
		return _device;
	}

	std::shared_ptr<PhysicalDevice> Device::physicalDevice() const{
		return _physicalDevice;
	}

	const std::vector<Device::QueueFamily>& Device::families() const{
		return _families;
	}
	
	uint32_t Device::graphicsFamily(){
		auto properties = _physicalDevice->queueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
				return i;
			}
		}
		throw std::runtime_error("failed to find a graphics family");
	}

	uint32_t Device::transfertFamily(){
		auto properties = _physicalDevice->queueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
				return i;
			}
		}
		throw std::runtime_error("failed to find a graphics family");
	}

	uint32_t Device::presentFamily(VkSurfaceKHR surface){
		auto properties = _physicalDevice->queueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			VkBool32 supported;
			vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice->get(), i, surface, &supported);
			return i;
		}
		throw std::runtime_error("failed to find a present family");
	}

	void Device::waitIdle() const{
		vkDeviceWaitIdle(_device);
	}
}