#include "render/vulkan/PhysicalDevice.hpp"
#include "core.hpp"

#include <vulkan/vk_enum_string_helper.h>

#include <stdexcept>
#include <cassert>
#include <set>

#define GET_FAMILY(familyType, vkFamily) { \
	if (builder.requiredFamilies[familyType] && queueFamily.queueFlags & vkFamily){ \
		families[familyType] = i; \
		availableFamilies.set(familyType); \
		this->families.push_back({i, QueueFamily::familyType, queueFamily.queueCount});\
	}}

namespace rnd::render::vulkan{
	void PhysicalDevice::init(PhysicalDeviceBuidler &builder){
		PROFILE_FUNCTION();
		assert(builder.instance != nullptr && "cannot create a physical device without a valid instance");
		instance = builder.instance;

		enabledFamiles = builder.requiredFamilies;

		uint32_t deviceCount = 0;
		{
			VkResult result = vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, nullptr);
			if (result != VK_SUCCESS){
				RND_RUNTIME_ERR("physical device error : vkEnumeratePhysicalDevices :: ", string_VkResult(result));
			}
		}

		if (deviceCount == 0){
			RND_RUNTIME_ERR("physical device : vkEnumeratePhysicalDevices");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance->getInstance(), &deviceCount, devices.data());

		for (auto &device : devices){
			if (isSuitableDevice(device, builder)){
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE){
			RND_RUNTIME_ERR("physical device : failed to found a suitable GPU");
		}
		
		features = builder.getFeatures();
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	}

	void PhysicalDevice::shutdown(){
		PROFILE_FUNCTION();
		if (instance == nullptr){
			instance = nullptr;
			physicalDevice = VK_NULL_HANDLE;
		}
		
	}

	void PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, PhysicalDeviceBuidler &builder){
		PROFILE_FUNCTION();
		
	}

	const std::bitset<FAMILY_COUNT>& PhysicalDevice::getEnabledFamilies(){
		PROFILE_FUNCTION();
		return enabledFamiles;
	}


	bool PhysicalDevice::isSuitableDevice(VkPhysicalDevice device, PhysicalDeviceBuidler &builder){
		PROFILE_FUNCTION();
		auto families = getFamilies(device, builder);
		if (!checkDeviceExtensions(device, builder)) return false;

		swapChainSupport = getSwapChainSupport(device);
		bool swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return swapChainAdequate && checkFeatures(supportedFeatures, builder);
	}

	std::array<uint32_t, FAMILY_COUNT> PhysicalDevice::getFamilies(VkPhysicalDevice physicalDevice, PhysicalDeviceBuidler &builder){
		PROFILE_FUNCTION();
		std::bitset<FAMILY_COUNT> availableFamilies;
		std::array<uint32_t, FAMILY_COUNT> families;
		families.fill(0);

		// query availables queues
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		uint32_t i=0;
		for (const auto &queueFamily : queueFamilies){
			if (queueFamily.queueCount == 0) continue;

			GET_FAMILY(FAMILY_GRAPHIC, VK_QUEUE_GRAPHICS_BIT);
			GET_FAMILY(FAMILY_COMPUTE, VK_QUEUE_COMPUTE_BIT);
			GET_FAMILY(FAMILY_TRANSFER, VK_QUEUE_TRANSFER_BIT);
			GET_FAMILY(FAMILY_PROTECTED, VK_QUEUE_PROTECTED_BIT);
			GET_FAMILY(FAMILY_SPARSE_BINDING, VK_QUEUE_SPARSE_BINDING_BIT);

			// present
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, instance->getSurface(), &presentSupport);

			if (builder.requiredFamilies[FAMILY_PRESENT] && presentSupport){
				families[FAMILY_PRESENT] = i;
				availableFamilies.set(FAMILY_PRESENT);
				this->families.push_back({i, QueueFamily::FAMILY_PRESENT, queueFamily.queueCount});
			}

			if (availableFamilies == builder.requiredFamilies) break;
			i++;
		}

		if (availableFamilies != builder.requiredFamilies){
			for (int i=0; i<FAMILY_COUNT; i++){
				printf("%d : %d\n", availableFamilies[i], builder.requiredFamilies[i]);
			}
			RND_RUNTIME_ERR("physical device : failed to find the required queue families");
		}
		
		return families;
	}

	bool PhysicalDevice::checkDeviceExtensions(VkPhysicalDevice device, PhysicalDeviceBuidler &builder){
		PROFILE_FUNCTION();
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(builder.requiredExtensions.begin(), builder.requiredExtensions.end());

		for (const auto &extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);
		
		return requiredExtensions.empty();
	}

	PhysicalDevice::SwapChainSupport PhysicalDevice::getSwapChainSupport(){
		PROFILE_FUNCTION();
		return getSwapChainSupport(physicalDevice);
	}


	PhysicalDevice::SwapChainSupport PhysicalDevice::getSwapChainSupport(VkPhysicalDevice device){
		PROFILE_FUNCTION();
		VkSurfaceKHR surface =  instance->getSurface();

		SwapChainSupport details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0){
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0){
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	bool PhysicalDevice::checkFeatures(VkPhysicalDeviceFeatures features, PhysicalDeviceBuidler &builder){
		PROFILE_FUNCTION();
		VkPhysicalDeviceFeatures requiredFeatures = builder.getFeatures();
		VkBool32* requiredArr = reinterpret_cast<VkBool32*>(&requiredFeatures);
		VkBool32* arr = reinterpret_cast<VkBool32*>(&features);

		for (int i=0; i<static_cast<int>(PhysicalDeviceFeature::FEATURES_COUNT); i++){
			if (requiredArr[i] && !arr[i]){
				return false;
			}
		}	
		return true;	
	}

	PhysicalDevice::FamilyDetails PhysicalDevice::getFamily(QueueFamily family) const{
		PROFILE_FUNCTION();
		for (auto &f : families){
			if (f.type == family) return f;
		}

		RND_RUNTIME_ERR("physical device : failed to found a required queue family");
	}

	VkFormat PhysicalDevice::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		PROFILE_FUNCTION();
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			} else if (
					tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		RND_RUNTIME_ERR("physical device : failed to found a supported format");
	}

	uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
		PROFILE_FUNCTION();
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		RND_RUNTIME_ERR("physical device : failed to found a suitable memory type");
	}

	VkPhysicalDeviceProperties& PhysicalDevice::getProperties(){
		PROFILE_FUNCTION();
		return properties;
	}

	VkPhysicalDeviceFeatures PhysicalDevice::getEnabledFeatures(){
		PROFILE_FUNCTION();
		return features;
	}

	VkPhysicalDevice PhysicalDevice::getDevice(){
		PROFILE_FUNCTION();
		return physicalDevice;
	}

	Instance* PhysicalDevice::getInstance(){
		PROFILE_FUNCTION();
		return instance;
	}

	uint32_t PhysicalDevice::getPresentFamily(){
		PROFILE_FUNCTION();
		return presentFamily;
	}
}