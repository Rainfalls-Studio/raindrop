#include "device/PhysicalDevice.hpp"

PhysicalDevice::PhysicalDevice(GfxAllocator& allocator, Instance& instance, VkPhysicalDevice physicalDevice) : _instance{instance}, _allocator{allocator}, _physicalDevice{physicalDevice}, _familiesProperties(allocator.get()), _extensions(allocator.get()){
	RAINDROP_profile_function();
	_loaded = false;
	if (!queryProperties()) return;
	if (!queryFeatures()) return;
	if (!queryExtensions()) return;
	if (!queryFamilies()) return;
	_loaded = true;
}

PhysicalDevice::~PhysicalDevice(){
	RAINDROP_profile_function();
}

bool PhysicalDevice::isFamilySuported(VkQueueFlagBits family){
	RAINDROP_profile_function();
	for (uint32 i=0; i<_familiesProperties.size(); i++){
		const auto& familyProp = _familiesProperties[i];

		if (familyProp.queueFlags & family){
			return true;
		}
	}
	return false;
}

const VkQueueFamilyProperties& PhysicalDevice::getFamilyProperties(uint32 familyIndex) const{
	RAINDROP_profile_function();
	return _familiesProperties[familyIndex];
}

const Array<VkQueueFamilyProperties>& PhysicalDevice::getFamiliesProperties() const{
	RAINDROP_profile_function();
	return _familiesProperties;
}

bool PhysicalDevice::isExtensionSupported(const char* extension, uint32 minimumVersion){
	RAINDROP_profile_function();
	if (minimumVersion == (uint32)-1){
		for (uint32 i=0; i<_extensions.size(); i++){
			if (strcmp(_extensions[i].extensionName, extension) == 0) return true;
		}
		return false;
	}

	for (uint32 i=0; i<_extensions.size(); i++){
		const auto& ext = _extensions[i];
		if (ext.specVersion <= minimumVersion && strcmp(ext.extensionName, extension) == 0) return true;
	}
	return false;
}

bool PhysicalDevice::isFeaturesSupported(VkPhysicalDeviceFeatures features){
	RAINDROP_profile_function();
	VkBool32 *required = (VkBool32*)&features;
	VkBool32 *supported = (VkBool32*)&_features;

	uint32 count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);
	for (uint32 i=0; i<count; i++){
		if (required[i] && !supported[i]) return false;
	}
	return true;
}

Array<VkPresentModeKHR> PhysicalDevice::getSurfacePresentModes(Allocator& allocator, VkSurfaceKHR surface) const{
	RAINDROP_profile_function();
	uint32 count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &count, nullptr);

	Array<VkPresentModeKHR> presentModes(allocator, count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, &count, presentModes.data());
	return presentModes;
}

Array<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormat(Allocator& allocator, VkSurfaceKHR surface) const{
	RAINDROP_profile_function();
	uint32 count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &count, nullptr);

	Array<VkSurfaceFormatKHR> formats(allocator, count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, &count, formats.data());
	return formats;
}

VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities(VkSurfaceKHR surface) const{
	RAINDROP_profile_function();
	VkSurfaceCapabilitiesKHR capabilities;
	VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, surface, &capabilities);
	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR error : %s", string_VkResult(result));
		return {};
	}
	return capabilities;
}

bool PhysicalDevice::loaded() const{
	RAINDROP_profile_function();
	return _loaded;
}

bool PhysicalDevice::queryProperties(){
	RAINDROP_profile_function();
	vkGetPhysicalDeviceProperties(_physicalDevice, &_properties);
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_memoryProperties);
	return true;
}

bool PhysicalDevice::queryFeatures(){
	RAINDROP_profile_function();
	vkGetPhysicalDeviceFeatures(_physicalDevice, &_features);
	
	return true;
}

bool PhysicalDevice::queryExtensions(){
	RAINDROP_profile_function();
	uint32 count;
	VkResult result = vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, nullptr);

	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "vkEnumerateDeviceExtensionProperties error : %s", string_VkResult(result));
		return false;
	}

	_extensions.resize(count);
	return vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &count, _extensions.data()) == VK_SUCCESS;
}

bool PhysicalDevice::queryFamilies(){
	RAINDROP_profile_function();
	uint32 count;
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, nullptr);

	_familiesProperties.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &count, _familiesProperties.data());

	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_memoryProperties);

	return true;
}

bool PhysicalDevice::isPresentSupported(uint32 queueIndex, VkSurfaceKHR surface) const{
	RAINDROP_profile_function();
	VkBool32 support;
	VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, queueIndex, surface, &support);

	if (result != VK_SUCCESS){
		RAINDROP_log(ERROR, GRAPHICS, "vkGetPhysicalDeviceSurfaceSupportKHR error : ", string_VkResult(result));
		return false;
	}
	return support == VK_TRUE;
}

const VkPhysicalDeviceProperties& PhysicalDevice::getProperties() const{
	RAINDROP_profile_function();
	return _properties;
}

const VkPhysicalDeviceFeatures& PhysicalDevice::getFeatures() const{
	RAINDROP_profile_function();
	return _features;
}

const VkPhysicalDeviceMemoryProperties& PhysicalDevice::getMemoryProperties() const{
	RAINDROP_profile_function();
	return _memoryProperties;
}

void PhysicalDevice::getPhysicalDevices(Array<PhysicalDevice*>& devices, GfxAllocator& allocator, Instance& instance){
	RAINDROP_profile_function();
	RAINDROP_log(INFO, GRAPHICS, "quering vulkan physical devices");
	uint32 count;
	vkEnumeratePhysicalDevices(instance.get(), &count, nullptr);

	Array<VkPhysicalDevice> vkDevices(allocator.stack(), count);
	vkEnumeratePhysicalDevices(instance.get(), &count, vkDevices.data());

	devices.resize(vkDevices.size());
	
	for (uint32 i=0; i<devices.size(); i++){
		devices[i] = Core::Memory::allocateNew<PhysicalDevice>(allocator.get(), allocator, instance, vkDevices[i]);
	}

	RAINDROP_log(INFO, GRAPHICS, "found %d vulkan physical devices :", devices.size());
	for (uint32 i=0; i<devices.size(); i++){
		const auto& device = *devices[i];
		RAINDROP_log(INFO, GRAPHICS, "\t - %s : %s", device.getProperties().deviceName, string_VkPhysicalDeviceType(device.getProperties().deviceType));
	}
}

VkPhysicalDevice PhysicalDevice::get() const{
	RAINDROP_profile_function();
	return _physicalDevice;
}

Instance& PhysicalDevice::instance(){
	return _instance;
}

bool PhysicalDevice::isFormatSupported(VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features){
	RAINDROP_profile_function();
	VkFormatProperties properties;
	vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &properties);

	if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) return true;
	if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features) return true;
	return false;
}

VkFormat PhysicalDevice::getFirstSupportedFormat(Array<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features){
	RAINDROP_profile_function();

	for (uint32 i=0; i<formats.size(); i++){
		const auto& format = formats[i];
		if (isFormatSupported(format, tiling, features)) return format;
	}

	return VK_FORMAT_UNDEFINED;
}

uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	RAINDROP_profile_function();
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    RAINDROP_log(ERROR, GRAPHICS, "failed to find suitable memory types");
	return -1;
}

uint32 PhysicalDevice::findFamily(VkQueueFlagBits families, bool presentSupported, VkSurfaceKHR surface){
	RAINDROP_profile_function();
	for (uint32 i=0; i<_familiesProperties.size(); i++){
		const auto& family = _familiesProperties[i];

		if (family.queueFlags & families){
			if (presentSupported){
				if (isPresentSupported(i, surface)) return i;
			}
			return i;
		}
	}

	return (uint32)-1;
}
